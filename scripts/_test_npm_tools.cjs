// Inspect existing npm installations without invoking npx or installing packages.
// Local fallbacks use a dependency-closed subset of the checked-in lockfile;
// CI continues to use the complete lockfile unchanged.
const fs = require('node:fs');
const path = require('node:path');
const { createRequire } = require('node:module');
const { execFileSync } = require('node:child_process');

// packageAt retains the original installation path so npm launchers and nested
// dependencies can keep resolving their adjacent resources.
function packageAt(root, name, version) {
  try {
    const directory = path.join(root, 'node_modules', name);
    const pkg = JSON.parse(fs.readFileSync(path.join(directory, 'package.json'), 'utf8'));
    if (pkg.version !== version) return null;
    const entry = createRequire(path.join(directory, 'package.json')).resolve(name);
    return { directory, pkg, entry };
  } catch { return null; }
}

// rendererWorks checks the Puppeteer actually resolved by the renderer rather
// than an unrelated top-level package with the right display version.
function rendererWorks(renderer, version) {
  try {
    // The reference-manual helper needs this parser/highlighter API. Loading
    // it catches incomplete dependency trees without rendering or downloading.
    const parser = require(path.join(path.dirname(renderer.entry), 'lib/get-marked-with-highlighter.js'));
    parser.getMarked({}, []);
    let directory = path.dirname(createRequire(renderer.entry).resolve('puppeteer'));
    while (directory !== path.dirname(directory)) {
      const file = path.join(directory, 'package.json');
      if (fs.existsSync(file)) {
        const pkg = JSON.parse(fs.readFileSync(file, 'utf8'));
        if (pkg.name === 'puppeteer') return pkg.version === version;
      }
      directory = path.dirname(directory);
    }
  } catch { /* An incomplete installation is reported as needing setup. */ }
  return false;
}

// The renderer may own a nested Puppeteer package. Return that dependency's
// actual installation instead of requiring an unrelated global top-level one.
function puppeteerRoot(root) {
  let loader = createRequire(path.join(root, 'node_modules', 'puppeteer', 'package.json'));
  const renderer = path.join(root, 'node_modules', 'md-to-pdf', 'package.json');
  if (fs.existsSync(renderer)) loader = createRequire(renderer);
  let directory = path.dirname(loader.resolve('puppeteer'));
  while (directory !== path.dirname(directory)) {
    const file = path.join(directory, 'package.json');
    if (fs.existsSync(file) && JSON.parse(fs.readFileSync(file, 'utf8')).name === 'puppeteer') return directory;
    directory = path.dirname(directory);
  }
  throw new Error('cannot locate the selected renderer\'s Puppeteer installation');
}

// inspect prints nonempty TSV fields for the Bash 3.2 startup table. Root order
// expresses local-before-cache preference; the caller supplies only cache roots
// in CI, where transitive versions must follow the checked-in lockfile.
function inspect(roots, versions, output = process.stdout, inventoryFile) {
  const systemRoot = roots[0];
  const names = ['markdownlint-cli', 'md-to-pdf', 'puppeteer'];
  // Only completed subset caches from this exact lockfile are eligible locally.
  const cache = roots[1];
  if (cache && (!process.env.CI || process.env.CI === 'false') && fs.existsSync(cache)) {
    for (const directory of fs.readdirSync(cache).sort()) {
      const candidate = path.join(cache, directory);
      try {
        if (directory.startsWith('subset-') && fs.readFileSync(path.join(candidate, '.ready'), 'utf8').trim() === path.basename(cache)) roots.push(candidate);
      } catch { /* Incomplete cache entries never become selectable. */ }
    }
  }
  // An interrupted full-cache installation needs repair even when its package
  // metadata happened to be extracted before the failure.
  roots = roots.filter((root, index) => {
    if (index !== 1 || !root) return true;
    try { return fs.readFileSync(path.join(root, '.ready'), 'utf8').trim() === path.basename(root); } catch { return false; }
  });
  const selected = names.map((name, index) => {
    for (const root of roots.filter(Boolean)) {
      const candidate = packageAt(root, name, versions[index]);
      if (!candidate) continue;
      if (name === 'md-to-pdf' && !rendererWorks(candidate, versions[2])) continue;
      if (name === 'markdownlint-cli') {
        try {
          const bin = typeof candidate.pkg.bin === 'string' ? candidate.pkg.bin : Object.values(candidate.pkg.bin || {})[0];
          if (!bin || execFileSync(process.execPath, [path.join(candidate.directory, bin), '--version'], {encoding:'utf8',timeout:10000}).trim() !== versions[index]) continue;
        } catch { continue; }
      }
      return { root, ...candidate };
    }
    return null;
  });
  // A missing renderer needs its matching Puppeteer dependency as part of the
  // fallback. Treat that pair as one npm dependency closure, not two installs.
  if (!selected[1]) selected[2] = null;
  else {
    const directory = puppeteerRoot(selected[1].root);
    selected[2] = { root: path.dirname(path.dirname(directory)), directory };
  }
  for (let index = 0; index < names.length; index++) {
    const candidate = selected[index];
    let found = 'missing';
    for (const root of roots.filter(Boolean)) {
      try { found = JSON.parse(fs.readFileSync(path.join(root, 'node_modules', names[index], 'package.json'), 'utf8')).version; break; } catch { /* Try the next installation. */ }
    }
    if (!candidate && found === versions[index]) found += ' (incomplete dependency graph)';
    output.write([names[index], versions[index], found, candidate ? 'reuse' : 'install', candidate?.root || '-'].join('\t') + '\n');
  }
  // Inventory is independent of selection: show the host and cache even when
  // only one supplies a complete usable graph. This sidecar never installs.
  if (inventoryFile) {
    const presentVersion = (root, name) => {
      try {
        // Report the dependency the renderer actually resolves, including a
        // nested Puppeteer instead of an unrelated global top-level package.
        const directory = name === 'puppeteer' ? puppeteerRoot(root) : path.join(root, 'node_modules', name);
        return JSON.parse(fs.readFileSync(path.join(directory, 'package.json'), 'utf8')).version;
      }
      catch { return '--'; }
    };
    const lines = names.map((name, index) => {
      const cached = [...new Set(roots.filter(root => root && root !== systemRoot).map(root => presentVersion(root, name)).filter(version => version !== '--'))];
      cached.sort((a, b) => Number(b === versions[index]) - Number(a === versions[index]));
      return [name, systemRoot ? presentVersion(systemRoot, name) : '--', cached[0] || '--'].join('\t');
    });
    fs.writeFileSync(inventoryFile, lines.join('\n') + '\n');
  }
  return selected;
}

// subsetLock follows npm's nearest-ancestor dependency lookup, including peers
// already represented by the lock. Missing required lock entries are errors;
// this code never resolves a new version from a registry.
function subsetLock(lock, names) {
  const packages = lock.packages;
  if (!packages || !packages['']) throw new Error('npm lockfile has no package graph');
  const dependencies = {};
  const result = { ...lock, packages: { '': { ...packages[''], dependencies } } };
  const visit = key => {
    if (result.packages[key]) return;
    const pkg = packages[key];
    if (!pkg) throw new Error(`npm lockfile lacks ${key}`);
    result.packages[key] = pkg;
    for (const [name] of Object.entries({ ...pkg.dependencies, ...pkg.optionalDependencies, ...pkg.peerDependencies })) {
      let parent = key;
      let found;
      while (true) {
        const candidate = `${parent ? parent + '/' : ''}node_modules/${name}`;
        if (packages[candidate]) { found = candidate; break; }
        const ancestor = parent.lastIndexOf('/node_modules/');
        if (!parent) break;
        parent = ancestor < 0 ? '' : parent.slice(0, ancestor);
      }
      if (found) visit(found);
      else if (!(name in (pkg.optionalDependencies || {})) && !pkg.peerDependenciesMeta?.[name]?.optional) {
        throw new Error(`npm lockfile lacks dependency ${name} of ${key}`);
      }
    }
  };
  for (const name of names) {
    if (!packages[''].dependencies?.[name]) throw new Error(`npm lockfile has no direct dependency ${name}`);
    dependencies[name] = packages[''].dependencies[name];
    visit(`node_modules/${name}`);
  }
  return result;
}

// writeSubset is called only after the startup summary and explicit consent.
// All output is disposable worktree-local npm input, never tracked source.
function writeSubset(source, destination, names) {
  const lock = subsetLock(JSON.parse(fs.readFileSync(source, 'utf8')), names);
  fs.mkdirSync(destination, { recursive: true });
  fs.writeFileSync(path.join(destination, 'package.json'), JSON.stringify({ name: lock.name, private: true, dependencies: lock.packages[''].dependencies }, null, 2) + '\n');
  fs.writeFileSync(path.join(destination, 'package-lock.json'), JSON.stringify(lock, null, 2) + '\n');
}

if (require.main === module) {
  try {
    const [operation, ...args] = process.argv.slice(2);
    if (operation === 'inspect' && (args.length === 5 || args.length === 6)) inspect(args.slice(0, 2), args.slice(2, 5), process.stdout, args[5]);
    else if (operation === 'lock' && args.length >= 3) writeSubset(args[0], args[1], args.slice(2));
    else if (operation === 'cli' && args.length === 2) {
      const directory = path.join(args[0], 'node_modules', args[1]);
      const pkg = JSON.parse(fs.readFileSync(path.join(directory, 'package.json'), 'utf8'));
      const bin = typeof pkg.bin === 'string' ? pkg.bin : Object.values(pkg.bin || {})[0];
      if (!bin || !fs.existsSync(path.join(directory, bin))) throw new Error(`missing npm executable for ${args[1]}`);
      console.log(path.join(directory, bin));
    } else if (operation === 'puppeteer-root' && args.length === 1) console.log(puppeteerRoot(args[0]));
    else if (operation === 'browser' && args.length === 1) {
      const loader = createRequire(path.join(puppeteerRoot(args[0]), 'package.json'));
      const puppeteer = loader('puppeteer');
      const executable = puppeteer.executablePath();
      if (!fs.existsSync(executable)) throw new Error(`missing Puppeteer browser ${executable}`);
      const version = require('node:child_process').execFileSync(executable, ['--version'], { encoding: 'utf8', timeout: 10000 });
      if (!version.split(/\s+/).includes(puppeteer.browserVersion)) throw new Error(`browser must match Puppeteer revision ${puppeteer.browserVersion}`);
      console.log(executable);
    }
    else throw new Error('usage: npm-tools inspect installed-root cache-root markdownlint md-to-pdf puppeteer | lock source destination names...');
  } catch (error) {
    console.error(`INCOMPLETE: ${error.message}`);
    process.exitCode = 2;
  }
}

module.exports = { inspect, subsetLock, rendererWorks };

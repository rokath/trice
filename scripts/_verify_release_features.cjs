// Verify shipped tools and target sources together, without touching examples.
// Node is already part of the release environment; only standard modules are used.
const assert = require('node:assert/strict');
const fs = require('node:fs');
const path = require('node:path');
const { spawnSync } = require('node:child_process');

// Repository paths are resolved once; child compilers receive relative paths.
const repositoryRoot = path.resolve(__dirname, '..');

// Missing artifacts must name the expected path before any compilation starts.
function requireFile(filename) {
  assert.ok(fs.existsSync(filename) && fs.statSync(filename).isFile() && fs.statSync(filename).size > 0,
    `missing or empty release artifact: ${filename}`);
}

// Compare the complete notice, including copyright and permission paragraphs.
function verifyLicense(filename, reference = path.join(repositoryRoot, 'LICENSE.md')) {
  requireFile(filename);
  requireFile(reference);
  assert.ok(fs.readFileSync(filename).equals(fs.readFileSync(reference)),
    `license content differs from LICENSE.md: ${filename}`);
}

// Preserve decoder stdout separately from diagnostics; failed commands are fatal.
function run(command, args, cwd, environment = process.env, report = false) {
  const result = spawnSync(command, args, { cwd, env: environment, encoding: 'utf8' });
  if (result.error || result.status !== 0) {
    throw new Error(`${command} ${args.join(' ')} failed in ${cwd}: ${result.error?.message || result.stderr || result.stdout || result.status}`);
  }
  if (report) {
    process.stdout.write(result.stdout);
    process.stderr.write(result.stderr);
  }
  return result.stdout;
}

// Parse every stdout line so banners or diagnostics cannot masquerade as NDJSON.
// Expected values come from the existing PC feature tour's application contract.
function verifyEvents(output, decoder) {
  const events = output.split(/\r?\n/).filter(line => line.trim() !== '').map((line, index) => {
    let event;
    try {
      event = JSON.parse(line);
    } catch {
      throw new Error(`${decoder}: stdout line ${index + 1} is not a JSON application event`);
    }
    assert.ok(event && typeof event === 'object' && typeof event.message === 'string',
      `${decoder}: stdout line ${index + 1} is not an application event`);
    return event;
  });
  assert.equal(events.length, 10, `${decoder}: expected all ten PC feature records`);
  // Finding by message proves values crossed the target/decoder boundary.
  const record = message => {
    const found = events.filter(event => event.message === message);
    assert.equal(found.length, 1, `${decoder}: missing or duplicated record ${JSON.stringify(message)}`);
    return found[0];
  };
  assert.deepEqual(record('Device pump A\n').fields, { device: 'pump A' });
  assert.equal(record('Phase 7\n').ts32, '0:00:00,100');
  assert.deepEqual(record('Phase 7\n').fields, { phase: 7 });
  assert.equal(record('Humidity 55 percent\n').ts16, '7');
  assert.deepEqual(record('Humidity 55 percent\n').fields, { humidity_pct: 55 });
  assert.deepEqual(record('Supply 3300 mV, cycle=7\n').fields, { voltage_mv: 3300, cycle: 7 });
  assert.deepEqual(record('Supply 3250 mV, cycle=11\n').fields, { voltage_mv: 3250, cycle: 11 });
  assert.equal(record('Supply 3250 mV, cycle=11\n').ts32Delta, '0:00:00,025');
  assert.equal(record('Retry 2\n').tag, 'WARNING');
  assert.equal(record('A message without a tag\n').tag, 'untagged');
  assert.equal(record('41 00 ff ').tag, 'RECEIVE');
  return events;
}

// Require the same ten records and enriched values in both readable formats.
function verifyReadable(output, format, decoder) {
  const lines = output.split(/\r?\n/).filter(line => line.trim() !== '');
  assert.equal(lines.length, 10, `${decoder}: ${format} output contains missing records or non-record lines`);
  if (format === 'text') {
    assert.ok(output.includes('Supply 3300 mV, cycle=7') && output.includes('Supply 3250 mV, cycle=11'),
      `${decoder}: text output lost Context Enrichment values`);
    assert.ok(output.includes('Device pump A') && output.includes('Humidity 55 percent'),
      `${decoder}: text output lost structured values`);
  } else {
    assert.ok(lines.every(line => line.startsWith('tag=') && line.includes(' message=')),
      `${decoder}: KV stdout contains a non-record line`);
    assert.ok(output.includes('field.voltage_mv=3300 field.cycle=7') && output.includes('field.voltage_mv=3250 field.cycle=11'),
      `${decoder}: KV output lost structured or Context Enrichment fields`);
  }
}

// Remove case-variant Windows PATH entries before selecting the shipped tools.
// Node child-process environments otherwise choose one duplicate unpredictably.
function toolEnvironment(toolRoot, inherited = process.env) {
  const environment = { ...inherited };
  const pathKey = Object.keys(environment).find(key => key.toLowerCase() === 'path') || 'PATH';
  const originalPath = environment[pathKey] || '';
  for (const key of Object.keys(environment)) {
    if (key.toLowerCase() === 'path' && key !== pathKey) delete environment[key];
  }
  environment[pathKey] = toolRoot + path.delimiter + originalPath;
  return environment;
}

// Place the shipped binaries first in PATH so the copied tour cannot silently
// use a developer installation. All Bind writes and captures stay in this tree.
function verifyFeatures(toolDirectory, targetArchive) {
  assert.ok(toolDirectory, 'missing unpacked host-tool directory');
  assert.ok(targetArchive, 'missing target-source archive: expected trice_target_sources_*.zip');
  const toolRoot = path.resolve(toolDirectory);
  const archive = path.resolve(targetArchive);
  const suffix = process.platform === 'win32' ? '.exe' : '';
  for (const tool of ['trice', 'tlog']) requireFile(path.join(toolRoot, tool + suffix));
  requireFile(archive);
  verifyLicense(path.join(toolRoot, 'LICENSE.md'));
  const temporaryRoot = path.join(repositoryRoot, 'temp');
  fs.mkdirSync(temporaryRoot, { recursive: true });
  // Retain each isolated run as evidence, including when a check fails.
  const workspace = fs.mkdtempSync(path.join(temporaryRoot, 'release-features-'));
  console.log(`Release feature workspace: ${path.relative(repositoryRoot, workspace)}`);
  fs.copyFileSync(archive, path.join(workspace, 'target.zip'));
  run('unzip', ['-q', 'target.zip', '-d', 'target'], workspace);
  const roots = fs.readdirSync(path.join(workspace, 'target'), { withFileTypes: true })
    .filter(entry => entry.isDirectory() && entry.name.startsWith('trice_target_sources_'));
  assert.equal(roots.length, 1, `expected one target-source root inside ${archive}`);
  const targetRoot = path.join(workspace, 'target', roots[0].name);
  verifyLicense(path.join(targetRoot, 'LICENSE.md'));
  for (const filename of ['trice.h', 'trice.c', 'SEGGER_RTT_ConfDefaults.h', 'SEGGER_RTT_LICENSE.md', 'default_conf/SEGGER_RTT_Conf.h']) {
    requireFile(path.join(targetRoot, 'src', filename));
  }
  fs.cpSync(path.join(targetRoot, 'src'), path.join(workspace, 'src'), { recursive: true });
  const example = path.join(workspace, 'examples', 'PC_features');
  fs.mkdirSync(example, { recursive: true });
  // No dictionaries or generated headers are copied: Bind populates a fresh TIL.
  for (const filename of ['main.c', 'triceConfig.h', 'build_and_run.sh', 'check_output.sh', 'show_text.sh', 'show_json.sh', 'show_kv.sh']) {
    fs.copyFileSync(path.join(repositoryRoot, 'examples', 'PC_features', filename), path.join(example, filename));
  }
  // Bind intentionally requires an existing TIL to avoid accidental ID resets.
  fs.writeFileSync(path.join(example, 'til.json'), '{}\n');
  const environment = toolEnvironment(toolRoot);
  // A Windows 'bash' lookup can select WSL; callers pass their native Git Bash.
  run(environment.RELEASE_FEATURE_BASH || 'bash', ['check_output.sh'], example, environment, true);
  requireFile(path.join(example, 'capture.bin'));
  // Exercise both shipped entry points against the same newly produced records.
  const options = ['-p', 'FILEBUFFER', '-args', 'capture.bin', '-pf', 'TCOBSv1', '-d16',
    '-til', 'til.json', '-li', 'li.json', '-hs', 'off', '-color', 'none',
    '-ts16', 'phase:%d', '-ts32', 'ms', '-ts32delta', 'ms', '-ulabel', 'sensor:450'];
  let referenceEvents;
  for (const tool of ['trice', 'tlog']) {
    const executable = path.join(toolRoot, tool + suffix);
    const prefix = tool === 'trice' ? ['log'] : [];
    for (const format of ['text', 'json', 'kv']) {
      const output = run(executable, [...prefix, ...options, '-logFormat', format], example, environment);
      fs.writeFileSync(path.join(workspace, `${tool}-${format}.txt`), output);
      if (format === 'json') {
        const events = verifyEvents(output, tool);
        if (referenceEvents) assert.deepEqual(events, referenceEvents, 'trice and tlog decoded different events');
        else referenceEvents = events;
      } else {
        verifyReadable(output, format, tool);
      }
    }
  }
  console.log('PASS: shipped trice/tlog and target sources preserve Bind, CE, structured fields and text/JSON/KV records; complete licenses match.');
}

// Unit tests use the same observable checks without rebuilding release packages.
module.exports = { verifyLicense, verifyEvents, verifyReadable, verifyFeatures, toolEnvironment };
if (require.main === module) {
  try {
    assert.equal(process.argv.length, 4, 'usage: node scripts/_verify_release_features.cjs TOOL_DIRECTORY TARGET_ZIP');
    verifyFeatures(process.argv[2], process.argv[3]);
  } catch (error) {
    console.error(`FAIL: ${error.message}`);
    // Artifact failures retain actionable context without replacing the error.
    console.error(`Node ${process.version}; ${process.platform}/${process.arch}`);
    const directories = new Set([path.dirname(process.argv[2] || '.'), path.dirname(process.argv[3] || '.'), 'artifacts', 'dist']);
    for (const directory of directories) {
      if (fs.existsSync(directory) && fs.statSync(directory).isDirectory()) {
        console.error(`${directory}: ${fs.readdirSync(directory).join(', ')}`);
      }
    }
    process.exitCode = 1;
  }
}

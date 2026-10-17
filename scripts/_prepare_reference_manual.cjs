// Prepare disposable RM chapters with the PDF renderer's own Markdown parser.
// HTML bodies preserve code/comment boundaries while stitchmd supplies the book
// structure. Only temp/manual-pdf is written; the maintained chapters stay intact.
const fs = require('node:fs');
const path = require('node:path');
const { execFileSync } = require('node:child_process');

// Reuse the packages selected by npx instead of adding another parser dependency.
function loadRenderer() {
  for (const directory of process.env.PATH.split(path.delimiter)) {
    try {
      const entry = require.resolve('md-to-pdf', { paths: [path.resolve(directory, '..')] });
      return require(path.join(path.dirname(entry), 'lib/get-marked-with-highlighter.js')).getMarked({}, []);
    } catch (error) {
      if (error.code !== 'MODULE_NOT_FOUND') throw error;
    }
  }
  throw new Error('md-to-pdf parser not found; run this helper through the PDF wrapper');
}

// Reject absent/empty inputs before a previous PDF can be mistaken for new output.
function readSource(filename) {
  if (!fs.existsSync(filename) || !fs.statSync(filename).isFile() || fs.statSync(filename).size === 0) {
    throw new Error(`missing or empty reference source: ${filename}`);
  }
  return fs.readFileSync(filename, 'utf8');
}

// Only the fixed build directory may be reset. Symlinked parents/targets must not
// redirect recursive cleanup into another workspace or a user's unrelated files.
function resetStage(repoRoot) {
  const temporary = path.join(repoRoot, 'temp');
  fs.mkdirSync(temporary, { recursive: true });
  if (fs.realpathSync(temporary) !== path.join(fs.realpathSync(repoRoot), 'temp')) {
    throw new Error('PDF staging parent must be the repository temp directory');
  }
  const stage = path.join(temporary, 'manual-pdf');
  if (fs.existsSync(stage) && fs.lstatSync(stage).isSymbolicLink()) {
    throw new Error('PDF staging directory must not be a symlink: temp/manual-pdf');
  }
  fs.rmSync(stage, { recursive: true, force: true });
  fs.mkdirSync(path.join(stage, 'chapters'), { recursive: true });
  return stage;
}

// Locate the authored details wrapper through Markdown tokens, so examples and
// commented-out containers are never removed merely because their text matches.
function withoutChapterContents(marked, source) {
  const tokens = marked.lexer(source);
  if (!tokens.some(token => token.type === 'html' && /^<!-- mdtoc -->/.test(token.raw))) return source;
  let offset = 0;
  let opening;
  let managed = false;
  for (const token of tokens) {
    const start = source.indexOf(token.raw, offset);
    if (start < 0) throw new Error('cannot locate Markdown token in reference source');
    offset = start + token.raw.length;
    if (token.type !== 'html') continue;
    if (/^<details\b/i.test(token.raw)) opening = start;
    else if (opening !== undefined && /^<!-- mdtoc -->/.test(token.raw)) managed = true;
    else if (opening !== undefined && /^<\/details>/i.test(token.raw)) {
      if (managed) return source.slice(0, opening) + source.slice(offset);
      opening = undefined;
    }
  }
  throw new Error('managed chapter Contents must have a complete details wrapper');
}

// Operate on actual HTML tags while retaining comments verbatim. Fenced/inline
// code is handled separately by marked and never enters this HTML transformation.
function htmlTags(html, transform) {
  return html.split(/(<!--[\s\S]*?-->)/g).map(part => part.startsWith('<!--') ? part : part.replace(/<[a-z][^>]*>/gi, transform)).join('');
}

// Escape generated attributes and labels without interpreting them as markup.
function escapeHtml(value) {
  return value.replace(/&/g, '&amp;').replace(/"/g, '&quot;').replace(/</g, '&lt;').replace(/>/g, '&gt;');
}

// Build both heading aliases and explicit authored anchors before resolving any
// cross-chapter links. Canonical heading IDs and authored IDs use separate spaces.
function inspectDocument(marked, filename, label, source, group) {
  const namespace = `rm-${encodeURIComponent(path.basename(filename, '.md')).replace(/%/g, '_')}`;
  const document = { filename, label, group, source: withoutChapterContents(marked, source), namespace, anchors: new Map(), headings: [] };
  const tokens = marked.lexer(document.source);
  marked.walkTokens(tokens, token => {
    if (token.type !== 'html' || token.pre) return;
    htmlTags(token.raw, tag => {
      for (const match of tag.matchAll(/\s(?:id|name)\s*=\s*(["'])(.*?)\1/gi)) {
        if (document.anchors.has(match[2])) throw new Error(`duplicate source anchor ${filename}#${match[2]}`);
        document.anchors.set(match[2], `${namespace}--anchor-${encodeURIComponent(match[2]).replace(/%/g, '_')}`);
      }
      return tag;
    });
  });
  const renderer = new marked.Renderer();
  renderer.heading = (text, depth, raw, slugger) => {
    const main = depth === 1 && !document.headings.some(heading => heading.main);
    const id = main ? `${namespace}--chapter` : `${namespace}--heading-${document.headings.length}`;
    const slug = slugger.slug(raw);
    if (!document.anchors.has(slug)) document.anchors.set(slug, id);
    document.headings.push({ depth, main, id, text: text.replace(/<[^>]*>/g, '') });
    return '';
  };
  // Metadata collection does not need to highlight hundreds of code examples.
  renderer.code = () => '';
  marked.parser(tokens, { ...marked.defaults, renderer });
  if (!document.headings.some(heading => heading.main)) throw new Error(`missing main heading: ${filename}`);
  document.tokens = tokens;
  return document;
}

// Use the current commit for repository links in release PDFs; fixtures and
// exported checkouts without Git retain the existing main-branch fallback.
function sourceRef(repoRoot) {
  try {
    return execFileSync('git', ['rev-parse', 'HEAD'], { cwd: repoRoot, encoding: 'utf8', stdio: ['ignore', 'pipe', 'ignore'] }).trim();
  } catch {
    return 'main';
  }
}

// Resolve included-document links inside the PDF. Other repository links become
// durable GitHub URLs; images remain local and must exist before rendering starts.
function destination(value, document, documents, repoRoot, stage, ref, image) {
  if (/^(?:[a-z][a-z0-9+.-]*:|\/\/)/i.test(value)) return value;
  const parsed = new URL(value, 'https://local.invalid/');
  const relative = decodeURIComponent(value.split(/[?#]/, 1)[0]);
  const filename = relative ? path.resolve(path.dirname(document.filename), relative) : document.filename;
  const included = documents.get(filename);
  if (!image && included) {
    const fragment = decodeURIComponent(parsed.hash.slice(1));
    const target = fragment ? included.anchors.get(fragment) : included.headings.find(heading => heading.main).id;
    if (!target) throw new Error(`unknown reference destination: ${document.filename}: ${value}`);
    return `#${target}`;
  }
  if (!image && filename === path.join(repoRoot, 'docs/rm/README.md')) return '#rm-start';
  const repositoryPath = path.relative(repoRoot, filename);
  if (repositoryPath === '..' || repositoryPath.startsWith(`..${path.sep}`) || path.isAbsolute(repositoryPath)) {
    throw new Error(`reference destination escapes repository: ${value}`);
  }
  if (image) {
    if (!fs.existsSync(filename)) throw new Error(`missing reference image: ${document.filename}: ${value}`);
    return path.relative(path.join(stage, 'chapters'), filename).split(path.sep).join('/') + parsed.search + parsed.hash;
  }
  const kind = fs.existsSync(filename) && fs.statSync(filename).isDirectory() ? 'tree' : 'blob';
  return `https://github.com/rokath/trice/${kind}/${encodeURIComponent(ref)}/${repositoryPath.split(path.sep).map(encodeURIComponent).join('/')}${parsed.search}${parsed.hash}`;
}

// Replace chapter-local mdtoc prefixes without disturbing anchors or inline
// formatting. A version or size at the start of a title is not a dotted prefix.
function withoutSectionNumber(text) {
  return text.replace(/^\d+(?:\.\d+)*\.\s+/, '');
}

// Number the visible heading hierarchy under its README-ordered main chapter.
// Each parent owns a sibling count; skipped Markdown levels attach to the nearest
// visible parent instead of creating zero-numbered, nonexistent sections.
function numberSections(document, chapterNumber) {
  const ancestors = [{ depth: 1, number: String(chapterNumber), children: 0 }];
  for (const heading of document.headings) {
    if (heading.main || heading.depth === 1) continue;
    while (ancestors.length > 1 && ancestors[ancestors.length - 1].depth >= heading.depth) ancestors.pop();
    const parent = ancestors[ancestors.length - 1];
    heading.number = `${parent.number}.${++parent.children}`;
    heading.text = `${heading.number}. ${withoutSectionNumber(heading.text)}`;
    ancestors.push({ depth: heading.depth, number: heading.number, children: 0 });
  }
}

// Render a prepared HTML body with the same parser/highlighter as md-to-pdf.
// Its Markdown H1 remains available for stitchmd's normal chapter promotion.
function renderDocument(marked, document, documents, repoRoot, stage, ref) {
  const renderer = new marked.Renderer();
  const standard = new marked.Renderer();
  let headingIndex = 0;
  // Isolate pre blocks from preceding HTML paragraphs. Otherwise the book's
  // final Markdown parse can treat blank lines inside code as new paragraphs.
  renderer.code = (code, language, escaped) => `\n\n${standard.code.call(renderer, code, language, escaped)}\n\n`;
  renderer.heading = text => {
    const heading = document.headings[headingIndex++];
    if (heading.main) return '';
    const level = Math.min(heading.depth + 1, 6);
    const label = heading.number ? `${heading.number}. ${withoutSectionNumber(text)}` : text;
    return `\n\n<h${level} id="${heading.id}">${label}</h${level}>\n\n`;
  };
  renderer.link = (href, title, text) => standard.link(destination(href, document, documents, repoRoot, stage, ref, false), title, text);
  renderer.image = (href, title, text) => standard.image(destination(href, document, documents, repoRoot, stage, ref, true), title, text);
  renderer.html = html => htmlTags(html, tag => tag.replace(/\s(id|name|href|src)\s*=\s*(["'])(.*?)\2/gi, (attribute, name, quote, value) => {
    const lower = name.toLowerCase();
    const rewritten = lower === 'id' || lower === 'name'
      ? document.anchors.get(value)
      : destination(value.replace(/&amp;/g, '&'), document, documents, repoRoot, stage, ref, lower === 'src');
    if (!rewritten) throw new Error(`unregistered HTML anchor: ${document.filename}#${value}`);
    return ` ${name}=${quote}${escapeHtml(rewritten)}${quote}`;
  })).replace(/(<!--[\s\S]*?-->)/g, '\n\n$1\n\n');
  const body = marked.parser(document.tokens, { ...marked.defaults, renderer });
  const main = document.headings.find(heading => heading.main);
  // A print hook marks main chapters for page breaks without changing source
  // headings or forcing separate pages for the introduction and footer.
  const chapterClass = ['HEADER.md', 'FOOTER.md'].includes(path.basename(document.filename))
    ? '' : ' class="rm-main-chapter"';
  // Contain authored HTML in its chapter so an unclosed list/container cannot
  // indent later chapter headings when Chromium repairs the combined document.
  return `# <a id="${main.id}"${chapterClass}></a>${escapeHtml(document.label)}\n\n<article class="rm-chapter">\n\n${body}\n\n</article>\n`;
}

// Generate one navigable Contents chapter from headings, not chapter-local ToCs.
function contents(documents) {
  const lines = ['# Contents', '', '<ul class="book-contents">'];
  let group;
  for (const document of documents.values()) {
    // README groups organize the contents only; they never consume chapter numbers.
    if (document.group !== group) {
      group = document.group;
      if (group) lines.push(`<li class="toc-group" style="list-style:none; font-size:2.5em; line-height:1.15; margin:0.6em 0 0.2em; break-after:avoid-page"><strong>${escapeHtml(group)}</strong></li>`);
    }
    const main = document.headings.find(heading => heading.main);
    lines.push(`<li><a href="#${main.id}">${escapeHtml(document.label)}</a>`);
    const sections = document.headings.filter(heading => !heading.main && heading.depth <= 4);
    if (sections.length) {
      lines.push('<ul>');
      for (const heading of sections) lines.push(`<li class="toc-depth-${heading.depth}"><a href="#${heading.id}">${heading.text}</a></li>`);
      lines.push('</ul>');
    }
    lines.push('</li>');
  }
  lines.push('</ul>', '');
  return lines.join('\n');
}

// Compile the manually maintained README order and retain the intermediate book
// inputs under temp/manual-pdf for diagnostics until the next generation resets it.
function prepare(marked, repoRoot) {
  const readme = path.join(repoRoot, 'docs/rm/README.md');
  const source = readSource(readme);
  const documents = new Map();
  // Number main chapters and their full subsection hierarchy only in the book;
  // framing documents and maintained sources retain their existing labels.
  let chapterNumber = 0;
  let group;
  marked.walkTokens(marked.lexer(source), token => {
    // Keep the authored group labels without introducing divider pages in the book.
    if (token.type === 'heading' && token.depth === 2) {
      group = token.text;
      return;
    }
    if (token.type !== 'link' || /^(?:[a-z][a-z0-9+.-]*:|#|\/\/)/i.test(token.href)) return;
    const filename = path.resolve(path.dirname(readme), decodeURIComponent(token.href));
    if (path.dirname(filename) !== path.dirname(readme) || path.extname(filename) !== '.md' || filename === readme) return;
    if (documents.has(filename)) throw new Error(`duplicate reference chapter in README: ${token.href}`);
    const framing = ['HEADER.md', 'FOOTER.md'].includes(path.basename(filename));
    const label = framing ? token.text : `${++chapterNumber}. ${token.text}`;
    const document = inspectDocument(marked, filename, label, readSource(filename), framing ? undefined : group);
    if (!framing) numberSections(document, chapterNumber);
    documents.set(filename, document);
  });
  if (!documents.size) throw new Error('no reference chapters listed in docs/rm/README.md');
  const stage = resetStage(repoRoot);
  const ref = sourceRef(repoRoot);
  const summary = ['# <a id="rm-start"></a>Trice Reference Manual', ''];
  let insertedContents = false;
  for (const document of documents.values()) {
    if (!insertedContents && path.basename(document.filename) !== 'HEADER.md') {
      summary.push('- [Contents](_contents.md)');
      insertedContents = true;
    }
    const filename = path.basename(document.filename);
    fs.writeFileSync(path.join(stage, 'chapters', filename), renderDocument(marked, document, documents, repoRoot, stage, ref));
    summary.push(`- [${document.label}](chapters/${filename})`);
  }
  if (!insertedContents) summary.push('- [Contents](_contents.md)');
  fs.writeFileSync(path.join(stage, '_contents.md'), contents(documents));
  fs.writeFileSync(path.join(stage, 'SUMMARY.md'), summary.join('\n') + '\n');
  fs.writeFileSync(path.join(stage, 'sources.json'), JSON.stringify({ ref, sources: [...documents.keys()].map(filename => path.relative(repoRoot, filename).split(path.sep).join('/')) }, null, 2) + '\n');
  console.log(`Prepared ${documents.size} reference sources in temp/manual-pdf`);
}

// Validate the actual stitched document rather than assuming stitchmd preserved
// every explicit anchor. Keep its HTML as a useful, inspectable intermediate.
function verify(marked, repoRoot) {
  const stage = path.join(repoRoot, 'temp/manual-pdf');
  const html = marked.parse(readSource(path.join(stage, 'TriceReferenceManual.md')));
  const ids = new Set();
  const links = [];
  htmlTags(html, tag => {
    for (const match of tag.matchAll(/\b(id|href)\s*=\s*(["'])(.*?)\2/gi)) {
      if (match[1].toLowerCase() === 'id') {
        if (ids.has(match[3])) throw new Error(`duplicate generated PDF anchor: ${match[3]}`);
        ids.add(match[3]);
      } else if (match[3].startsWith('#')) links.push(decodeURIComponent(match[3].slice(1)));
    }
    return tag;
  });
  for (const target of links) if (!ids.has(target)) throw new Error(`missing generated PDF anchor: ${target}`);
  fs.writeFileSync(path.join(stage, 'TriceReferenceManual.html'), html);
  console.log(`Verified ${links.length} internal PDF links and ${ids.size} unique anchors`);
}

// Both modes are invoked by the shell wrapper inside the selected npx environment.
if (require.main === module) {
  try {
    const marked = loadRenderer();
    const repoRoot = path.resolve(__dirname, '..');
    if (process.argv[2] === '--verify') verify(marked, repoRoot);
    else if (process.argv.length === 2) prepare(marked, repoRoot);
    else throw new Error('usage: node scripts/_prepare_reference_manual.cjs [--verify]');
  } catch (error) {
    console.error(`ERROR: ${error.message}`);
    process.exitCode = 1;
  }
}

module.exports = { prepare, verify, withoutChapterContents };

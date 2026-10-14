// Render-time adjustments keep Markdown links relative in the repository while
// making exported PDFs usable after the temporary local HTTP server has stopped.
const {execFileSync} = require('node:child_process');
const path = require('node:path');

// Pin exported navigation to the sources used for this build. Source archives
// without Git metadata retain the repository's default branch as a fallback.
let sourceRef = 'main';
try {
  sourceRef = execFileSync('git', ['rev-parse', 'HEAD'], {
    cwd: path.resolve(__dirname, '..'),
    encoding: 'utf8',
    stdio: ['ignore', 'pipe', 'ignore'],
  }).trim() || sourceRef;
} catch {
  // An exported source tree can still produce a usable standalone PDF.
}
const repositoryBase = 'https://github.com/rokath/trice/blob/' + encodeURIComponent(sourceRef);

module.exports = {
  script: [{
    content: `
      // Chromium may hide the content of a closed details element even when
      // print CSS sets its children to display:block. Open it before printing.
      for (const details of document.querySelectorAll('details')) {
        details.open = true;
      }

      // Return navigation targets the whole printed book. Chapter sources keep
      // their local top links so they remain useful when read separately.
      const documentStart = document.querySelector('h1[id]');

      // Chromium uses the page title for PDF metadata; never export the
      // temporary renderer address as the title of a release manual.
      const title = documentStart && (documentStart.textContent || '').trim();
      if (title) document.title = title;

      // Preserve in-document destinations and external URLs. Only links served
      // from md-to-pdf's temporary origin become repository source links;
      // images still load locally so PDF rendering needs no remote image fetch.
      for (const link of document.querySelectorAll('a[href]')) {
        const href = link.getAttribute('href');
        if (!href) continue;
        const returnToStart = documentStart && /^\\s*(?:back|go)\\s+to\\s+top\\s*$/i.test(link.textContent || '');
        if (href.startsWith('#')) {
          if (returnToStart) link.setAttribute('href', '#' + documentStart.id);
          continue;
        }
        const target = new URL(href, document.location.href);
        if (target.origin !== document.location.origin) continue;
        if (target.pathname === document.location.pathname) {
          link.setAttribute('href', returnToStart ? '#' + documentStart.id : target.hash || '#');
        } else {
          link.setAttribute('href', ${JSON.stringify(repositoryBase)}
            + target.pathname + target.search + target.hash);
        }
      }
    `,
  }],
};

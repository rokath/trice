// Render-time adjustments keep Markdown links relative in the repository while
// making exported PDFs usable after the temporary local HTTP server has stopped.
module.exports = {
  script: [{
    content: `
      // Chromium may hide the content of a closed details element even when
      // print CSS sets its children to display:block. Open it before printing.
      for (const details of document.querySelectorAll('details')) {
        details.open = true;
      }

      // Preserve in-document destinations and external URLs. Only links served
      // from md-to-pdf's temporary origin become repository source links;
      // images still load locally so PDF rendering needs no remote image fetch.
      for (const link of document.querySelectorAll('a[href]')) {
        const href = link.getAttribute('href');
        if (!href || href.startsWith('#')) continue;
        const target = new URL(href, document.location.href);
        if (target.origin !== document.location.origin) continue;
        if (target.pathname === document.location.pathname) {
          link.setAttribute('href', target.hash || '#');
        } else {
          link.setAttribute('href', 'https://github.com/rokath/trice/blob/main'
            + target.pathname + target.search + target.hash);
        }
      }
    `,
  }],
};

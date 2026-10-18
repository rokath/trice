#!/usr/bin/env python3
"""Prepare the same limited Jekyll source tree for local and CI builds."""

from pathlib import Path
import posixpath
import re
import shutil
from urllib.parse import unquote, urlsplit


# Resolve once from this script, so callers need not run from the checkout root.
ROOT = Path(__file__).resolve().parents[1]
OUTPUT = ROOT / "temp/pages-source"
GITHUB = "https://github.com/rokath/trice/blob/main/"
IMAGES = {".png", ".gif", ".jpg", ".jpeg", ".svg", ".webp", ".ico"}
# Match linked image previews as a unit; their inner image is handled separately.
LINK = re.compile(r'(?P<image>!)?\[(?:!\[[^\]\n]*\]\([^)\n]+\)|[^\[\]\n]*)\]\((?P<url>[^)\n]+)\)')
ATTRIBUTE = re.compile(r'\b(?P<kind>href|src)\s*=\s*(?P<quote>["\'])(?P<url>.*?)(?P=quote)')


def read_source(path):
    """Reject missing, empty or escaping inputs before replacing the staging tree."""
    source = ROOT / path
    if not source.resolve().is_relative_to(ROOT):
        raise ValueError(f"source escapes repository: {path}")
    if not source.is_file() or source.stat().st_size == 0:
        raise ValueError(f"missing or empty Pages source: {path}")
    return source.read_bytes()


def rewrite(text, document, pages, files):
    """Rewrite visible links and collect resources, preserving authored examples."""
    def destination(value, image=False):
        # Titles after Markdown destinations are retained byte for byte.
        token = re.match(r'(<[^>]+>|\S+)(.*)', value)
        if not token:
            return value
        url, title = token.groups()
        angled = url.startswith("<")
        url = url[1:-1] if angled else url
        # Existing absolute links to this repository may also name rendered pages.
        local = url[len(GITHUB):] if url.startswith(GITHUB) else None
        parsed = urlsplit(local if local is not None else url)
        if not parsed.path or parsed.scheme or parsed.netloc or parsed.path.startswith("/"):
            return value
        target = posixpath.normpath(posixpath.join("" if local is not None else document.parent.as_posix(), unquote(parsed.path)))
        if target == ".." or target.startswith("../"):
            raise ValueError(f"link escapes repository in {document}: {value}")
        suffix = ("?" + parsed.query if parsed.query else "") + ("#" + parsed.fragment if parsed.fragment else "")
        if target in pages:
            # GitHub Pages renders each README as its directory's index page.
            rendered = posixpath.join(posixpath.dirname(target), "index.html") if posixpath.basename(target) == "README.md" else target[:-3] + ".html"
            result = posixpath.relpath(rendered, document.parent.as_posix())
            if parsed.path.startswith("./") and not result.startswith("."):
                result = "./" + result
        elif image or Path(target).suffix.lower() in IMAGES:
            # Only referenced resources enter the site; unrelated assets stay out.
            files.setdefault(target, read_source(target))
            result = parsed.path if local is None else posixpath.relpath(target, document.parent.as_posix())
        else:
            return GITHUB + target + suffix + title
        result += suffix
        return ("<" + result + ">" if angled else result) + title

    def visible(line):
        # Excluding matches inside inline code still permits code-formatted labels.
        def replace(match, spans):
            if any(start <= match.start() < end for start, end in spans):
                return match[0]
            start, end = match.span("url")
            prefix = match[0][:start - match.start()]
            if match.re is LINK and prefix.startswith("[!["):
                prefix = visible(prefix)
            is_image = bool(match["image"]) if match.re is LINK else match["kind"] == "src"
            return prefix + destination(match["url"], is_image) + match[0][end - match.start():]

        for pattern in (LINK, ATTRIBUTE):
            spans = [match.span() for match in re.finditer(r"(`+)(.*?)\1", line)]
            line = pattern.sub(lambda match: replace(match, spans), line)
        return line

    lines = []
    fence = None
    in_comment = False
    for line in text.splitlines(keepends=True):
        stripped = line.strip()
        marker = re.match(r"(`{3,}|~{3,})", stripped)
        if in_comment:
            in_comment = "-->" not in line
        elif fence:
            if re.fullmatch(re.escape(fence[0]) + "{" + str(len(fence)) + ",}", stripped):
                fence = None
        elif marker:
            fence = marker[1]
        elif "<!--" in line:
            in_comment = "-->" not in line[line.index("<!--") + 4:]
        else:
            line = visible(line)
        lines.append(line)
    return "".join(lines)


def main():
    """Validate all inputs, then replace only the dedicated disposable source tree."""
    if OUTPUT.is_symlink() or OUTPUT.resolve() != ROOT / "temp/pages-source":
        raise ValueError("temp/pages-source must be a local directory, not a symlink")
    pages = {"README.md", "docs/README.md", "docs/TriceUserManual.md", "docs/TriceGuide.md", "docs/rm/README.md"}
    pages.update(path.relative_to(ROOT).as_posix() for path in (ROOT / "docs/rm").glob("*.md"))
    files = {path: read_source(path) for path in ["index.md", "_config.yml", *sorted(pages)]}
    for path in sorted(pages):
        # Byte I/O avoids newline conversion, including on Windows.
        text = rewrite(files[path].decode("utf-8"), Path(path), pages, files)
        # Jekyll also interprets Liquid inside code fences. Escape each opening
        # delimiter so documented templates survive rendering, even endraw tags.
        text = re.sub(r"{{|{%", lambda match: "{% raw %}" + match[0] + "{% endraw %}", text)
        files[path] = text.encode("utf-8")
    if OUTPUT.exists():
        shutil.rmtree(OUTPUT)
    for path, contents in sorted(files.items()):
        target = OUTPUT / path
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(contents)
    print(f"Prepared {len(pages)} Markdown sources and {len(files) - len(pages) - 2} resources in temp/pages-source")


if __name__ == "__main__":
    try:
        main()
    except (OSError, ValueError) as error:
        raise SystemExit(f"ERROR: {error}") from error

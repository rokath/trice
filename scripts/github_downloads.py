# SPDX-License-Identifier: MIT
"""Render release-download charts from paginated GitHub CLI output using only stdlib."""

import argparse
from datetime import datetime, timedelta, timezone
from html import escape
import json
from pathlib import Path
import sys


def release_snapshot(pages, observed_at):
    """Keep published release totals, deduplicating IDs repeated across API pages."""
    if not isinstance(pages, list) or any(not isinstance(page, list) for page in pages):
        raise ValueError("expected the JSON array of pages from gh api --paginate --slurp")
    releases = {}
    for page in pages:
        for release in page:
            if release.get("draft") or not release.get("published_at"):
                continue
            counts = [asset["download_count"] for asset in release["assets"]]
            if any(type(count) is not int or count < 0 for count in counts):
                raise ValueError("download_count must be a nonnegative integer")
            releases[release["id"]] = {
                "id": release["id"],
                "tag": release["tag_name"],
                "published_at": release["published_at"],
                "downloads": sum(counts),
            }
    return {
        "observed_at": observed_at,
        "releases": sorted(releases.values(), key=lambda item: item["published_at"], reverse=True),
    }


def total(snapshot):
    """Count downloads of assets that existed at the time of this observation."""
    return sum(release["downloads"] for release in snapshot["releases"])


def text(x, y, value, anchor="start"):
    """Escape labels from release metadata before inserting them into SVG markup."""
    return f'<text x="{x:.1f}" y="{y:.1f}" text-anchor="{anchor}">{escape(str(value))}</text>'


def svg(title, height, elements):
    """Produce a self-contained, downloadable chart with an accessible title."""
    return (
        f'<svg xmlns="http://www.w3.org/2000/svg" width="960" height="{height}" '
        f'viewBox="0 0 960 {height}" role="img"><title>{escape(title)}</title>'
        '<style>text{font:13px system-ui,sans-serif;fill:#303846}'
        '.grid{stroke:#e5e9ef}.series{stroke:#4c78a8;stroke-width:2;fill:none}</style>'
        f'<rect width="960" height="{height}" fill="white"/>' + "".join(elements) + "</svg>\n"
    )


def release_chart(releases):
    """Show every release in publication order; the HTML provides a scrollable view."""
    left, width = 240, 640
    height = max(110, 65 + 24 * len(releases))
    maximum = max((release["downloads"] for release in releases), default=0) or 1
    elements = []
    for tick in range(5):
        x = left + width * tick / 4
        elements.append(f'<path class="grid" d="M{x} 30V{height - 25}"/>')
        elements.append(text(x, 20, f"{maximum * tick / 4:,.0f}", "middle"))
    for index, release in enumerate(releases):
        y = 43 + index * 24
        size = width * release["downloads"] / maximum
        elements.append(text(left - 10, y + 12, release["tag"], "end"))
        elements.append(
            f'<rect x="{left}" y="{y}" width="{size:.2f}" height="16" fill="#4c78a8">'
            f'<title>{escape(release["tag"])}: {release["downloads"]:,} downloads</title></rect>'
        )
        elements.append(text(left + size + 6, y + 12, f'{release["downloads"]:,}'))
    if not releases:
        elements.append(text(left, 65, "No published releases."))
    elements.append(text(left + width / 2, height - 5, "Release-asset downloads", "middle"))
    return svg("Downloads by release, newest first", height, elements)


def history_chart(history):
    """Use UTC calendar dates and connect only consecutive observed days."""
    days = [datetime.fromisoformat(item["observed_at"].replace("Z", "+00:00")).date() for item in history]
    span = (days[-1] - days[0]).days
    maximum = max(total(item) for item in history) or 1
    elements = []
    for tick in range(5):
        y = 250 - 210 * tick / 4
        elements.append(f'<path class="grid" d="M85 {y}H920"/>')
        elements.append(text(75, y + 4, f"{maximum * tick / 4:,.0f}", "end"))
    previous = None
    for index, (day, item) in enumerate(zip(days, history)):
        x = 85 + 835 * (day - days[0]).days / span if span else 502.5
        y = 250 - 210 * total(item) / maximum
        if previous is not None and (day - days[index - 1]).days == 1:
            elements.append(f'<path class="series" d="M{previous[0]:.2f} {previous[1]:.2f}L{x:.2f} {y:.2f}"/>')
        elements.append(
            f'<circle data-date="{day}" cx="{x:.2f}" cy="{y:.2f}" r="4" fill="#4c78a8">'
            f'<title>{day}: {total(item):,} downloads</title></circle>'
        )
        previous = (x, y)
    # Calendar-spaced labels stay readable even when observations have long gaps.
    for offset in sorted({0, span // 2, span}):
        x = 85 + 835 * offset / span if span else 502.5
        elements.append(text(x, 278, days[0] + timedelta(days=offset), "middle"))
    elements.append(text(500, 305, "Observation date (UTC)", "middle"))
    return svg("Total release-asset downloads over time", 320, elements)


def report_html(history):
    """Explain exactly which counters are plotted beside the two standalone charts."""
    latest = history[-1]
    return f'''<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Trice release downloads</title>
<style>
body {{font:16px/1.5 system-ui,sans-serif;color:#303846;max-width:980px;margin:auto;padding:16px}}
.chart {{overflow:auto}} .releases {{max-height:70vh}}
img {{display:block;width:100%;height:auto;min-width:640px}}
</style>
</head>
<body>
<p><a href="../">Project statistics</a></p>
<h1>Release downloads</h1>
<p><strong>{total(latest):,} downloads</strong> across {len(latest["releases"])} published releases,
including prereleases. Last observation: {escape(latest["observed_at"])}.</p>
<p>Downloads count existing GitHub release assets, including binaries, manuals and checksums.
Repeated and automated downloads count too; this is not a count of unique users or installations.
Git clones and automatically generated source archives are excluded.</p>
<h2>Total downloads over time</h2>
<p>Daily UTC observations start on {escape(history[0]["observed_at"][:10])}. Earlier daily counts cannot be recovered.
Missing days are gaps. Deleting or replacing release assets can lower the measured total.</p>
<div class="chart"><img src="over-time.svg" alt="Observed total release-asset downloads by UTC date"></div>
<p><a href="over-time.svg" download>Download time chart (SVG)</a></p>
<h2>Downloads by release</h2>
<p>Current lifetime counters per release, newest first. Scroll to see all releases.</p>
<div class="chart releases"><img src="by-release.svg" alt="Release download totals, newest releases first"></div>
<p><a href="by-release.svg" download>Download release chart (SVG)</a></p>
<p>Source: <a href="https://docs.github.com/en/rest/releases/releases#list-releases">GitHub release API</a>.</p>
</body>
</html>
'''


def main():
    """Validate input and existing history before replacing this UTC day's snapshot."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("releases_json", type=Path, help="gh api --paginate --slurp releases output")
    parser.add_argument("data_directory", type=Path, help="download data directory in the statistics branch")
    parser.add_argument("--observed-at", help="UTC timestamp override for reproducible offline checks")
    args = parser.parse_args()
    try:
        stamp = datetime.fromisoformat(args.observed_at.replace("Z", "+00:00")) if args.observed_at else datetime.now(timezone.utc)
        if stamp.utcoffset() is None:
            raise ValueError("observation timestamp must include a timezone")
        observed_at = stamp.astimezone(timezone.utc).replace(microsecond=0).isoformat().replace("+00:00", "Z")
        current = release_snapshot(json.loads(args.releases_json.read_text(encoding="utf-8")), observed_at)
        snapshots = args.data_directory / "snapshots"
        history = {path.stem: json.loads(path.read_text(encoding="utf-8")) for path in sorted(snapshots.glob("????-??-??.json"))}
        history[observed_at[:10]] = current
        ordered = [history[day] for day in sorted(history)]
        # Render everything first; invalid input/history must not replace a good snapshot.
        artifacts = {
            "by-release.svg": release_chart(ordered[-1]["releases"]),
            "over-time.svg": history_chart(ordered),
            "index.html": report_html(ordered),
        }
        report = args.data_directory / "report"
        snapshots.mkdir(parents=True, exist_ok=True)
        report.mkdir(parents=True, exist_ok=True)
        (snapshots / f"{observed_at[:10]}.json").write_text(json.dumps(current, indent=2) + "\n", encoding="utf-8")
        for name, contents in artifacts.items():
            (report / name).write_text(contents, encoding="utf-8")
        print(f"Generated {report}/index.html: {total(ordered[-1]):,} downloads, {len(ordered)} observed days")
    except (OSError, ValueError, KeyError, TypeError, AttributeError) as error:
        parser.exit(1, f"Download report failed for {args.releases_json} / {args.data_directory}: {error}\n")


if __name__ == "__main__":
    main()

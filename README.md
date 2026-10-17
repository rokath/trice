# Generated statistics - do not merge

> **This is the `github-repo-stats` data branch. Do not merge it into other
> branches, and do not merge development branches into it.**

Read the public [Trice statistics dashboard](https://rokath.github.io/trice/stats/).
It includes traffic reports and links the release-download charts after their
first collection. These counters include automated and repeated access; they
are not a count of unique users or installations.

This branch stores generated data for `rokath/trice`:

- [Traffic and star/fork data](rokath/trice/ghrs-data/).
- [Latest report](rokath/trice/latest-report/report.md).
- Daily release-download snapshots and charts under `rokath/trice/downloads/`
  after the first download collection.

The **Repository statistics** workflow runs from `main`, collects data and
appends ordinary commits here. **Deploy GitHub Pages** publishes the reports.
Collector scripts, application sources, manuals and build workflows belong on
development branches. The only workflow retained here is the small
**Statistics branch guard**, which rejects pull requests involving this branch
without checking out or executing pull-request code.

Keep the archived statistics and their history. Do not reset, force-push or
delete this branch. Its ahead/behind count does not mean that it needs to be
updated from `main`. The root README is maintained separately from the generated
`rokath/trice/README.md` and is not overwritten by the report generator.

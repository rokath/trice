# User manual handover

## Goal and stopping point

Continue the Trice User Manual review and first-instrumentation examples when the user gives the next instruction. The latest task established a library formatting guard. Implementation is stopped; do not resume automatically on `handson usermanual`.

## Verified checkout

- Branch: `wip`.
- HEAD: `4a2d745490d43bd553c6ce606713532c9e03fb94` (`style: centralize library formatting protection`).
- Working tree was clean before creating this handover. This handover is the only new uncommitted file.
- `examples/F030_inst_rtt` and `examples/F030_inst_uart`, their TIL dictionaries, and the UM links are committed. Each example uses the same two startup records and simple portable build/log scripts. Both derive from `F030_bare`; no `TRICE_FLAGS` or RPC support was added.
- `src/.clang-format` contains `BasedOnStyle: InheritParentConfig` and `DisableFormat: true`. Set the latter to `false` to enable library formatting with the parent style.
- The C formatter now uses `--style=file`, honoring the local library guard. Seven inline `clang-format off/on` markers were removed from `trice.h`, `triceOn.h`, `triceOff.h`, and `triceDoubleBuffer.c`.

## Binding decisions

- Preserve `F030_inst`; the user requested separate RTT and UART examples. Preserve vendor files outside `Core` byte-for-byte.
- Explain instrumentation primarily through folder comparisons with `F030_bare`. Mention that the existing `F030_inst` logs through RTT and UART in parallel.
- UM links should display meaningful file paths and allow scripts to be opened directly. Tables omit outer pipes.
- Avoid suggesting that firmware/library sources must match the host-tool checkout. Accumulated `til.json` can decode older firmware; source locations in `li.json` must match the relevant build.
- Version-control `til.json` and `triceConfig.h`; normally exclude `li.json` and regenerable sidecars. Bind creates missing `-genDir` parents; CLI help documents this. Mention tlog `-s` and `-debug` for diagnostics.
- Only explicit requests authorize commits or pushes. Any content edit after a commit request needs user review before committing, unless explicitly waived. Handover creation does not authorize staging, committing, or pushing.

## Validation already performed

- `examples/F030_inst_rtt/build.sh -j2` and `examples/F030_inst_uart/build.sh -j2`: ARM cross-builds passed. Hardware tests were not performed.
- Folder comparison: all 45 vendor files in each new example were byte-identical to `F030_bare`. Among existing Core files, RTT differs only in `main.c`; UART additionally differs in `stm32f0xx_it.c`. New configuration headers are intentional additions.
- `rtk go test ./scripts -run 'TestClangFormat(FileSelection|Batch)$' -count=1`: last run passed; RTK reported 7 tests. The current checkout contains `TestClangFormatBatch`, but no longer contains `TestClangFormatFileSelection`.
- `./scripts/_280_format_c_code.sh check`: last run passed after switching to local configuration lookup.
- Native clang-format 23.1.2 checks: all 55 library C/H files remained byte-identical with `DisableFormat: true`. A temporary enabled fixture inherited the root style and formatted an intentionally unformatted probe as expected.
- Earlier memory-only checks with `ReflowComments: Always`: `trice.h` converged after two passes; `triceDoubleBuffer.c` alternated its closing-comment indentation on successive passes. No root formatting output was written to library sources.

## Current differences and open issues

- Current committed root `.clang-format` has `ReflowComments: Never`, unlike the earlier `Always` experiment. Do not describe those earlier stability tests as tests of the current setting.
- Current `_280_format_c_code.sh` uses plain `git ls-files`: the earlier change including non-ignored untracked sources is absent. Both new examples are now tracked, so they are selected. The user previously requested untracked-file coverage; clarify whether its removal was intentional before restoring it.
- The `Always` flipflop remains a known issue for explicitly enabled library formatting. Default library protection avoids it.
- Optional local artifacts under `demo/deferred/build` include host tools, ARM build logs and temporary formatter fixtures. They are not required for transfer and must not be committed.

## Next steps

On `handson usermanual`, compare branch, HEAD, working tree and the files above, report discrepancies, then wait for the user's next instruction. Resume UM review only when requested. Before changing formatting behavior, resolve the current `Never` setting and tracked-only selection against the user's intent.

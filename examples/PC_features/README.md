# PC feature tour

This small PC program emits a short binary capture for the normal Trice host
decoder. It demonstrates named fields, a runtime string, Context Enrichment,
tags and an untagged event, a buffer record, and two target timestamp widths.
The C source is short enough to change and rerun while comparing outputs.

| Feature | Try in `main.c` | Observe with |
| --- | --- | --- |
| Named fields and a runtime string | `emit_sample` and `TriceS` | `./show_json.sh` |
| CE without changing the call | `info:ctx:` in `emit_sample` | `cycle` in JSON or KV |
| Tags, priorities, and custom `sensor:` | `wrn:`, `sensor:`, `dbg:` | `./show_json.sh -logLevel wrn` |
| Two stamp widths and a delta | `TriceS`/`Trice8` and `TRice16`/`TRice32` | `ts16`, `ts32`, `ts32Delta` |
| Buffer and untagged message | End of `main` | `./show_json.sh` |

The larger [TriceCheck corpus](../../_test/testdata/triceCheck.c) covers macro
and format variants. [PC_log](../PC_log/README.md) and
[G0B1_log](../G0B1_log/README.md) cover target-side local logging;
[DemoData_Trice](../DemoData_Trice/README.md) covers live plot data. This tour
keeps the host-decoder features together in a small capture.

From this directory, with `trice` and a C compiler in `PATH`:

```sh
./build_and_run.sh
./show_text.sh
./show_json.sh
./show_kv.sh
```

Run `./check_output.sh` to rebuild and check the decoded fields, timestamps,
tag filtering, and KV output automatically.

## Changing the tour

`check_output.sh` checks specific values from the current C calls and decoder
options. After editing `main.c`, the CE rule in `build_and_run.sh`, or a
`show_*.sh` command, run `./build_and_run.sh` and inspect `./show_json.sh` and
`./show_kv.sh`. Then update the matching `case` pattern in `check_output.sh`
and rerun `./check_output.sh`. Keep each check tied to an observable result;
if you remove a demonstrated feature, replace or remove its check deliberately
rather than commenting it out and leaving a misleading `PASS` message.

Macro capitalization selects the stamp width: `trice...` has no target stamp,
`Trice...` has 16 bits, and `TRice...` has 32 bits. For example, changing
`Trice16("info:Phase ...")` to `TRice16("info:Phase ...")` moves that event from
`ts16` to `ts32`; it does not change the 16-bit width of its `phase` value.
The `-ts16` and `-ts32` options in the show scripts control display, not which
stamp the target sends. Adding a stamped event can also change the next
`ts16Delta` or `ts32Delta`. JSON escapes a source newline as the two visible
characters `\n`, which the shell patterns match literally.

The build binds local IDs and applies `-ce` to `ctx:` calls. The source call
`Supply {voltage_mv:%u}` gains the current `pc_sample_phase` as the named
`cycle` field without changing the call in `main.c`. The runtime device name
uses `TriceS`, because CE does not add runtime arguments to string Trices.
The capture, generated headers, and executable are local build artifacts;
`til.json` and `li.json` are versioned ID tables updated by the bind step.
Rebuild after editing the source or the CE rule.

Compare `./show_json.sh` and `./show_json.sh -logLevel wrn`. The first shows
each event as one JSON object; the second retains Warning and higher weighted
groups. `./show_text.sh -pick info` selects just the Info group. The `sensor:`
record is a user label with weight 450; try
`./show_json.sh -ulabel sensor:650 -logLevel wrn` to include it alongside the
Warning event. `./show_text.sh -tagStat` prints counts for every decoded tag.
The 16-bit stamp represents a sample phase; the 32-bit stamp is milliseconds,
so the second supply reading also shows a 25 ms delta. Options passed to the
show scripts are appended to their `trice log` command.

The G0B1 companion is [G0B1_features](../G0B1_features/README.md).

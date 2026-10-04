# G0B1 feature tour

This is a direct copy of [G0B1_inst](../G0B1_inst/ReadMe.md), with a short
logging tour added to the two existing FreeRTOS tasks. Its original hardware
configuration remains in place. The original project's large `TriceCheck`
loop is omitted so the two task records are easy to find. The companion
[PC feature tour](../PC_features/README.md) runs without a board.

| Feature | Try in `Core/Src/main.c` | Observe with |
| --- | --- | --- |
| One CE call site in two tasks | `LogFeatureSample` and both task entries | Different `task` fields in `./show_json.sh` |
| Named fields and runtime string | `sample`, `load_pct`, and `triceS` | `./show_json.sh` or `./show_kv.sh` |
| Tags, timestamps, and buffer | `Trice16`, `TRice32`, `sensor:`, `TRICE8_B` | Any show script |

With `trice` and `arm-none-eabi-gcc` in `PATH`, build from this directory:

```sh
./demo_build.sh
```

`./check_build.sh` also checks that the generated table and adapter contain
the task, string, and user-tag examples. It needs no connected board.

The script binds this copy and its shared producers to `til.json` and adds
`-ce 'ctx:", task={task:%p}", osThreadGetId()'`. The call in
`LogFeatureSample` runs once from each task. Its `task` field is the current
FreeRTOS task handle: the two records should have different values although
they come from the same C call site. The `sample` field and text remain easy
to recognize. The adjacent `triceS` call transports a runtime worker name;
CE does not add another runtime value to a string Trice. The same function
also shows 16- and 32-bit stamps, a Warning, an untagged message, and a short
buffer record. A `sensor:` event demonstrates a user-defined tag with weight
450 in the show scripts.

Flash `out.gcc/G0B1.elf` using the board procedure from the original project.
Capture RTT channel 0 with a J-Link in a separate terminal:

```sh
mkdir -p temp
JLinkRTTLogger -Device STM32G0B1RE -If SWD -Speed 4000 -RTTChannel 0 temp/trice.bin
```

Stop the logger after the startup records arrive. Decode the completed capture:

```sh
./show_text.sh
./show_json.sh
./show_kv.sh
```

JSON is one object per event (NDJSON). Try `./show_json.sh -pick info` to see
the two task events and `./show_kv.sh -logLevel wrn` to retain Warning and
higher weighted tags. Each show script accepts additional `trice log` options.
The scripts expect this project's `til.json`, so rebuild and recapture after
changing the demo calls or CE rule. A live board and J-Link are needed only
for capturing; the scripts can decode an already saved `temp/trice.bin`.

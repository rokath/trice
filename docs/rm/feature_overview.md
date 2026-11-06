# Trice Features at a Glance

Find a capability, then follow its link for examples, syntax and limits. Rows run from newest to oldest. **Version** is the first stable release with the listed capability; features introduced during the v2 betas are grouped under 2.0.0. **1.0 or earlier** means the feature was already present in 1.0, not that it first appeared there.

**Obsolete** marks features to avoid in new projects. The row states whether they remain available. For removed option names and other upgrade changes, follow the [migration guide](id_management.md#migrating-1x-scripts-and-projects). For changes between releases, use the [Changelog](../../CHANGELOG.md).

Version        | Feature                                                                                                       | Use and current status
-------------- | ------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------
2.0.0          | [Bind](bind.md)                                                                                               | Keep log calls ID-free; generate their IDs in compiler headers.
2.0.0          | [Structured Logging](structured_logging.md)                                                                   | Name values such as `{voltage_mv:%u}`; output text, JSON or key/value records.
2.0.0          | [Context Enrichment](context_enrichment.md)                                                                   | Add selected context values automatically with Bind or Insert.
2.0.0          | [Weighted tags and log levels](tags_color_and_weights.md)                                                     | Filter by priority; configure weights, colors and display palettes.
2.0.0          | [Consistent alias tags](legacy_user_code.md#alias-identity-and-tags-in-v2)                                    | Keep macro names in dictionaries and use the same event tag across output formats.
2.0.0          | [Stamp expressions](stamp_expressions.md)                                                                     | Convert hardware counters on the host with `printf`, `unwrap` and `delta`.
2.0.0          | [Visualization output](visualization.md)                                                                      | Export selected numeric values as CSV or UDP data, for example to LabPlot.
2.0.0          | [Local deferred text logging](target_implementation.md#local-deferred-text-log)                               | Format buffered records on the target without a host decoder.
2.0.0          | [Source-selected C tables and dictionary reports](generate.md)                                                | Generate C metadata for selected sources, readable dictionaries and Bind reports.
2.0.0          | [Deterministic ID assignment](id_management.md#different-ids-for-same-trices)                                 | Reproduce IDs for unchanged source selections and ID pools; changed sites can change IDs.
2.0.0          | [User and Reference Manuals](../TriceGuide.md)                                                                | Read the guided introduction or full reference as Markdown, PDF or EPUB 3.
1.3.0          | [Asynchronous Broadcast Commands (ABC)](abc_asynchronous_broadcast_commands.md)                               | Send commands between nodes on a shared bus; generate receive tables with `-abc`.
1.2.4          | [Counted typeX0 user packets](typex0_user_packets.md)                                                         | Carry application data alongside logs without a Trice ID or dictionary entry.
1.2.3          | [Standalone `tlog`](cli.md#common-information)                                                                | Run `tlog ...` as the short equivalent of `trice log ...`.
1.2.2          | [Linux packages and Homebrew installation](tool_build_release.md)                                             | Install packaged host tools; the Homebrew tap follows stable releases.
1.2.0          | [Target-stamp delta columns](stamps.md#target-timestamp-delta-columns)                                        | Display differences between successive target stamps.
1.2.0          | [Additional compiler and integer-width support](integration_notes.md)                                         | Compile as C++ or for targets with 16-bit integers or no 64-bit integer type.
1.1.0          | [Macro aliases](legacy_user_code.md)                                                                          | Integrate existing logging macro names with Trice.
1.1.0          | [User-defined tags](tags_color_and_weights.md#user-defined-tags-weights-and-colors)                           | Add application tags without rebuilding the host tool; v2 uses `-utag`.
1.1.0          | [UDP input](cli.md#udp4-input)                                                                                | Receive binary logs from a device over IPv4 UDP.
1.1.0          | [Epoch timestamp display](stamps.md#target-timestamps-formatting)                                             | Display a 32-bit seconds counter as UTC time with `-ts32 epoch`.
1.0 or earlier | [ID-based binary logging](overview.md#how-it-works)                                                           | Send an ID and values; the host supplies the format string from the dictionary.
1.0 or earlier | [Printf-like values](printf_comparison.md)                                                                    | Log integers, floating-point values, runtime strings and buffers.
1.0 or earlier | [Direct and deferred output](overview.md#direct-and-deferred-output)                                          | Send records immediately or collect them in ring/double buffers for later transfer.
1.0 or earlier | [UART and custom byte transports](custom_output.md)                                                           | Use UART or supply a writer for another byte-capable connection or storage.
1.0 or earlier | [SEGGER RTT](rtt.md)                                                                                          | Read target logs through a debug probe without a dedicated UART.
1.0 or earlier | [COBS/TCOBS framing](binary_encoding.md#framing---none-or-with-cobs-or-tcobs-encoding)                        | Separate binary packets and recover packet boundaries after an interrupted stream.
1.0 or earlier | [XTEA encryption](binary_encoding.md#optional-xtea-encryption)                                                | Encrypt binary log data before transfer.
1.0 or earlier | [Target and host timestamps](stamps.md)                                                                       | Show target event timing and host receipt time.
1.0 or earlier | [Source locations](cli.md#location-information)                                                               | Display the source file and line recorded in the location dictionary.
1.0 or earlier | [Tag colors and selection](tags_color_and_weights.md)                                                         | Color log messages and select or exclude tags on the host.
1.0 or earlier | [Compile-time disabling](target_configuration.md#target-side-compile-time--trice-on-off)                      | Remove logging code for a file or build with `TRICE_OFF`.
1.0 or earlier | [Target routing](target_routing.md#id-routing)                                                                | Route selected ID ranges to configured output channels.
1.0 or earlier | [Buffer protection and diagnostics](target_configuration.md#buffer-protection-and-diagnostics)                | Monitor overflow and buffer usage when sizing the logging path.
1.0 or earlier | [Automatic dictionary reload](cli.md#automatic-dictionary-reload)                                             | Use updated dictionaries without restarting the host logger.
1.0 or earlier | [Text log files](cli.md#logfile-output)                                                                       | Save decoded output for later inspection.
1.0 or earlier | [Binary capture and replay](cli.md#binary-logfile)                                                            | Record raw bytes and decode them later with matching dictionaries.
1.0 or earlier | [TCP input and output](cli.md#tcp4-input)                                                                     | Receive binary logs via TCP or forward decoded output to another program.
1.0 or earlier | [Display server and multiple targets](cli.md#logging-over-a-display-server)                                   | Combine log output from separate receivers in one display.
1.0 or earlier | [UART command shell](cli.md#stimulate-target-with-a-user-command-over-uart)                                   | Send application-defined commands while reading logs.
1.0 or earlier | [Text and raw-byte monitors](cli.md#text-and-raw-byte-monitors)                                               | Inspect a stream with CHAR or DUMP before decoding Trice records.
1.0 or earlier | [Cross-platform host tools](project_resources.md#download-and-install-trice)                                  | Run the host tools on Linux, macOS or Windows.
1.0 or earlier | [Insert/Clean and source cache](insert_clean_cache.md)                                                        | **Obsolete (deprecated, still available).** Writes IDs into source calls; prefer Bind for new projects.
1.0 or earlier | [Legacy RPC](target_implementation.md#obsolete-rpc-support)                                                   | **Obsolete.** Compatibility code remains; use ABC for new integrations.
1.0 or earlier | [Automatic unframed alignment (`-pf=none`)](binary_encoding.md#framing---none-or-with-cobs-or-tcobs-encoding) | **Obsolete (deprecated).** Select explicit alignment such as `none8` or `none32` for unencrypted streams.

Keep this table current when a release adds a capability: add a linked row at its introduction version, newest first. Update an existing row when its status changes; retain obsolete entries with their replacement. Record fixes and individual option changes in the Changelog.


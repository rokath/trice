# ID reference list til.json

<details markdown="1">
<summary>Contents</summary>

---

<!-- mdtoc -->

* [1. Compatibility with firmware and host-tool versions](#compatibility-with-firmware-and-host-tool-versions)
* [2. Migrating 1.x scripts and projects](#migrating-1x-scripts-and-projects)
* [3. til.json Version control](#tiljson-version-control)
* [4. Long Time Availability](#long-time-availability)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

---

</details>

* The `trice insert` command demands a **til.json** file - it will not work without it. That is a safety feature to avoid unwanted file generations. If you are sure to create a new **til.json** file, create an empty one: `touch til.json`.
* The name **til.json** is a default one. With the command line parameter `-i` you can use any filename.
* It is possible to use several **til.json** files - for example one for each target project but it is easier to maintain only one **til.json** file for all projects.
* The ID reference list keeps obsolete IDs with their format strings. Decoding former firmware versions also requires a compatible host template parser, as described below.
* One can delete the ID reference list when IDs inside the code. It will be reconstructed automatically from the source tree with the next `trice clean` command, but history is lost then.
* Keeping obsolete IDs makes it more comfortable during development to deal with different firmware variants at the same time.

## 1. <a id="compatibility-with-firmware-and-host-tool-versions"></a>Compatibility with firmware and host-tool versions

Keep each released firmware together with its `til.json`, matching `li.json` if needed, host-tool version, and decoding options such as framing, byte order, and default value width. Retaining an ID preserves its dictionary entry; it does not make every dictionary compatible with every host version. Location information must describe the firmware actually running, even when one cumulative TIL covers multiple firmware versions.

Firmware and dictionary | Host tool | Supported use
--- | --- | ---
v1.3.0 firmware with its unchanged dictionaries | v1.3.0 | Reproduce historical output with the corresponding original options.
Existing firmware with classic printf formats and retained IDs | Current host | Supported when the formats are valid under the current template syntax and transport settings match. Literal braces are a relevant exception; see the examples below.
Current firmware and current dictionaries, including structured fields or CE | Current host | Supported within the documented record families and CE limits. Current instrumentation and its matching target sources are required when building these features.
A dictionary containing current named fields or doubled literal braces | v1.3.0 | Not a supported interpretation of the new templates. The old host does not understand fields and prints doubled braces literally.
C headers, implementation files, or generated Bind sidecars mixed from different releases | Any host | No general source/build compatibility guarantee. Use target files from one release and regenerate sidecars with the corresponding host tool.

The following examples use the same unstamped TREX record layout and ID in both host versions. `Strg` is the stored dictionary string; the scalar case carries the value `7`.

Stored `Strg` | Record payload | v1.3.0 interpretation | Current interpretation
--- | --- | --- | ---
`msg:count=%d` | One 32-bit value | `count=7` | `count=7`
`hi` | No values | `hi` | `hi`; JSON/KV classify it as `untagged` without inserting a tag into `message`.
`msg:literal={x}` | No values | `literal={x}` | `{x}` requires a value; the record is rejected with a diagnostic.
`msg:set={1,2}` | No values | `set={1,2}` | Invalid field name; the record is rejected with a diagnostic.
`msg:literal={{x}}` | No values | `literal={{x}}` | `literal={x}`
`msg:value={x}` | One 32-bit value | Not a supported one-value printf format | `value=7`, with field `x=7` in structured output.

For unchanged historical firmware that used literal braces, replay with its archived dictionary and matching old host tool. In sources built with the current tools, write literal braces as `{{` and `}}`, then regenerate IDs/dictionaries and rebuild. Logging does not rewrite supplied dictionaries, and historical dictionaries are not automatically converted. An unchanged binary record layout alone does not establish template compatibility. Replay checks must inspect output and diagnostics: a rejected record does not necessarily make the logger exit with a nonzero status.

## 2. <a id="migrating-1x-scripts-and-projects"></a>Migrating 1.x scripts and projects

Review scripts written for 1.x before using the v2 tools. These changes are relative to v1.3.0 and apply to both `trice log` and `tlog` where relevant:

Previous usage | Required adjustment
--- | ---
`trice generate -tilC` | Use `trice generate -src Core -logC`, selecting your own sources. Run Insert or Bind first. Only currently referenced IDs are emitted, not the whole historical TIL. The default output is now `generated/til.c`; use `-logC=build/til.c` to choose a path.
`trice generate -abc deviceX` followed by compiling `deviceX.c` | A name without a directory now produces `generated/deviceX.h` and `.c`. Compile the file from its new location, select another directory with `-genDir`, or specify the target path, for example `-abc=custom/deviceX`. ABC itself was already included in v1.3.0.
ID management with `-liPath` | Use `-liRoot <project-root>` to choose the directory that stored paths are relative to. The default is the directory containing `li.json`. Its `File` field now contains a source path, for example `Core/Src/main.c`, rather than only `main.c`; adjust scripts that read it.
Logging with `-liPath` | Remove it. Use `-liMaxDirs N` to show up to N parent directories; the default `0` shows only the filename.
`-pick err:wrn` or `-ban dbg:trace` | Repeat the switch: `-pick err -pick wrn` or `-ban dbg -ban trace`. Colon lists are rejected; `-pick` and `-ban` cannot be combined. `-pick 600` selects only weight 600; use `-logLevel 600` for weight 600 and above.
`-ulabel alpha:beta` to register two tags | Use `-ulabel alpha -ulabel beta`. A colon now assigns one tag's weight or color, for example `-ulabel alpha:650`. User-defined tags match exact spelling.
Built-in one-letter tags or selectors, or `OK` as a Message alias | Use descriptive names such as `err`, `wrn`, `inf`, `dbg` and `msg`, including in `-IDRange`. Built-in names ignore case. The old aliases are removed; unregistered prefixes count as `untagged`, and unknown selectors are errors. `rx` now selects Receive and `tx` selects Transmit, without the former overlap with Read/Write.
`-logLevel` based on the old tag-table order | Recheck which messages pass: selection now uses a minimum weight. `-logLevel wrn` includes Warning and messages with higher priority; `-loglevel 600` is equivalent with default weights. Messages without a recognized tag are also filtered. Tags used for timestamps or other output columns do not decide whether a message is shown.

Further changes can affect builds and automated checks:

- **Literal braces and replay:** use `{{` and `}}` in new source formats. For old firmware and dictionaries, follow the [compatibility rules](#compatibility-with-firmware-and-host-tool-versions) above. Replay can reject a record and still exit successfully; scripts must also check diagnostics and expected records.
- **ID ranges and identical calls:** Insert/Bind can change existing IDs when their allowed range changes or identical calls are added, removed or moved. Rebuild the firmware and keep its matching `li.json`; see [ID assignment and ranges](id_management.md#different-ids-for-same-trices).
- **Location information:** keep `li.json` between ID-management runs, including partial scans. To hide locations during logging, use `trice log -li off`; passing `-li off` to Insert/Bind also disables source ownership information. See [location information](cli.md#location-information).
- **RTT builds:** put project and generated header directories first, then `src`, then `src/default_conf` on the include path. Update the three SEGGER source/header files together and retain the license and your project-specific configuration; see [RTT integration](rtt.md).
- **Custom encoding calls:** if `TriceEncode()` uses a framing format not selected by an active output, enable `TRICE_COBS_ENCODE_SUPPORT` or `TRICE_TCOBS_ENCODE_SUPPORT` and link that encoder. Unavailable framing returns `0`; see [encoder selection](code_size.md).
- **Output parsing:** statistics include messages hidden by host filters. For JSON/KV, read application records from standard output and diagnostics from standard error; see [Structured Logging](structured_logging.md) and [event statistics](tags_color_and_weights.md#event-statistics).
- **Repository automation:** helper/test scripts use numbered filenames, for example `scripts/_530_test_links.sh` replaces `_testAll_06_LinkCheck.sh`. Update hard-coded helper and documentation paths. `./scripts/testAll.sh` defaults to the quick Bind workflow; use `full` for broad Insert/Clean coverage and `--stop` for early exit. See [target tests](target_tests.md).
- **Installation:** source builds require Go 1.25 or later. Install v2 from its release archive or build a checkout of its tag; remote `go install` with a v2 version tag is not supported because the module path has no `/v2` suffix. Betas do not update the stable Homebrew tap; see [beta installation](tool_build_release.md#install-a-beta-or-a-stable-release).

## 3. <a id="tiljson-version-control"></a>til.json Version control

* The ID list should go into the version control repository of your project.
* To keep it clean from the daily development garbage one could `git restore til.json`, and re-build just before check-in.

```diff
--> Deleting til.json should not not be done when the sources are without IDs. 
--> That would result in a loss of the complete ID history and a assignment of a complete new set of IDs.
```

You could write a small bash script similar to this (untested):

```bash
trice insert -cache # Insert the IDs into the source code.
git restore til.json # Forget the todays garbage.

# Add the todays IDs to the restored til.json and clean the code.
# We have to deactivate the cache to force the file processing to get the new IDs into til.json.
trice clean # Remove the IDs from the source code with deactivated cache.  
```

## 4. <a id="long-time-availability"></a>Long Time Availability

* You could place a download link for the Trice tool and the used **til.json** list.
* Optionally add the (compressed/encrypted) ID reference list as resource into the target FLASH memory to be sure not to loose it in the next 200 years.

<p align="right">(<a href="#id-reference-list-tiljson">back to top</a>)</p>


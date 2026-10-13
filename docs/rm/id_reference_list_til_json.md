# ID reference list til.json

<details markdown="1">
<summary>Contents</summary>

<!-- mdtoc -->

* [1. Compatibility with firmware and host-tool versions](#compatibility-with-firmware-and-host-tool-versions)
* [2. til.json Version control](#tiljson-version-control)
* [3. Long Time Availability](#long-time-availability)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>

* The `trice insert` command demands a **til.json** file - it will not work without it. That is a safety feature to avoid unwanted file generations. If you are sure to create a new **til.json** file, create an empty one: `touch til.json`.
* The name **til.json** is a default one. With the command line parameter `-i` you can use any filename.
* It is possible to use several **til.json** files - for example one for each target project but it is easier to maintain only one **til.json** file for all projects.
* The ID reference list keeps obsolete IDs with their format strings. Decoding former firmware versions also requires a compatible host template parser, as described below.
* One can delete the ID reference list when IDs inside the code. It will be reconstructed automatically from the source tree with the next `trice clean` command, but history is lost then.
* Keeping obsolete IDs makes it more comfortable during development to deal with different firmware variants at the same time.

## 1. <a id="compatibility-with-firmware-and-host-tool-versions"></a>Compatibility with firmware and host-tool versions

Keep each released firmware together with its `til.json`, matching `li.json` if needed, host-tool version, and decoding options such as framing, byte order, and default value width. Retaining an ID preserves its dictionary entry; it does not make every dictionary compatible with every host version. Location information must describe the firmware actually running, even when one cumulative TIL covers multiple firmware versions.

| Firmware and dictionary | Host tool | Supported use |
| --- | --- | --- |
| v1.3.0 firmware with its unchanged dictionaries | v1.3.0 | Reproduce historical output with the corresponding original options. |
| Existing firmware with classic printf formats and retained IDs | Current host | Supported when the formats are valid under the current template syntax and transport settings match. Literal braces are a relevant exception; see the examples below. |
| Current firmware and current dictionaries, including structured fields or CE | Current host | Supported within the documented record families and CE limits. Current instrumentation and its matching target sources are required when building these features. |
| A dictionary containing current named fields or doubled literal braces | v1.3.0 | Not a supported interpretation of the new templates. The old host does not understand fields and prints doubled braces literally. |
| C headers, implementation files, or generated Bind sidecars mixed from different releases | Any host | No general source/build compatibility guarantee. Use target files from one release and regenerate sidecars with the corresponding host tool. |

The following examples use the same unstamped TREX record layout and ID in both host versions. `Strg` is the stored dictionary string; the scalar case carries the value `7`.

| Stored `Strg` | Record payload | v1.3.0 interpretation | Current interpretation |
| --- | --- | --- | --- |
| `msg:count=%d` | One 32-bit value | `count=7` | `count=7` |
| `hi` | No values | `hi` | `hi`; JSON/KV classify it as `untagged` without inserting a tag into `message`. |
| `msg:literal={x}` | No values | `literal={x}` | `{x}` requires a value; the record is rejected with a diagnostic. |
| `msg:set={1,2}` | No values | `set={1,2}` | Invalid field name; the record is rejected with a diagnostic. |
| `msg:literal={{x}}` | No values | `literal={{x}}` | `literal={x}` |
| `msg:value={x}` | One 32-bit value | Not a supported one-value printf format | `value=7`, with field `x=7` in structured output. |

For unchanged historical firmware that used literal braces, replay with its archived dictionary and matching old host tool. In sources built with the current tools, write literal braces as `{{` and `}}`, then regenerate IDs/dictionaries and rebuild. Logging does not rewrite supplied dictionaries, and historical dictionaries are not automatically converted. An unchanged binary record layout alone does not establish template compatibility. Replay checks must inspect output and diagnostics: a rejected record does not necessarily make the logger exit with a nonzero status.

Published command-line changes relative to v1.3.0 also matter when updating scripts:

| v1.3.0 usage | Current contract |
| --- | --- |
| `trice generate -tilC` | Use `-logC`. This is a source-based generator for current sites already resolved by Insert or Bind, not a spelling alias for dumping every historical TIL entry. Its default output is `generated/til.c`. |
| `-liPath` | Use `-liRoot` during ID management for stored source paths; use `-liMaxDirs` during logging for displayed parent directories. |
| `-ulabel alpha:beta` for two user tags | Use `-ulabel alpha -ulabel beta`. A colon now specifies a weight or color for one tag. |
| Tag selection using `-pick`, `-ban`, or `-logLevel` | Repeat `-pick`/`-ban` once per tag group or exact integer weight; colon-separated lists are rejected. Built-in aliases ignore case and select their whole group; built-in one-letter aliases are removed. `-logLevel`/`-loglevel` selects a minimum weight. Invalid selectors are rejected before opening the input. Untagged events participate in filtering. Metadata no longer determines application selection. |
| `trice generate -abc deviceX` | Bare names produce `generated/deviceX.h` and `.c`. Use `-genDir` to select the directory or an explicit target path to keep a chosen location. |

## 2. <a id="tiljson-version-control"></a>til.json Version control

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

## 3. <a id="long-time-availability"></a>Long Time Availability

* You could place a download link for the Trice tool and the used **til.json** list.
* Optionally add the (compressed/encrypted) ID reference list as resource into the target FLASH memory to be sure not to loose it in the next 200 years.

<p align="right">(<a href="#id-reference-list-tiljson">back to top</a>)</p>


# Future Design Ideas

These historical proposals preserve design work for later discussion. They are not release commitments or instructions for current tools. In particular, the proposed dictionary schema called "TIL V2" is independent of the Trice v2.0.0 release number. Serial Studio integration remains deferred in favor of other tools.

<details markdown="1">
<summary>Contents</summary>

<!-- mdtoc -->

* [1. A self-describing dictionary](#a-self-describing-dictionary)
  * [1.1. Profiles and past stream settings](#profiles-and-past-stream-settings)
  * [1.2. Fixed protocol tables and reference vectors](#fixed-protocol-tables-and-reference-vectors)
  * [1.3. Differences between the dictionary drafts](#differences-between-the-dictionary-drafts)
* [2. Typed streams for dashboard tools](#typed-streams-for-dashboard-tools)
* [3. A small C host decoder](#a-small-c-host-decoder)
* [4. Source snapshots](#source-snapshots)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>

## 1. <a id="a-self-describing-dictionary"></a>A self-describing dictionary

The July 2026 drafts proposed keeping the information needed to decode a saved binary stream in its matching dictionary. Today, use the documented [dictionary and firmware compatibility rules](id_management.md#compatibility-with-firmware-and-host-tool-versions); the schema below is a proposal.

| Proposed part | Purpose |
| --- | --- |
| `SchemaVersion` | Identify the dictionary format explicitly and reject unknown versions without partial interpretation. |
| `Profiles` | Name stream configurations, including a required `default`; profiles change decoder settings, not the ID list. |
| `StreamFormat` | Store framing, single/multi packet mode, doubled 16-bit IDs, byte order, default value width, encryption settings, stamp formats and typeX0 handling. |
| `Trices` | Hold the existing ID-to-`Type`/`Strg` entries, with room for later stable field names, units or data types. |

Device connections, baud rates, ports, input paths, display filters, colors and location files would remain separate. Framing algorithms and the fixed record layout would belong to shared protocol definitions. This separates "how these bytes were encoded" from "where to read them and how to display them".

### 1.1. <a id="profiles-and-past-stream-settings"></a>Profiles and past stream settings

Each parameter would contain its own profile values and history. For example, this draft fragment records a change from COBS to TCOBSv1:

```json
{"Current": "TCOBSv1",
 "HistoryUntil": {"2026-06-01T00:00:00Z": "COBS"}}
```

Before that UTC timestamp the effective value is COBS; exactly at the boundary and afterwards it is TCOBSv1. With multiple history entries, choose the earliest boundary strictly later than the requested time; otherwise use `Current`.

Missing non-default profile values would inherit the default at the selected time; historical `null` would select that inheritance. The default itself requires concrete values. Draft-only selectors `-profile` and `-stream-date` would choose a profile and past time. An undated capture cannot supply that time itself.

The newer draft proposed leaving existing dictionaries in their old format and creating new ones in the versioned format, without automatic conversion. Explicit CLI changes would be validated and confirmed parameter by parameter, then stored together with one change timestamp. Rejection or noninteractive disagreement would leave the file unchanged; selecting a past state would be read-only. These persistence rules remain proposals, not changes to the current CLI.

The draft allowed optional passwords, keys and their history. It required matching password/key pairs, clear failure without a usable key for encrypted data, and no recreation of deliberately removed secrets. Dictionary storage versus separate secret input remains a decision.

### 1.2. <a id="fixed-protocol-tables-and-reference-vectors"></a>Fixed protocol tables and reference vectors

The proposed `specs/` material separated fixed TREX record fields, a complete Trice type table and counted-buffer layout from project settings. A type entry could explicitly describe kind, width and parameter count, or refer to the configured default width. Build tools could generate the table; a decoder would consume explicit entries without implementing a template language. COBS, TCOBS and XTEA would remain algorithms in code, checked with shared reference vectors.

The proposed validation included known schema/type names, complete defaults, valid parameter combinations, UTC history keys and consistent encryption metadata. Errors should identify the parameter, profile and relevant history boundary.

Proposed acceptance cases mixed ts0/ts16/ts32 records, different value widths and typeX0 packets in each COBS/TCOBS single/multi configuration. Further cases covered byte order, doubled IDs, default width, inheritance and exact history boundaries. A future implementation would need to reconcile Go/C behavior before declaring a shared type table authoritative.

### 1.3. <a id="differences-between-the-dictionary-drafts"></a>Differences between the dictionary drafts

The older draft and revision 0.2 disagree on some details. Preserve these as decisions to revisit rather than combining them into one apparent specification:

| Question | Older draft | Revision 0.2 |
| --- | --- | --- |
| Profile-name comparison | Case-sensitive. | Case-insensitive, retaining the declared spelling. |
| Values for an undeclared profile | Consistency error. | Ignore them when reading and omit them on the next write. |
| Explicit value equal to an inherited default | Pin an independent profile value, recording prior inheritance. | The update procedure specifies no change when the effective value is equal. |

## 2. <a id="typed-streams-for-dashboard-tools"></a>Typed streams for dashboard tools

The deferred Trice-to-Serial-Studio (TSS) sketch proposed a host adapter alongside normal text logging. Its reusable idea is to export typed values from decoded records, using ID and argument position, instead of parsing already formatted console text.

A mapping could give one record stable channel names such as `right.vel.x`, `right.vel.y` and `right.vel.z`. A conceptual update would be:

```json
{"right.vel.x": 10, "right.vel.y": 20, "right.vel.z": 30}
```

Channel names, units, transformations, source identity and time basis need defined meanings. Keep values from one input record together, distinguish missing updates from zero, and decide explicitly whether a dashboard retains or clears absent channels. The old sketch explored sparse updates and host-side mapping while keeping widget layout outside target firmware.

| Historical format candidate | Design question |
| --- | --- |
| MessagePack map | Named typed values and sparse updates; the draft's preferred binary path. |
| Binary TLV | Compact type/length/value records, with explicit float or fixed-point scaling rules. |
| ASCII key/value | Visible debug or transition format, with additional text formatting and bandwidth. |
| Protobuf | A schema and generated project/import workflow would be needed. |
| Raw bytes | Fixed channel order and types; sparse updates and mixed values need a separate contract. |

The draft considered generating dashboard projects and separate Trice-owned mapping metadata. Updating an existing project should preserve stable dataset identity, user layouts, colors, units, alarms and manual transforms. Suggested safeguards were backups before patching, additive updates and no automatic deletion of old datasets. The purpose and actual consumer of the proposed sidecar were still open questions. These are design considerations, not an implemented project converter.

Before reconsidering such an adapter, choose the consumer, verify its actual parser/project format, and define filtering, timestamps, numeric precision, record boundaries and ownership of generated settings. The old proposal's independent dashboard filtering must not be assumed to match today's `-vis` behavior. Its `ss_` tags, `-ssproj` options and MessagePack/project output were sketches, not supported Trice interfaces.

For current use, follow [Visualization](visualization.md): `-vis` writes user-defined text records to files or UDP, and the LabPlot examples demonstrate that path. The Serial Studio-specific work remains deferred; preserving its reusable ideas does not reopen it.

## 3. <a id="a-small-c-host-decoder"></a>A small C host decoder

The RX architecture proposal also considered decoding on systems without a suitable Go host, such as QNX, custom embedded Linux or constrained systems. The current [RX contract](target_implementation.md#receiving-and-resolving-records) and [local deferred formatter](target_implementation.md#local-deferred-text-log) provide building blocks; they do not constitute a standalone replacement for `tlog`.

The proposed layering was:

```text
application byte reader
  -> framing and optional decryption
  -> record parser
  -> ID metadata resolver
  -> formatter or application consumer
```

A caller-supplied read function and buffer could retain incomplete input between calls. Payload views must remain valid until the consumer finishes. RX-only builds should avoid TX buffers and output backends; framing, decryption and optional C++ host code should remain separate from the plain-C receive core and ABC handlers.

The later TIL draft proposed a deliberately small first host experiment: binary-file input, COBS/TCOBS, single/multi packets, generated C-friendly metadata and deterministic ASCII output. It would initially omit networking, RTT, XTEA, filters, colors, location information, a V2 JSON reader and a full Go CLI clone. This is a possible experiment, not an existing `examples/tlog-c` command or a new implementation plan.

Use the same binary fixtures to compare expected output, the Go decoder and a future C decoder. Include partial input, EOF, invalid/truncated records, byte order, stamp widths, counted buffers and mixed records. ABC demonstrations could cross-check the same generated bytes through both external and internal receivers while keeping the bus simulator protocol-neutral.

A full host tool would additionally need decisions about dictionary loading, formatter compatibility, optional locations, error reporting, memory ownership and supported transports. Any speed, memory or portability benefit needs measurements for the chosen environment; the historical proposal supplies no current benchmark or compatibility guarantee.

## 4. <a id="source-snapshots"></a>Source snapshots

This appendix consolidates the July 2026 TIL drafts, the deferred TSS sketch and the future-host part of the RX architecture paper. Their complete historical text remains in commit `43b51e94926fd274497c218147af7dfc6442ce97`. These commands read it without restoring the removed directory:

```sh
# Read historical proposals; these commands do not restore files.
source_commit=43b51e94926fd274497c218147af7dfc6442ce97
archive=docs/_scratchPad/obsolete
git show "$source_commit:$archive/til-json-v2-spezifikationsentwurf.md"
git show "$source_commit:$archive/trice-til-v2-entwurf-0.2/til-json-v2-spezifikation.md"
git show "$source_commit:$archive/trice_serial_studio_tss_entwurf.md"
git show "$source_commit:$archive/TriceRx_Tlog_C_ArchitectureDesign.md"
```

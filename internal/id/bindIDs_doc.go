// SPDX-License-Identifier: MIT

// Package id implements Trice ID management, including hierarchical metadata
// recovery for trice bind.
//
// # Bind Ownership Model
//
// One bind invocation has exactly one writable context. FnJSON is the primary
// TIL, LIFnJSON is the primary LI, and BindDir is the generated output tree.
// Source files selected through Srcs may name individual files or directory
// trees. Only the primary TIL, primary LI, current BindDir, and source lines
// that genuinely require bind instrumentation may be written.
//
// Additional JSON documents and generated sidecars are read-only evidence.
// They help a parent build preserve IDs previously assigned by a nested or
// differently rooted project, but they never become additional write targets.
// This distinction allows one source file to retain its File Key while fresh
// sidecars are generated in a different build/triceIDs directory.
//
// # Bounded Discovery
//
// For each source file, discovery starts in the file's directory and walks up
// to the most specific containing Srcs entry. A file-valued -src is anchored at
// the file's parent directory. At most one further parent directory is added.
// The directories containing the primary TIL and LI, plus at most one parent
// of each, are additional bounded anchors. Discovery never performs an
// unbounded filesystem search and never scans sibling trees recursively.
//
// Hidden directories are skipped during source traversal and implicit metadata
// discovery. A hidden directory is a path component whose name starts with a
// dot, excluding the navigation elements "." and "..". This makes .git, .trice,
// and similar obsolete cache trees inert without coupling bind behavior to Git
// repository boundaries. Explicit primary TIL, LI, and BindDir paths retain
// their CLI meaning; the hidden-directory rule controls source traversal and
// implicit discovery.
//
// Every immediate *.json file in the bounded directories is inspected by
// content. A non-empty map with numeric keys and Type/Strg values is a TIL; a
// non-empty map with numeric keys and File/Line values is an LI. Filenames have
// no semantic role, so names such as demoIDs.json are supported. Empty
// secondary maps contain no reusable evidence and are ignored. Malformed,
// mixed, unreadable, or unrelated secondary JSON is ignored and described only
// in verbose output. CLI-selected primary metadata remains authoritative and
// keeps its existing error behavior.
//
// # File Keys and Historical Sidecars
//
// A valid File Key stored in a source include is stable across source roots,
// metadata files, and BindDir changes. A bind-owned file without a File Key
// receives one when required. A duplicate key owned by multiple physical files
// remains fatal because the original owner cannot be inferred safely.
//
// For a source that already owns a File Key, discovery looks for its exact
// sidecar filename in the current BindDir and in conventional build/triceIDs
// directories along the bounded search corridor. Historical sidecars are
// parsed only as ID evidence. They are never copied: their line descriptors may
// be stale after source edits. Current Trice sites are analyzed again, old IDs
// are validated through available TIL data, and equal TriceFmt values are
// aligned in source order. The current BindDir always receives a freshly
// rendered sidecar and any required rebase helpers.
//
// Compact generated rebase lines are also regenerated semantically. When an
// old and new line have the same helper name, File Key, scope, and boundary
// kind, formatter-owned horizontal whitespace from the source is restored
// byte-for-byte. A no-op bind therefore does not rewrite or recompile an
// otherwise unchanged source merely because clang-format aligned a comment.
//
// # ID Candidate Order
//
// Trice matching continues to use the established parser and TriceFmt rules.
// A match is the normalized Trice type together with its exact format string;
// equal strings with different transport types are not interchangeable.
// Historical data supplies candidates in this order:
//
//   - a valid sidecar descriptor for the same File Key and ordered TriceFmt;
//   - a matching TIL entry with LI evidence for the same source file;
//   - an unused exact TriceFmt in the primary TIL without LI ownership;
//   - an unused exact TriceFmt in the nearest read-only TIL;
//   - a newly allocated ID from the primary ID range.
//
// Candidate ordering is deterministic by evidence strength, search proximity,
// line distance, numeric ID, and metadata path. Each numeric ID is claimed by
// at most one active site in a bind plan. Repeated equal TriceFmt values use
// sidecar and LI evidence when available and otherwise receive sorted IDs in
// sorted source order. Without any persistent location evidence, the exact old
// per-site association of identical formats is unknowable, but decoding remains
// correct and the fallback remains reproducible.
//
// Preferred historical IDs are inserted only into an in-memory copy of the
// source and then processed by insertTriceIDs. Bind therefore does not maintain
// an independent parser, allocator, range implementation, or conflict engine.
// The real source stays ID-free.
//
// # Primary Precedence and Recoverable Collisions
//
// Existing primary TIL mappings are immutable during conflict resolution. If a
// read-only project proposes an ID that the primary TIL maps to another type or
// string, the secondary proposal yields. Bind tries another exact-format ID and
// finally allocates a new primary ID. This is recoverable and silent by default;
// verbose output explains the discarded candidate.
//
// A conflict-free secondary ID is retained even when it lies outside IDMin and
// IDMax, provided it is otherwise a valid Trice ID. Those ranges govern only
// new allocation. The selected mapping is added to the primary TIL, and its
// current location is written to the primary LI. Only entries used by active
// scanned sites are imported. Secondary TILs, LIs, sidecars, and helper files
// remain unchanged.
//
// If multiple read-only projects propose the same free number differently,
// deterministic site processing lets the strongest first proposal claim it;
// later proposals yield and receive another existing or new ID. Thus a parent
// product can compile a subproject with a parent-specific ID while the same
// source continues to use its original ID in the standalone subproject build.
//
// # Diagnostics and Transactionality
//
// Recoverable secondary-data problems are verbose information, not normal
// diagnostics. Fatal conditions are limited to states from which no complete
// safe primary output can be planned, such as malformed primary metadata,
// duplicate File-Key ownership, unsupported source structure, exhausted ID
// space, or an output failure.
//
// Source analysis, discovery, conflict resolution, ID assignment, JSON
// rendering, and sidecar rendering complete before the first regular write.
// Fatal analysis emits sorted actionable diagnostics and writes no source,
// primary metadata, or generated artifact. The commit phase compares exact
// bytes, omits unchanged destinations, uses atomic replacement, and rolls back
// earlier replacements if a later destination fails.
package id

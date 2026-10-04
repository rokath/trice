Shared JetBrains project settings are versioned here. The module list uses
`trice.iml`; the former identical `trice.rokath.iml` is unnecessary.

The shared spelling dictionary is `dictionaries/project.xml`, in JetBrains XML
format. It is currently empty; add project-wide words through the IDE. It is not
a plain-text `trice.dict` or `trice.dic` file. Other, personal dictionaries in that
directory are ignored.

CLion uses the [CMake review host](../_test/clion-review/CMakeLists.txt) for
indexing and navigation of the C library. Select an installed host toolchain in
CLion; no particular installation directory is prescribed. This review host
does not replace the embedded example builds or the repository tests.

Workspace layout, task state, statistics, shelves, caches and generated IDE
metadata are local and ignored. Shared code styles, inspections, editor settings,
module definitions and VCS mappings remain versioned. Keep personal toolchain
paths and credentials out of the shared files.

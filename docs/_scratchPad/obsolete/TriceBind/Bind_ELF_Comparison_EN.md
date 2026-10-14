# Historical Bind and ELF Comparison

This is the retained English translation of an earlier design comparison. It describes historical proposals, including limitations that have since changed. It is not the current product specification. Current usage and supported source constructs are documented in [Trice Bind](../../../TriceUserManual.md#trice-bind).

## Architecture Decision: `trice bind` Instead of an ELF-Patching Solution

### Purpose of This Document

This document explains the decision to use `trice bind` and generated sidecar headers to bring stable Trice IDs into the target code. An ELF-based patching or linking solution was investigated as an alternative.

The decision concerns only the mechanism by which an already determined Trice ID reaches the compiler or final target code. The existing persistent ID management with `til.json` and `li.json` remains unchanged.

### Requirements

The binding mechanism should:

- use stable IDs from `til.json` and `li.json`,
- require no numeric IDs in user Trice calls,
- compile the original sources directly,
- require no additional runtime lookup on the target,
- leave the existing Trice wire format unchanged,
- keep historical logs decodable without the ELF belonging to the build,
- work with different C and C++ toolchains,
- avoid unnecessarily expanding incremental builds,
- generate understandable and reproducible build artifacts.

### MVP of `trice bind`

#### Basic Principle

The user writes an ID-free Trice log site:

```c
trice("msg:module initialized\n");
```

`trice bind` scans the source files before preprocessing, maps every supported log site to a stable ID from `til.json` and `li.json`, and generates one sidecar header per file.

For a source file `module.c`, the include line stored persistently in the user code may look like this:

```c
#include "trice_module_c_F73A915E9C4021B8.h" // trice-bind
```

`F73A915E9C4021B8` is a randomly generated 64-bit file key created once and represented as a preprocessor token beginning with `F`. It identifies the file independently of its path and safely distinguishes files with identical names. The file key is not a Trice ID, is not transferred to the target, and occupies no target memory.

The generated header contains, for example:

```c
#undef TRICE_FILE_KEY
#define TRICE_FILE_KEY F73A915E9C4021B8

#define TRICE_ID_F73A915E9C4021B8_L9 12345u // trice("msg:module initialized\n")
```

The Trice macros combine the current `TRICE_FILE_KEY` with the standardized `__LINE__` preprocessor macro. The compiler ultimately sees an ordinary integer constant.

#### Why a File-Local Include Is Required

All sidecars processed in a translation unit share the same macro namespace. The 64-bit file key prevents collisions between their ID definitions. In addition, every direct Trice call must be associated with the file to which its line number belongs.

Including all sidecars centrally, for example in `triceConfig.h`, provides all ID macros but does not select the file key belonging to a particular log site. The C preprocessor cannot derive a matching macro name from the `__FILE__` string literal.

Therefore, each file's sidecar sets the current `TRICE_FILE_KEY` immediately before the file's own Trice log sites. The file-local include is not just a storage location for definitions; it is part of the unambiguous `file + line` selection.

#### Sidecar Directory and File Names

All sidecars can reside in a single build directory, for example:

```text
generated/
```

The build then needs only one additional include path. The base name in the sidecar name improves readability, while the file key provides uniqueness:

```text
trice_module_c_F73A915E9C4021B8.h
trice_module_c_F88217D4AC101E62.h
trice_module_h_F1111111111111111.h
```

The complete source directory structure does not need to be replicated under `build/`.

#### File-Key Persistence and Validation

The file key is stored in the version-controlled include line of the user file. It therefore survives deletion of the build directory or movement of the file.

64 bits are sufficient for this purpose. In addition, `trice bind` verifies that no key is assigned to multiple different files within a project. This detects both an extremely unlikely random collision and a duplicated key caused by copying a source file.

#### One-Time Include Insertion

If the sidecar include is absent, `trice bind` adds it once using a heuristic to choose a likely suitable position. It prefers a position:

- after the normal includes effective for the file,
- before the file's first direct Trice log site,
- inside an existing include guard for header files.

Without fully evaluating all preprocessor conditions, the position cannot be determined safely in every C or C++ program. Conditional includes can prevent unambiguous automatic placement. Therefore:

- An existing correct include is not moved.
- An automatically inserted line is clearly marked with `// trice-bind`.
- `trice bind` checks structural plausibility.
- For an uncertain or contradictory structure, the user receives a specific diagnostic and moves the marked line if necessary.

The one-time include addition is not recurring instrumentation. During normal builds, user sources are not modified.

#### Header Files and `static inline`

Header files with direct Trice calls receive their own file key and sidecar. This also applies to Trice calls inside `static inline` functions.

Example:

```c
#ifndef MODULE_H
#define MODULE_H

#include "dependency.h"
#include "trice_module_h_F1111111111111111.h" // trice-bind

static inline void moduleCheck(int value)
{
    trice("msg:value=%d\n", value);
}

#endif
```

After processing this header, the sidecar of the including `.c` file sets that file's own `TRICE_FILE_KEY`. This keeps log sites from the header and source file collision-free.

#### MVP Limitations

The MVP supports direct Trice calls in `.c`, `.cc`, `.cpp`, and header files, as well as in normal and `static inline` functions.

Initially unsupported are:

- multiple Trice calls on the same physical source line,
- Trice calls inside a preprocessor macro definition, for example:

```c
#define LOG_ERROR(x) trice("error=%d\n", x)
```

For a Trice call in a macro definition, `__LINE__` and the current file key are evaluated only during the later macro expansion. They therefore describe the call site and not reliably the definition site. Simple binding by `file + line` is insufficient.

`trice bind` reports such constructs as an error in the MVP. Existing projects that depend on them continue to use `trice insert`.

### Investigated ELF-Patching Solution

With an ELF-based solution, Trice macros would generate additional metadata and bindable ID placeholders in object files during compilation. A later tool would have to evaluate this information and insert the final IDs through relocations, additional link objects, or direct patching of the object or image code.

Such a solution requires at least:

- a defined metadata format in object sections,
- an unambiguously identifiable placeholder for every log site,
- support for the relevant ELF and relocation variants,
- knowledge of the target architecture and ABI,
- coordinated behavior with the linker, section garbage collection, and LTO,
- handling of object archives and archive members that are only partially selected,
- separate solutions for toolchains that do not use ELF or use it differently.

An ELF solution also requires prepared Trice macros or prepared libraries. Neither complete Trice metadata nor safely patchable ID sites can be reconstructed from an arbitrary, already compiled `.a` file.

### Comparison of the Two Approaches

| Criterion                                    | `trice bind` MVP                                        | ELF-patching solution                                |
|----------------------------------------------|---------------------------------------------------------|------------------------------------------------------|
| Numeric IDs in the Trice call                | no                                                      | no                                                   |
| Original sources are compiled                | yes                                                     | yes                                                  |
| Stable IDs in target code                    | directly as C constants                                 | through an additional patch/link step                |
| Runtime lookup on the target                 | no                                                      | not required, but design-specific                    |
| Wire-format change                           | no                                                      | no, if implemented accordingly                       |
| Build-specific ELF required for log decoding | no                                                      | avoidable only if final stable IDs are bound cleanly |
| Dependency on ELF and relocations            | no                                                      | yes                                                  |
| Dependency on target architecture and ABI    | no                                                      | yes                                                  |
| Dependency on linker and LTO behavior        | no                                                      | yes                                                  |
| Language mechanisms used                     | standardized C/C++ preprocessor rules, `__LINE__`, `##` | compiler, object-format, and linker mechanisms       |
| Understandability of intermediate artifacts  | simple generated headers                                | object sections, symbols, and relocations            |
| Incremental build                            | affected translation unit through its sidecar           | depends on patch and link workflow                   |
| Additional include in user code              | yes                                                     | no                                                   |

For a normal source project, the remaining general advantage of the ELF solution is therefore essentially that it would avoid the file-local sidecar include. This convenience comes at the cost of substantially greater toolchain and implementation complexity.

### Examination of the Apparent ELF Advantages

#### Precompiled Static Libraries

A prepared `.a` library can be supported later without replacing the normal sidecar mechanism. During its own build, the library would have to generate metadata and a bindable ID placeholder for every Trice site. During the product build, `trice bind` could read this information, determine free or new IDs, extend `til.json` and `li.json`, and generate an additional binding artifact for the final link.

The benefit would be subsequent integration of prepared libraries into the stable ID space of the final product.

This feature can use ELF internally, but it is an additive library extension. It does not require ELF-based handling of normal user sources and is therefore not an independent advantage of a general ELF-patching architecture.

#### Discovery of Inactive Log Sites

The MVP scans sources before preprocessing. As a result, it also discovers log sites in currently inactive `#if` branches. This is desirable for stable IDs: a log site does not lose its mapping merely because a specific build configuration temporarily disables it.

An ELF file, by contrast, contains only code that reached at least the object or link stage. It is therefore not the appropriate source for a complete, configuration-independent ID inventory.

#### Active Log Sites in a Build Configuration

If the log sites active in a specific configuration also need to be determined, the actual preprocessor can later be run with the defines and include paths of that build.

This requires:

- the actual preprocessor options of every translation unit,
- include paths and defines,
- optionally a `compile_commands.json` or comparable build description,
- a mapping from preprocessed sites to stable IDs.

The benefit is a report of the active subset. ID assignment and sidecar binding do not change.

#### Log Sites Actually Present in the Final Image

An active log site can disappear from the final image through optimization, LTO, section garbage collection, or because an archive member is not selected. If an exact image inventory is required, the final ELF or a link map can be analyzed later.

This requires:

- an identifiable relationship between final code and Trice ID,
- toolchain-specific ELF or map analysis,
- consideration of LTO and linker optimizations.

The benefit is an exact report of the subset remaining in the specific image. This analysis also changes neither the ID mapping nor the sidecar binding.

The three sets are therefore clearly distinct:

```text
all textually present log sites
        ⊇ log sites active in one configuration
        ⊇ log sites present in the final image
```

Only the first set is required for stable ID assignment in the MVP.

#### Macro Expansion and String Generation

Trice requires static, directly recognizable format strings. Generating different format strings through preprocessor concatenation is not intended. Variable content is transferred as parameters, for example with a Trice string variant.

Consequently, individual expanded format-string variants do not have to be distinguished in object code. This does not create a relevant ELF advantage either.

### Future Additive Extensions

The following functions are explicitly not part of the MVP. They can be added later without changing the basic sidecar model.

#### Active-Configuration Analysis

**Required:** Run the actual preprocessor with the build options of every translation unit.

**Benefit:** Report which of the already bound log sites are active in a specific configuration.

#### Post-Link Image Inventory

**Required:** Analyze ELF or the link map and map the result to Trice IDs in a toolchain-specific way.

**Benefit:** Exact list of log sites present in the final image.

#### Prepared `.a` Libraries

**Required:** Library-side metadata and bindable ID placeholders, plus an additional binding artifact for the final link.

**Benefit:** Subsequent assignment of stable IDs from the final product's ID space without rebinding the library from its sources.

#### Multiple Trice Calls per Source Line

**Required:** An additional stable occurrence index or a suitable, sufficiently portable counter mechanism.

**Benefit:** Support for a syntactically possible but currently unnecessary coding style.

#### Trice Calls in Macro Definitions

**Required:** Defined semantics for definition-site or call-site IDs and an additional mechanism, such as selective `trice insert`, explicit wrapper identifiers, or preprocessor analysis.

**Benefit:** Migration of existing wrapper macros to a mixed workflow.

These extensions supplement the MVP. None of them requires switching normal user sources to a general ELF-patching solution.

### Decision

For normal C and C++ sources, `trice bind` with file-local sidecar headers will be pursued. A general ELF-patching solution will not be pursued further.

The decision is based on the following technical points:

1. The MVP binds stable IDs as compile-time constants using standardized C/C++ preprocessor facilities.
2. It requires no ELF knowledge or architecture, ABI, relocation, or linker logic.
3. `til.json` and `li.json` remain the persistent ID truth across builds.
4. Historical logs remain decodable without the corresponding ELF.
5. Stable 64-bit file keys distinguish files with identical names and log sites in headers unambiguously.
6. The original sources are built directly; only a sidecar include added once remains in the user code.
7. The apparent functional advantages of the ELF solution can be implemented, if needed, as additive analyses or specialized extensions.
8. These extensions do not change the MVP binding mechanism.
9. The main general ELF advantage that remains is avoiding a file-local include. That advantage does not justify the additional toolchain complexity.

The architecture therefore remains simple: source scanning and persistent databases determine the IDs, generated headers provide them to the standards-conforming preprocessor, and the existing compiler generates the target code.

# Trice bind MVP2 preprocessing PoCs

This directory contains two independent verification prototypes. They do not
change the current `trice bind` MVP or its generator.

Run both with:

```sh
./run.sh
```

Set `CC` to check one compiler explicitly, for example `CC=clang ./run.sh`.

## Multiple Trice sites on one line

`counter_sites.c` uses the production expansion-time dispatch from
`src/triceBind.h` and changes only the site selector from:

```text
(file key, physical line)
```

to:

```text
(file key, physical line, __COUNTER__ value)
```

Three `trice` calls share logical line 100 and two calls share logical line
200. Every call resolves a distinct simulated sidecar descriptor. An unrelated
`__COUNTER__` expansion deliberately consumes value 3, demonstrating that a
generator must observe the complete counter stream rather than merely count
Trice calls.

This mechanism works only when generator and compiler see an identical token
stream, include order, predefined-macro set, conditional configuration, and
forced-include configuration. A changed third-party use of `__COUNTER__` shifts
all later keys. `__COUNTER__` is also a common compiler extension, not a C or
C++ language guarantee.

The executable also contains two no-`__COUNTER__` controls on same-line calls:

- an unchanged line-only source plus helper descriptor maps both calls to ID
  4199, confirming the collision;
- a simulated generated shadow source carries explicit ordinals `0` and `1`
  and resolves distinct IDs 4201 and 4202.

## Trice inside a macro definition

`macro_definition.c` contains a real multi-line logging macro of this form:

```c
#define LOG_ERROR(value)           \
    do {                           \
        switch (value) {           \
        case 7:                    \
            trice("error=%d", 7); \
            break;                 \
        case 8:                    \
            trice("error=%d", 8); \
            break;                 \
        default:                   \
            break;                 \
        }                          \
    } while (0)
```

The logical macro definition starts at `wrapper_definition.h:50`. Calls at
`wrapper_owned.c:300` and `wrapper_owned.c:301` each expand both Trice branches
during preprocessing. The sidecar therefore contains four invocation
descriptors: the case-7 definition consistently carries ID 5101 and the case-8
definition consistently carries ID 5102. Runtime switch selection emits only
the applicable branch. This proves three relevant properties:

- nested expansion evaluates `__LINE__` at the wrapper invocation;
- separate Trice definitions inside one wrapper can receive separate IDs;
- a generator can preserve each definition identity by assigning its stable ID
  to every generated invocation-site descriptor.

This is deliberately stronger than the initial one-Trice wrapper proposal. Two
inner Trices share the wrapper invocation's `__LINE__`, so this form also needs
an occurrence key such as `__COUNTER__` or explicit ordinals in generated
compiler input. A practical generator should still exclude token pasting,
stringification, nested wrappers, and dynamic format construction initially.
The current MVP parser rejects Trice calls in ordinary macro definitions; this
PoC demonstrates target feasibility, not generator support.

## Can helper files replace `__COUNTER__`?

Not while the compiler consumes the original source unchanged. The standard C
preprocessor exposes a line but no column or occurrence index, and an included
header cannot mutate macro state once per expansion. Two identical macro calls
on one physical line therefore present the same lookup key to a plain sidecar.

The executable confirms that pre-generated helper files can work when paired
with another mechanism that changes or augments the compiler input. Options
include:

- compile a generated shadow source that gives every site an explicit ordinal;
- rewrite calls in generated compiler input to an internal `TRICE_BIND_AT(n, …)`
  form while preserving original files and diagnostic mappings;
- use a compiler-specific column builtin where one exists;
- run the exact target preprocessor first and use its observed `__COUNTER__`
  values, accepting configuration coupling;
- emit prepared metadata objects and assign IDs during a later link/patch step.

A helper header by itself, including one containing additional `#line`
directives, cannot distinguish multiple unchanged calls on the same source
line. The least invasive source-based candidate is therefore `__COUNTER__`, but
only with a verified and reproducible preprocessing configuration.

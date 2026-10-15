# Retired Manual Drafts

Historical proposals and an obsolete workflow overview removed during R20. These texts are not current user contracts or implementation instructions. Source: the full manual at commit 7ea9ec92. The active Context Enrichment appendix is retained in the reference manual.

<!--
### 45.1. <a id="trice-log-level-control-specification-draft"></a>Trice Log-level Control Specification Draft

> Specification Draft

#### 45.1.1. <a id="what-log-levels-exist-in-general-including-exotic-ones-and-what-is-their-exact-weighting-relative-to-each-other"></a>What log levels exist in general, including exotic ones, and what is their exact weighting relative to each other?

🧭 **Basic principle**

* Lower level → more noise / diagnostic detail
* Higher level → more severe / critical condition

🔢 **Common standardized levels (by severity)**

| Level | Name                      | Weight          | Meaning                                                                                              |
|-------|---------------------------|-----------------|------------------------------------------------------------------------------------------------------|
| 0     | TRACE                     | lowest          | Finest-grained details — e.g., every function call or variable change. Used for deep debugging only. |
| 1     | DEBUG                     | low             | Developer-level details about execution flow. No failure.                                            |
| 2     | INFO                      | medium          | Normal operational messages — startup, config loaded, connection established.                        |
| 3     | NOTICE                    | slightly higher | Significant but expected events (e.g., user login). Exists in syslog.                                |
| 4     | WARN / WARNING            | rather high     | Something unexpected but not yet a failure. System continues running.                                |
| 5     | ERROR                     | high            | A problem occurred — operation failed, but program still runs.                                       |
| 6     | CRITICAL                  | very high       | A subsystem failure. Urgent attention required.                                                      |
| 7     | ALERT                     | extremely high  | Immediate human intervention needed.                                                                 |
| 8     | EMERGENCY / FATAL / PANIC | highest         | System unusable. Shutdown or restart required.                                                       |

🧩 **Rare or exotic variants**

| Name                       | Origin / Context                | Severity                | Description                                    |
|----------------------------|---------------------------------|-------------------------|------------------------------------------------|
| VERBOSE                    | Windows, Android, C/C++ loggers | Between TRACE and DEBUG | Very detailed, but not quite as deep as TRACE. |
| SUCCESS / OK / PASS        | Test frameworks                 | Between INFO and NOTICE | Indicates successful operations.               |
| FAIL                       | Test frameworks                 | ERROR                   | Failed test but not system error.              |
| SECURITY / AUDIT           | Compliance systems              | Variable                | Logs security or compliance events separately. |
| CONFIG / INIT              | Embedded / frameworks           | INFO                    | Configuration or initialization messages.      |
| DEPRECATION                | Compilers, frameworks           | WARN                    | Deprecated feature warnings.                   |
| ASSERT                     | Debuggers, C/C++                | CRITICAL                | Assertion failure, usually aborts program.     |
| NOTICE / IMPORTANT / EVENT | Various                         | Between INFO and WARN   | Events worth attention but not errors.         |
| OFF                        | Logging frameworks              | none                    | Turns off all logging.                         |
| ALL                        | Logging frameworks              | lowest                  | Enables every log level.                       |

🧮 **Example comparison across systems**

| Severity | Syslog  | Log4J / Java | Python   | .NET        | Meaning          |
|----------|---------|--------------|----------|-------------|------------------|
| 0        | debug   | TRACE        | NOTSET   | Trace       | Internal details |
| 1        | info    | DEBUG        | DEBUG    | Debug       | Developer info   |
| 2        | notice  | INFO         | INFO     | Information | Normal ops       |
| 3        | warning | WARN         | WARNING  | Warning     | Unexpected       |
| 4        | err     | ERROR        | ERROR    | Error       | Operation failed |
| 5        | crit    | FATAL        | CRITICAL | Critical    | Severe           |
| 6        | alert   | —            | —        | —           | Immediate action |
| 7        | emerg   | —            | —        | —           | System crash     |

🧠 **Suggested numeric scale**

| Level                     | Weight | Meaning           |
|---------------------------|--------|-------------------|
| TRACE                     | 10     | Ultra-detailed    |
| VERBOSE                   | 20     | Very detailed     |
| DEBUG                     | 30     | Developer info    |
| INFO                      | 40     | Normal operation  |
| NOTICE                    | 50     | Significant event |
| WARN                      | 60     | Warning           |
| ERROR                     | 70     | Error             |
| CRITICAL                  | 80     | Serious problem   |
| ALERT                     | 90     | Urgent            |
| EMERGENCY / FATAL / PANIC | 100    | Total failure     |

For The Trice project (an embedded logging tool), logging must be:

* lightweight,
* memory-efficient (Flash/RAM),
* but still expressive enough for both developers and customers.

Here’s a 7-level scheme, embedded-friendly yet compatible with syslog/log4j conventions:

🔧 Recommended Trice Log Level Scale

| Macro/Level           | Name                | Weight | Meaning                            | Typical Use                                 |
|-----------------------|---------------------|--------|------------------------------------|---------------------------------------------|
| **0 – trice_SILENT**  | **OFF / NONE**      | 0      | No output at all.                  | Disable logging in release builds.          |
| **1 – trice_FATAL**   | **FATAL / PANIC**   | 100    | System unusable, restart required. | Watchdog reset, hard fault, stack overflow. |
| **2 – trice_ERROR**   | **ERROR**           | 80     | Recoverable error.                 | CRC failure, timeout, file missing.         |
| **3 – trice_WARN**    | **WARN**            | 60     | Unexpected but tolerable.          | Retry, threshold exceeded.                  |
| **4 – trice_INFO**    | **INFO**            | 40     | Regular operation messages.        | Init complete, connection established.      |
| **5 – trice_DEBUG**   | **DEBUG**           | 30     | Developer-level diagnostics.       | Variable states, state transitions.         |
| **6 – trice_VERBOSE** | **VERBOSE / TRACE** | 10     | Deepest trace level.               | Function calls, ISR entry, timings.         |

🎯 Advantages

* [x] Compatible with Syslog conventions
* [x] Filtered by one threshold (if(level <= currentLevel))
* [x] Backward-compatible (old log constants still valid)
* [x] Easily extendable (e.g., add NOTICE or ASSERT later)

#### 45.1.2. <a id="compile-time-log-level-control"></a>Compile-time Log-level Control

In [Trice Structured Logging Compile-time Information](#trice-structured-logging-compile-time-information) we see, how `trice insert ...` could modify (temporarily) the source code. With an additional *insert* switch like `-loglevel` the shown example could get changed in this way:

User may have written inside *val.c*:

```C
void doStuff( void ){
    // ...
    trice("info:The answer is %d.\n", 42);
    // ...
}
```

and a `trice insert -loglevel` command could change that into (revertable with `trice clean`):

```C
void doStuff( void ){
    // ...
    trice_INFO(iD(123), "info:The answer is %d.\n", 42);
    // ...
}
```

The idea here is to modify also the `trice` macro name into `trice_INFO`, when a Trice tag "info" or "inf" was found. We could define this way:

```C
#define TRICE_LOG_LEVEL TRICE_LEVEL_INFO

#if TRICE_LOG_LEVEL >= TRICE_LEVEL_INFO
#define trice_INFO(...) trice( __VA_ARGS__)
#else
#define trice_INFO(...) ((void)0)
#endif

#if TRICE_LOG_LEVEL >= TRICE_LEVEL_DEBUG
#define trice_DEBUG(...) trice(__VA_ARGS__)
#else
#define trice_DEBUG(...) ((void)0)
#endif
```

That results in no code generation for `trice("info:The answer is %d.\n", 42);` for TRICE_LOG_LEVEL < TRICE_LEVEL_INFO. What we get this way is:

* A fine-granular compile-time log-level control.
* The user is free to add its own log-levels.
* No run-time costs at all.

#### 45.1.3. <a id="run-time-log-level-control"></a>Run-time Log-level Control

Trice logs are very light-weight and usually is no need for their run-time control. Nevertheless there could be a need for that. The very first we need, is a control channel to tell the target device about a changing log-level. See for example chapter [Stimulate target with a user command over UART](#stimulate-target-with-a-user-command-over-uart).

When we are able to set a value *LogLevel* in the target device, we can use this value as an ID threshold in combination with the `-IDRange` switch. More in detail as an example:

```bash
trice insert -loglevel -IDRange debug:1,999 -IDRange info:2000,2999 -IDMin 4000 -IDMax 9999 -IDRange err:10000,10999
```

It is important to understand, that all other Trice messages get IDs in the range `-IDMin` and `-IDMax` and that no range overlapping is allowed.

| LogLevel | Result                                    |
|---------:|-------------------------------------------|
|    16384 | no output                                 |
|    10000 | only error messages                       |
|     4000 | normal messages and error messages        |
|     2000 | all output except info and debug messages |
|     1000 | all output except debug messages          |
|        0 | all output                                |

That implies a small Trice library extension, which gets active only with a `LOGLEVELS` switch. In that case we get a small additional run-time overhead. What we cannot achieve this way is a tag specific target-side selection, but that would be no big deal to add as well.

-->


<!--

> **Specification Draft**

Context Enrichment automatically adds compile time and runtime data to logs. The user should be able to configure, which data get added and also should have control about the data formatting. The generic data insertion should also work with structured logging as option.

Trice is considerable already a bit as a (very limited) context enrichment logger, if we look at the file and line insertion capability and the timestamp options. The following is about how Trice could get full Context Enrichment capability without making a breaking change.

#### 45.2.1. <a id="trice-context-enrichment-compile-time-information"></a>Trice Context Enrichment Compile-time Information

*file, line, function, compiler version, module, build time, firmware version, machine name, user name, locale, host OS version, log level, format string, compiler flags, (locally) defined values...*

These data can be strings or numbers.

#### 45.2.2. <a id="trice-context-enrichment-runtime-information"></a>Trice Context Enrichment Runtime Information

*uptime, timestamp, hw serial, task ID, stack depth, event count, core ID, device position, variables values, parameter values ...*

In an initial approach we assume, these data do not contain runtime generated strings. If really needed, a derived hash is usable instead for now. Despite of this, runtime generated strings are an important feature and therefore Trice supports `triceS`, capable to transmit a single string up to 32KB long, and `triceS` relatives like `triceB`. We could add compile-time data (as inserted fixed strings) but runtime information can only get as an additional part of the runtime generated string into the structured log. This should be acceptable and we will deal with this later.

#### 45.2.3. <a id="trice-context-enrichment-limitations-and-special-cases"></a>Trice Context Enrichment Limitations and Special Cases

For performance reasons, Trice was designed to only transmit 0-12 (straight forward extendable) numbers of equal bit-width **OR** a single runtime generated string. Firstly we look at only "normal" Trice macros `trice`, `Trice`, `TRice` and exclude the special cases `triceS`, `TriceS`, `TRiceS`. Also we consider just trices without specified bit-width, assume 32-bit and exlude cases like `trice32_4` firstly.

#### 45.2.4. <a id="a-trice-context-enrichment-example"></a>A Trice Context Enrichment Example

User may have written inside *val.c*:

```C
void doStuff( void ){
    // ...
    trice("info:The answer is %d.\n", 42);
    // ...
}
```

and (as we know) a `trice insert` command would change that into (revertable with `trice clean`):

```C
void doStuff( void ){
    // ...
    trice(iD(123), "info:The answer is %d.\n", 42);
    // ...
}
```

But a `trice insert` command with context option will, for example, change that line into (revertable with `trice clean`):

```C
void doStuff( void ){
    // ...
    trice(iD(456), "[level=info][file=\"val.c\"][line=321][func=doStuff][taskID=%x][fmt=\"The answer is %d.\"][uptime=%08us][temperature=%3.1f°C]\n", getTaskID(), 42, uptime(), aFloat(sensorValue));
    // ...
}
```

#### 45.2.5. <a id="trice-context-enrichment-cli-switches-and-variables"></a>Trice Context Enrichment CLI Switches and Variables

To achieve that, 2 Context Enrichment CLI switches `-cef` and `-cev` on `trice insert` and `trice clean` are usable:

| CLI switch | meaning                   |
|------------|---------------------------|
| `-cef`     | Context Enrichment format |
| `-cev`     | Context Enrichment values |

Additionally the Trice tool uses these internal variables (no bash variables!) as replacements during `trice insert` and `trice clean`:

| Variable  | Example               | Comment                                                                                                                                          |
|-----------|-----------------------|--------------------------------------------------------------------------------------------------------------------------------------------------|
| `$level`  | `info`                | The bare trice format string part until the first colon (`:`), if known as channel/tag value.                                                    |
| `$file`   | `val.c`               | The file name, where the Trice log occures.                                                                                                      |
| `$line`   | `321`                 | The file line, where the Trice log occures.                                                                                                      |
| `$func`   | `doStuff`             | The function name, where the Trice log occures.                                                                                                  |
| `$fmt`    | `The asnwer is %d.`   | The bare Trice format string stripped from the channel/tag specifier including the colon (`:`) according to the Trice rule (lowercase-only ones) |
| `$values` | `42`                  | The bare Trice statement values.                                                                                                                 |
| `$usr0`   | `abc` \| ` ` \| `xyz` | A predefined string value with location dependent values (see below).                                                                            |

#### 45.2.6. <a id="trice-context-enrichment-user-defined-values"></a>Trice Context Enrichment User Defined Values

This use case is not expected for most cases, but mentioned here to show the possibilities. Adding user specific values like `$usr0` can be done in this way:

* File *main.c*:

```C
 88 | ...
 89 | #define XSTR(x) STR(x)
 90 | #define STR(x) #x
 91 | 
 92 | trice("info:hi");
 93 |  
 94 | #define TRICE_ETC "xyz"
 95 | #pragma message "$usr0=" XSTR(TRICE_ETC)
 96 | trice("info:hi");
 97 | 
 98 | #undef TRICE_ETC
 99 | #pragma message "$usr0=" XSTR(TRICE_ETC)
100 | trice("info:hi");
101 | 
102 | #define TRICE_ETC "abc"
103 | #pragma message "$usr0=" XSTR(TRICE_ETC)
104 | trice("info:hi");
105 | ...
```

This is just a demonstration. The `#pragma message "$usr0=" XSTR(TRICE_ETC)` line probably is needed only on a few lines in the project. A pre-compile output 

```bash
$ ./build.sh 2>&1 | grep "pragma message:"
Core/Src/main.c:95:9: note: '#pragma message: $usr0="xyz"'
Core/Src/main.c:99:9: note: '#pragma message: $usr0=""'
Core/Src/main.c:103:9: note: '#pragma message: $usr0="abc"'
```

could get transferred automatically to the Trice tool, with a user generator script to tell, that normally `$usr0=""`, but `$usr0="xyz"` for Trices in file *main.c* from line 95 to 99, that `$usr0="abc"` is valid for *main.c* after line 103.

Those things are compiler and user specific and not part of the Trice tool design. But on demand a CLI multi switch `-stu` can get invented, to inject such information into the `trice insert` process automatically. With

```bash

CEF='{"level":"%s","loc":"%s:%d","fmt":"$fmt","etc":"%s"}'
CEV='$level, $file, $line, $values, $usr0'

# user script generated begin ################################################
ST0='usr0="xyz":main.c:95'                        # user script generated line
ST1='usr0="":main.c:99'                           # user script generated line
ST2='usr0="abc":main.c:103'                       # user script generated line
STU="-stu $ST0 -stu $ST1 -stu $ST2"               # user script generated line
# user script generated end ##################################################

trice insert $STU -cef $CEF -cev $CEV
```

The structured log output would be:

```bash
{...}
{"level":"info","loc":"main.c:92","fmt":"hi","etc":""}
{"level":"info","loc":"main.c:96","fmt":"hi","etc":"xyz"}
{"level":"info","loc":"main.c:100","fmt":"hi","etc":""}
{"level":"info","loc":"main.c:104","fmt":"hi","etc":"abc"}
{...}
```

#### 45.2.7. <a id="trice-context-enrichment-cli-switches-usage-options"></a>Trice Context Enrichment CLI Switches Usage Options

The in [A Trice Context Enrichment Example](#a-trice-structured-logging-example) shown `trice insert` result is possible with
 
```bash
trice insert \
-cef='[level=$level][file=$file][line=$line][func=$func][taskID=%x][fmt=$fmt][uptime=%08us][temperature=%3.1f°C]' \
-cev='getTaskID(), $values, uptime(), aFloat(sensorValue)'
```

The raw string syntax is mandatory here, to pass the internal Trice tool variables names. 

Adding variable values like `$line` as strings has performance advantages, but on each such value change a new Trice ID is generated then. Those variables are better inserted as values, if the code is under development. A `$line` value insertion looks like this:

```bash
trice insert \
-cef='[level=$level][file=$file][line=%5d][func=$func][taskID=%04x][fmt=$fmt][uptime=%08us][temperature=%3.1f°C]' \
-cev='$line, getTaskID(), $values, uptime(), aFloat(sensorValue)'
```

It is also possible to use string format specifiers to allow somehow aligned values. For example:

```bash
trice insert \
-cef='[level=%-6s][file=%24s][line=%5d][func=%-16s][taskID=%04x][fmt=$fmt][uptime=%08us][temperature=%3.1f°C]' \
-cev='$level, $file, $line, $func, getTaskID(), $values, uptime(), aFloat(sensorValue)'
```

Or, if you like alignment after the format string, even:

```bash
trice insert \
-cef='[level=%-6s][file=%24s][line=%5d][func=%-16s][taskID=%04x][fmt=%64s][uptime=%08us][temperature=%3.1f°C]' \
-cev='$level, $file, $line, $func, getTaskID(), $fmt, $values, uptime(), aFloat(sensorValue)'
```

The user has full control and could also use any other syntax like a JSON format. Only the format specifiers are requested to match the passed values after the Trice tool internal variables replacement during `trice insert`, so that the Trice tool can perform a printf during logging.

To achieve a log output in compact JSON with line as string we can use:

```bash
trice insert \
-cef='{"level":"$level","file":"$file","line:"$line","taskID":"%04x","fmt":$fmt,"uptime":%08u us"}' \
-cev='getTaskID(), $values, uptime()'
```

**To put things together:** Any structured format string design is possible and the user can insert the $line (example) value:

* directly as string (fastest execution, straight forward)
* indirectly as formatted string (fastest execution alignment option)
* indirectly as formatted number (recommended when often changing)

After `trice insert` we get this (compact JSON) log line according to `-cef` and `-cev`:

```C
void doStuff( void ){
    // ...
    trice(iD(789), "{\"level\":\"info\",\"file\":\"val.c\",\"line\":\"321\",\"taskID\":\"%04x\",\"fmt\":\"The answer is %d.\",\"uptime\":\"%08u us\"}\n', getTaskID(), 42, uptime());
    // ...
}
```

All compile time strings are part of the Trice format string now, which is registered inside the *til.json* file. The needed Trice byte count stays 4 bytes only plus the 3 times 4 bytes for the runtime parameter values taskID, 42, uptime. The default [TCOBS](https://github.com/rokath/tcobs) compression will afterwards reduce these 16 bytes to 12 or 13 or so.

A `trice clean` command will remove the context information completely including the ID. Please keep in mind, that with `trice insert` as a pre-compile and `trice clean` as post-compile step, the user all the time sees only the original written code:

```C
void doStuff( void ){
    // ...
    trice("info:The answer is %d.\n", 42);
    // ...
}
```

The optional `-cache` switch makes things blazing fast.

The appropriate Trice tool log line output would be similar to

```bash
{...}
{"level":"info","file":"val.c","line":"321","taskID":"0123","fmt":"The answer is 42.","uptime":"12345678 us"}
{...}
```

When *CEF* and *CEV* are empty strings (default), `trice insert` and `trice clean` commands will work the ususal way. If they are not empty, the `trice insert` command will on each Trice statement use a heuristic to check if the context information was inserted already and update it or otherwise insert it. **ATTENTION:** That will work only, if *CEF* and *CEV* where not changed by the user inbetween. In the same way `trice clean` would remove the context information only, if *CEF* and *CEV* kept unchanged. If the user wants to change *CEF* and *CEV* during development, first a `trice clean` is needed. Use a `build.sh` script like this:

```bash
#!/bin/bash

# Run "rm -rf ~/.trice/cache/*" automatically after changing this file !!! 

CEF='{"level":"$level","file":"$file","line:"$line","taskID":"%04x","fmt":$fmt,"uptime":%08u us"}'
CEV='getTaskID(), $values, uptime()'

trice insert -cache -cef="$CEF" -cev="$CEV"
# make
trice clean  -cache -cef="$CEF" -cev="$CEV"
```

The `-cache` switch is still experimental - to stay safe, use (here again with `$line` as string):

```bash
#!/bin/bash
CEF='{"level":"$level","file":"$file","line:"$line","taskID":"%04x","fmt":$fmt,"uptime":%08u us"}'
CEV='getTaskID(), $values, uptime()'

trice insert -cef="$CEF" -cev="$CEV"
# make
trice clean  -cef="$CEF" -cev="$CEV"
```

#### 45.2.8. <a id="trice-context-enrichment-level-specific-configuration"></a>Trice Context Enrichment Level Specific Configuration

Configure the Trice Context Enrichment selectively in a way, to provide as much helpful diagnostic info as possible on `ERROR` level for example. Example script:

```bash
#!/bin/bash

# Specify `-cef` and `-cev` differently for different channels/tags.

STL="" # Trice Context Enrichment configuration

# Trices with an `ERROR:` tag `trice("err:...", ...);`:
CEF_ERROR='ERROR:{"log level":"%-6s","file":"%24s","line:"%5d","func":"%-16s","taskID":"%x","fmt":"$fmt","uptime":"%08u us"}'` # (with location)
CEV_ERROR='ERROR:$level, $file, $line, $func, getTaskID(), $values, uptime()'`
STL+=" -cef $CEF_ERROR -cev $CEV_ERROR "

# Trices with an underscore tag, like `trice("_DEBUG:...", ...);` or `trice("_info:...", ...);`:
CEF_underscoreTagStart='_*:{"log level":"%-6s","file":"%24s","line:"%5d","func":"%-16s","fmt":"$fmt","uptime":"%08u us"}'` # (no task ID)
CEV_underscoreTagStart='_*:$level, $file, $line, $func, $values, uptime()'`
STL+=" -cef $CEF_underscoreTagStart -cev $CEV_underscoreTagStart "

# Tices with any other tag:
CEF_anyTag='*:{"log level":"%-6s","file":"%24s","line:"%5d","func":"%-16s","fmt":"$fmt"}'` # (no task ID, no uptime)
CEV_anyTag='*:$level, $file, $line, $func, $values'`
STL+=" -cef $CEF_anyTag -cev $CEV_anyTag "

# Trices with no tag at all:
CEF_noTag='{"file":"%24s","line:"%5d","fmt":"$fmt"}'` # (only location information)
CEV_noTag='$file, $line, $values'`
STL+=" -cef $CEF_noTag -cev $CEV_noTag "

trice insert $STL ...
source make.sh # build process
trice clean  $STL ...
```

#### 45.2.9. <a id="trice-context-enrichment-assert-macros-todo"></a>Trice Context Enrichment Assert Macros (TODO)

Configure `TriceAssert` like macros and this works also with the `-salias` switch.
-->


<!--

##### 🧩 Visual Architecture Diagram — Trice .github Automation System

```pgsql
                                       ┌───────────────────────────────────────┐
                                       │               GitHub UI               │
                                       │  (Issues, Pull Requests, Actions)     │
                                       └───────────────────┬───────────────────┘
                                                           │
                                                           ▼
                                       ┌───────────────────────────────────────┐
                                       │               .github/                │
                                       │  Project automation & CI/CD settings  │
                                       └───────────────────┬───────────────────┘
                                                           │
     ┌─────────────────────────────────────────────────────┼──────────────────────────────────────────────────┐
     │                                                     │                                                  │
     ▼                                                     ▼                                                  ▼
┌────────────────┐                                 ┌───────────────────┐                          ┌──────────────────────────┐
│ ISSUE_TEMPLATE │                                 │   FUNDING.yml     │                          │        labeler.yml       │
│  bug/feature   │                                 │ Sponsor settings  │                          │ Automatic PR labelling   │
└───────┬────────┘                                 └───────────────────┘                          └───────────┬──────────────┘
        │                                                                                                     │
        ▼                                                                                                     ▼
┌────────────────┐                                                                                ┌─────────────────────────┐
│ New Issue form │   Contributors create issues → GitHub loads templates                          │ Labels added to PRs     │
│ Guided inputs  │──────────────────────────────────────────────────────────────────────────────► │ based on files & title  │
└────────────────┘                                                                                └─────────────────────────┘



───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
                                   GITHUB ACTIONS WORKFLOWS (.github/workflows/)
───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────

                                           (Triggered by pushes, PRs, tags, or manually)
        ┌─────────────────────────┬────────────────────────┬─────────────────────────┬──────────────────────────────┐
        │                         │                        │                         │                              │
        ▼                         ▼                        ▼                         ▼                              ▼
┌────────────────────┐    ┌────────────────────┐   ┌──────────────────┐   ┌────────────────────────┐   ┌──────────────────────┐
│     go.yml         │    │   codeql.yml       │   │ superlinter.yml  │   │    goreleaser.yml      │   │     stale.yml        │
│ Build & test Go    │    │ Security scanning  │   │ Linting of code  │   │ Build+release pipeline │   │ Auto-close inactive  │
│ on every push/PR   │    │ for vulnerabilities│   │ for consistency  │   │ for multi-platform     │   │ issues & PRs         │
└─────────┬──────────┘    └──────────┬───────V─┘   └──────────┬───────┘   └────────────┬───────────┘   └───────────┬──────────┘
          │                          │                        │                        │                           │
          ▼                          ▼                        ▼                        ▼                           ▼
┌────────────────┐     ┌───────────────────────┐   ┌────────────────────┐   ┌──────────────────────────┐   ┌─────────────────────┐
│ CI test result │     │ Security report       │   │ Linter annotations │   │ Build artifacts (dist/)  │   │ Issues marked stale │
│ pass/fail      │     │ shown in Security tab │   │ shown in PR checks │   │ GitHub Release published │   │ Closed after timeout│
└────────────────┘     └───────────────────────┘   └────────────────────┘   └──────────────────────────┘   └─────────────────────┘

───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
                                      COMMUNITY AUTOMATION (.github/workflows/)
───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────

                             ┌───────────────────────┬────────────────────────┐
                             │                       │                        │
                             ▼                       ▼                        ▼
                    ┌─────────────────┐     ┌──────────────────┐     ┌─────────────────────────┐
                    │ greetings.yml   │     │ manual.yml       │     │ learn-github-actions.yml│
                    │ Welcome message │     │ Run tasks manually│    │ Example workflow        │
                    │ for new PR/issue│     │ on demand        │     │ for contributors        │
                    └─────────────────┘     └──────────────────┘     └─────────────────────────┘

───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
                                   SUPPORTING FILES (.github/properties/, icons/)
───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────

                       ┌──────────────────────────┐       ┌────────────────────────────┐
                       │ properties/*.json        │       │ icons/* (e.g., go.svg)     │
                       │ Metadata for workflows   │       │ Used in badges or UI       │
                       │ (category, permissions)  │       │ decorations in README      │
                       └──────────────────────────┘       └────────────────────────────┘

───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
                                              GITHUB ACTIONS OUTPUT FLOW
───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────


                                               User pushes commit / PR / tag
                                                       │
                                                       ▼
                                               GitHub triggers matching workflows
                                                       │
                                                       ▼
                                               Workflows run in parallel:
                                               - Build & test
                                               - Security scan
                                               - Linting
                                               - Release packaging
                                               - Labeling PRs
                                               - Greetings
                                               - Stale handling
                                                       │
                                                       ▼
                                               Results appear in:
                                               - Pull request checks
                                               - Security dashboard
                                               - GitHub Releases page
                                               - Automated comments and labels
```

-#### Automatic Pull Request Labeling in Trice

Trice uses GitHub’s **Labeler** workflow to automatically assign labels to pull requests.  
These labels help maintainers and contributors quickly understand what a PR affects, without having to inspect every file manually.

The labeling is triggered by two factors:

1.  **Which files were changed** (file-based labeling)
    
2.  **What is written in the PR title or description** (text-based labeling)
    

This system ensures consistent categorization and improves the review workflow.

```sql
                           ┌───────────────────────────┐
                           │   Contributor opens a     │
                           │      Pull Request         │
                           └──────────────┬────────────┘
                                          │
                                          ▼
                          ┌─────────────────────────────────┐
                          │ GitHub Action: "Labeler" starts │
                          │ (.github/labeler.yml)           │
                          └─────────────────┬───────────────┘
                                            │
                      ┌─────────────────────┼─────────────────────┐
                      │                     │                     │
                      ▼                     ▼                     ▼
        ┌────────────────────┐  ┌──────────────────────┐ ┌──────────────────────┐
        │ File-based rules   │  │ Title-based rules    │ │ Body-based rules     │
        │(changed-files)     │  │(contains keywords)   │ │(contains keywords)   │
        └──────────┬─────────┘  └───────────┬──────────┘ └──────────┬───────────┘
                   │                        │                       │
                   ▼                        ▼                       ▼
       ┌───────────────────┐     ┌───────────────────┐     ┌───────────────────┐
       │ Example outputs:  │     │ Example outputs:  │     │ Example outputs:  │
       │  go, c, docs, ci  │     │  fix, feature     │     │  fix, feature     │
       └──────────┬────────┘     └──────────┬────────┘     └──────────┬────────┘
                  │                         │                         │
                  └──────────────┬──────────┴──────────────┬──────────┘
                                 │                         │
                                 ▼                         ▼
                        ┌────────────────────────────────────────┐
                        │ Labels are attached to the PR          │
                        │ automatically within seconds           │
                        └────────────────────────────────────────┘
```

* * *

##### File-based labels

GitHub automatically applies specific labels depending on which files a pull request changes.  
Below is an overview of the most relevant label categories.

###### **`AnyChange`**

Applied to **every** pull request that modifies at least one file.  
Useful as a catch-all label for triggers or filtering.

* * *

###### **`go`**

Applied when the PR changes any Go source file:

`**/*.go`

This marks PRs that affect Go code, modules, or logic.

* * *

###### **`c`**

Applied when the PR touches C or header files inside:

-   `src/`
    
-   `examples/`
    
-   `_test/`
    

Examples:

`src/foo.c examples/demo.h _test/bar.c`

This helps identify changes relevant to embedded/low-level components.

* * *

###### **`docs`**

Applied when documentation is modified, including:

-   any file in `docs/`
    
-   any Markdown file (`*.md`) anywhere in the repo
    

This is helpful for changes affecting documentation only.

* * *

###### **`tests`**

Applied when the PR updates or adds tests:

-   Go test files (`*_test.go`)
    
-   any files inside the `_test/` directory
    

* * *

###### **`ci`**

Applied when the PR modifies continuous integration or GitHub configuration files:

`.github/**`

This includes workflow files, templates, automation settings, and metadata.

* * *

##### Text-based labels

Some labels are applied based on **keywords** in the pull request title or description.

###### **`fix`**

Applied when the PR title or body contains the keyword:

`fix`

This is useful when PRs follow conventional commit messages (e.g., `fix: prevent overflow`).

* * *

###### **`feature`**

Applied when the title or description contains:

`feat feature`

This marks PRs that introduce new functionality.

* * *

##### Benefits of automatic labeling

-   **Faster reviews**: reviewers instantly see what areas of the project are affected.
    
-   **Better filtering**: maintainers can filter for categories such as documentation, CI, features, etc.
    
-   **Consistent classification**: no need for contributors to manually apply labels.
    
-   **Supports larger workflows**: labels can trigger additional automations, such as notifications or CI behavior.
    

* * *

##### How contributors can help

Contributors can ensure correct labeling by:

-   writing clear PR titles (e.g., `fix: handle invalid input`)
    
-   using structured commit messages
    
-   modifying files within appropriate directories
    

Labels will be applied automatically within seconds after opening or updating a pull request.

-->


<!--

* The information, if the stream is aligned or not can be passed wit `-pf=none32` or `-pf=none8` but is also detectable.
* In multi-pack mode only unaligned  

* The Trice tool, when receiving the transfer buffers, knows the framing and also if encryption is active, but does not know if TRICE_SINGLE_PACK_MODE or TRICE_MULTI_PACK_MODE is active. Additionally the information direct or deferred is not used by the Trice tool. It has to deal with the option of 0-7 padding zeroes after a decoded Trice message:
  * Encryption with framing NONE: forbidden - uninteresting case and resync is difficult
  * Encryption with framing COBS: 0-7 padding zero bytes inside the decoded buffer only at its end possible.
  * Encryption with framing TCOBS: 0-7 padding zero bytes inside the decoded buffer only at its end possible. This configuration is not recommended, because random data not compressble.
  * No Encryption with framing NONE: 0-3 padding zero bytes after each Trice possible.
  * No Encryption with framing COBS: 0-3 padding zero bytes inside the decoded buffer only at its end possible.
  * No Encryption with framing TCOBS: 0-3 padding zero bytes inside the decoded buffer only at its end possible.
  * *In short*:
    * Only inside at package end: 0-7 padding zeroes with encryption and 0-3 without encryption are possible.
    * When package framing NONE 0-3 padding zeroes possible and typeX0 records are mixed with normal Trices.
  * Usually, when transmitting over UART unencrypted for example, there are no padding bytes at all.
  * But with `TRICE_LEAVE` is called `TriceNonBlockingDirectWrite`, what could add padding bytes inside the (T)COBS buffers at their end.
  * In deferred mode, padding bytes inside a (T)COBS package only possible together with encryption. After packing and when using a 32-bit write function, after (outside) the packages are 1-3 zero bytes possible. Those are treated as package delimiters.
* The further transfer buffer interpretation after successfully decoding one Trice is:
  * No encryption:
    * If framing (T)COBS or NONE and at least 4 bytes left: Try to interpret next bytes.
    * If framing (T)COBS and max 3 bytes left:
      * If all 3 are zero: these are padding bytes to be removed before next package is read.
      * If at least one of the 3 remaining bytes is != 0, this is an error.
    * If framing (T)COBS and the 4 bytes are not a full Trice -> error
    * If framing NONE and the 4 bytes are not a full Trice -> read more
      * Even if we get more, we do not know, if there are 0-4 padding bytes before the next Trice starts. Cases:

        ```C
        d n n n n // case  1:                                start of next Trice is n n

        d 0 n n n // case  2: 0 is         padding byte  and start of next Trice is n n
        d 0 n n n // case  3: 0 is no      padding byte  and start of next Trice is 0 n

        d 0 0 n n // case  4: 0 0 are      padding bytes and start of next Trice is n n
        d 0 0 n n // case  5: first 0 is   padding byte  and start of next Trice is 0 n
        d 0 0 n n // case  6: no           padding bytes and start of next Trice is 0 0 (error)

        d 0 0 0 n // case  7: 0 0 0 are    padding bytes and start of next Trice is n
        d 0 0 0 n // case  8: 0 0   are    padding bytes and start of next Trice is 0 n
        d 0 0 0 n // case  9: 0     is     padding byte  and start of next Trice is 0 0 (error)
        d 0 0 0 n // case 10: no           padding bytes and start of next Trice is 0 0 (error)
        ```

      * The padding bytes positions must fit the ByteCount. But even they fit, cases 2 & 3, 4 & 5, 7 & 8 are not distinguishable.
      * Also the error cases could by interpreting one or two zeroes as padding byte get valid cases.
      * For a consistent interpretation we need to know if padding is used. That is a use case especially when using RTT8 or RTT32.
      * Is it possible to detect that automatically for `pf=none`? As soon we have case 2 or 3 and cannot distinguish, there is a high probalbility that one of them will fail. Then we silently know for the current Trice tool life time.
      * We could invent additional CLI switches `-pf=none8` and `pf=none32` to tell explicitely if package framing none is with padding or not.
    * We invent a global variable `NopfPadding` witch we set to 0 with `-pf=none`, to 8 with `pf=none8` and 32 with `pf=none32`.
    * With NopfPadding != 0 we know exactly how to interpret framing NONE streams.
    * With NopfPadding == 0 we check the ByteCount and if ByteCount mod 4 != 0 and there are no matching zeroes afterwards we set NopfPadding = 8.
    * With NopfPadding == 0 we check the ByteCount and if ByteCount mod 4 != 0 and there are    matching zeroes afterwards we try to interpret the variants and set NopfPadding = 8||32 according to the success.

    * If framing NONE and max 3 bytes left:
        * If no more data within 100ms, try to interpret them as typeX0 message and report an error if no success.
        * If more data arrive, the Trice tool has to determine the correct count of padding bytes.** That can be easily done with the already interpreted byte count **ByteCount**.
          * BC mod 4 == 0 -> no padding bytes
          * BC mod 4 == 1 -> 1 padding byte, which is expected to be 0.
          * BC mod 4 == 2 -> 2 padding bytes, which are expected to be 0.
          * BC mod 4 == 3 -> 3 padding bytes, which are expected to be 0.

  * With encryption:
    * If framing (T)COBS or NONE and at least 8 bytes left: Try to interpret next bytes.
    * If framing (T)COBS and max 7 bytes left:
      * If all 7 are zero: these are padding bytes to be removed before next package is read.
      * If at least one of the 7 remaining bytes is != 0, this is an error.
    * If framing NONE: forbidden situation

-->


<!---
When introducing or changing X0 behavior, a useful implementation sequence is:

1. Add or adjust the X0 test in one test configuration and verify that it fails for the missing behavior.
2. Implement or update `-typeX0` in the Go decoder.
3. Make the single X0 test pass.
4. Enable `TRICE_TX_X0_COUNTED_BUFFER_SUPPORT` in all relevant `_test/.../triceConfig.h` files.
5. Add or update the shared `decoder.TypeX0` option for all tests using `triceCheck.c`.
6. Run the full `_test/` matrix.
-->


<!--
### Remote function call syntax support with triceF (deprecated)

```diff
-> Do not use `triceF` family macros for new projects! 
```
The `triceF` macros were an experimental remote-function-call syntax. They are deprecated and should not be used for new designs. New command-style communication between devices should use [Trice ABC - Asynchronous Broadcast Commands](#trice-abc---asynchronous-broadcast-commands).

> The `TRICE8_F`, `TRICE16_F`, `TRICE32_F`, `TRICE64_F`, macros expect a string without format specifiers which is usable later as a function call. Examples:
> 
> ```C
> trice8F(   "call:FunctionNameW", b8,  sizeof(b8) /sizeof(int8_t) );   //exp: time:            default: call:FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
> TRICE16_F( "info:FunctionNameX", b16, sizeof(b16)/sizeof(int16_t) );  //exp: time: 842,150_450default: info:FunctionNameX(0000)(ffff)(fffe)(3344)
> TRice16F(  "call:FunctionNameX", b16, sizeof(b16)/sizeof(int16_t) );  //exp: time: 842,150_450default: call:FunctionNameX(0000)(ffff)(fffe)(3344)
> Trice16F(  "call:FunctionNameX", b16, sizeof(b16)/sizeof(int16_t) );  //exp: time:       5_654default: call:FunctionNameX(0000)(ffff)(fffe)(3344)
> trice16F(  "call:FunctionNameX", b16, sizeof(b16)/sizeof(int16_t) );  //exp: time:            default: call:FunctionNameX(0000)(ffff)(fffe)(3344)
> TRICE32_F( "info:FunctionNameY", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time: 842,150_450default: info:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)
> TRice32F(  "call:FunctionNameY", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time: 842,150_450default: call:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)
> Trice32F(  "call:FunctionNameY", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time:       5_654default: call:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)
> trice32F(  "call:FunctionNameY", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time:            default: call:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)
> TRICE64_F( "info:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time: 842,150_450default: info:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
> TRice64F(  "call:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time: 842,150_450default: call:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
> Trice64F(  "call:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time:       5_654default: call:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
> trice64F(  "call:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time:            default: call:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
> ```
> 
> The Trice tool displays the parameter buffer in the shown manner. There is a [Generating an RPC Function Pointer List (deprecated)](#generating-a-rpc-function-pointer-list-deprecated), which generates mainly a function pointer list with associated IDs. This list can get part of the > source code of a remote device. Then, when receiving a Trice message, the remote device can execute the assigned function call using the transferred parameters. This way several devices can communicate in an easy and reliable way.
> 
> With `#define TRICE_F TRICE16_F` in the project specific _triceConfig.h_ file the user can specify which should be the bitwidth (16 in this example) for `triceF` macros. The default value is 8.
> 
> **Hint:** If you add for example `"rpc"` as [tag](#explpore-and-modify-tags-and-their-colors) and call `trice log -ban "rpc"`, the Trice tool will not display the RPC Trices, but all others. That could be helpful, if you have frequent RPCs and do not > wish to spoil your log output with them.
> 
> * Future extensions are possible:
>   * `triceD( "dump:32", addr, 160 );` -> The Trice tool dumps in 32 byte rows.
>   * An appropriate syntax is needed.
-->


<!--
### Generating an RPC Function Pointer List (deprecated)

```diff
-> Do not use for new projects!
```

This was an experimental implementation and will be removed in the future. Use `trice generate -abc=<device>` instead. See [Trice ABC - Asynchronous Broadcast Commands](#trice-abc---asynchronous-broadcast-commands).

> When several embedded devices are going to communicate, `trice generate -rpcH -rpcC` could be helpful.
> 
> You will get 2 files similar to:
> 
> ```C
> //! \file tilRpc.h
> //! ///////////////////////////////////////////////////////////////////////////
> 
> //! Trice generated code - do not edit!
> 
> #include <stdint.h>
> 
> typedef void (*triceRpcHandler_t)(void* buffer, int count);
> 
> typedef struct{
>     int id;
>     triceRpcHandler_t fn;
> } triceRpc_t;
> 
> extern triceRpc_t triceRpc[];
> extern int triceRpcCount;
> 
> /*  TRICE16_F */ void FunctionNameXa( int16_t* p, int cnt );
> /*  TRICE32_F */ void FunctionNameYa( int32_t* p, int cnt );
> /*   TRICE8_F */ void TryoutBufferFunction( int8_t* p, int cnt );
> /*   TRice16F */ void FunctionNameXb( int16_t* p, int cnt );
> /*   trice32F */ void FunctionNameYd( int32_t* p, int cnt );
> /*    TRice8F */ void FunctionNameWb( int8_t* p, int cnt );
> /*   trice64F */ void FunctionNameZd( int64_t* p, int cnt );
> /*   Trice16F */ void ARemoteFunctionName( int16_t* p, int cnt );
> /*    Trice8F */ void FunctionNameWc( int8_t* p, int cnt );
> /*   Trice16F */ void FunctionNameXc( int16_t* p, int cnt );
> /*   TRICE8_F */ void TryoutStructFunction( int8_t* p, int cnt );
> /*   Trice64F */ void FunctionNameZc( int64_t* p, int cnt );
> /*    trice8F */ void FunctionNameWd( int8_t* p, int cnt );
> /*   TRice64F */ void FunctionNameZb( int64_t* p, int cnt );
> /*  TRICE64_F */ void FunctionNameZa( int64_t* p, int cnt );
> /*   trice16F */ void FunctionNameXd( int16_t* p, int cnt );
> /*   TRICE8_F */ void FunctionNameWa( int8_t* p, int cnt );
> /*   TRice32F */ void FunctionNameYb( int32_t* p, int cnt );
> /*   Trice32F */ void FunctionNameYc( int32_t* p, int cnt );
> 
> // End of file
> 
> ```
> 
> ```C
> //! \file tilRpc.c
> //! ///////////////////////////////////////////////////////////////////////////
> 
> //! Trice generated code - do not edit!
> 
> #include <stdio.h> // needed for __attribute__((weak)) 
> #include "tilRpc.h"
> 
> //! triceRpc contains all rpc IDs together with their function pointer address.
> const triceRpc_t triceRpc[] = {
> 	/* Trice type */  //  id, function pointer
> 	/*    TRice8F */ { 14227, FunctionNameWb },
> 	/*  TRICE32_F */ { 14234, FunctionNameYa },
> 	/*   TRICE8_F */ { 16179, TryoutBufferFunction },
> 	/*   Trice16F */ { 14232, FunctionNameXc },
> 	/*   Trice64F */ { 14240, FunctionNameZc },
> 	/*   TRice64F */ { 14239, FunctionNameZb },
> 	/*  TRICE16_F */ { 14230, FunctionNameXa },
> 	/*   TRICE8_F */ { 16178, TryoutStructFunction },
> 	/*    Trice8F */ { 14228, FunctionNameWc },
> 	/*   trice16F */ { 14233, FunctionNameXd },
> 	/*   trice64F */ { 14241, FunctionNameZd },
> 	/*   trice32F */ { 14237, FunctionNameYd },
> 	/*   TRICE8_F */ { 14226, FunctionNameWa },
> 	/*   TRice16F */ { 14231, FunctionNameXb },
> 	/*   TRice32F */ { 14235, FunctionNameYb },
> 	/*   Trice16F */ { 16337, ARemoteFunctionName },
> 	/*    trice8F */ { 14229, FunctionNameWd },
> 	/*   Trice32F */ { 14236, FunctionNameYc },
> 	/*  TRICE64_F */ { 14238, FunctionNameZa }
> };
> 
> //! triceRpcListElements holds the compile time computed count of list elements.
> const unsigned triceRpcElements = sizeof(triceRpc) / sizeof(triceRpc_t);
> 
> void TryoutBufferFunction( int8_t* p, int cnt) __attribute__((weak)) {}
> void FunctionNameXc( int16_t* p, int cnt) __attribute__((weak)) {}
> void FunctionNameZc( int64_t* p, int cnt) __attribute__((weak)) {}
> void FunctionNameZb( int64_t* p, int cnt) __attribute__((weak)) {}
> void FunctionNameXa( int16_t* p, int cnt) __attribute__((weak)) {}
> void TryoutStructFunction( int8_t* p, int cnt) __attribute__((weak)) {}
> void FunctionNameWc( int8_t* p, int cnt) __attribute__((weak)) {}
> void FunctionNameXd( int16_t* p, int cnt) __attribute__((weak)) {}
> void FunctionNameZd( int64_t* p, int cnt) __attribute__((weak)) {}
> void FunctionNameYd( int32_t* p, int cnt) __attribute__((weak)) {}
> void FunctionNameWa( int8_t* p, int cnt) __attribute__((weak)) {}
> void FunctionNameXb( int16_t* p, int cnt) __attribute__((weak)) {}
> void FunctionNameYb( int32_t* p, int cnt) __attribute__((weak)) {}
> void ARemoteFunctionName( int16_t* p, int cnt) __attribute__((weak)) {}
> void FunctionNameWd( int8_t* p, int cnt) __attribute__((weak)) {}
> void FunctionNameYc( int32_t* p, int cnt) __attribute__((weak)) {}
> void FunctionNameZa( int64_t* p, int cnt) __attribute__((weak)) {}
> void FunctionNameWb( int8_t* p, int cnt) __attribute__((weak)) {}
> void FunctionNameYa( int32_t* p, int cnt) __attribute__((weak)) {}
> 
> // End of file
> ```
> 
> Assume a project with several devices. You can add these 2 files to all targets and if a special target should execute any functions, simply implement them. These functions on their own can execute other Trice statements to transmit results. If a client > executes an RPC function this way, the request is transmitted with the Trice speed. Several target devices (servers) can receive and respond and the client can wait for the first or some of them. That server receiving and client waiting functionality is not > part of the Trice library. 
-->

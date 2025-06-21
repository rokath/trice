# Trice Structured Logging (Specification Draft)

Structured logging, in contrast to unformatted logging, adds compile time and runtime information to logs as well as log level information. These data are added automatically to logs. The user should be able to configure, which data get added and also should get control about the data formatting. The generic data insertion allows later an automatic log file analysis and frees the developer from manually typing this, what is also error-prone.

Trice is considerable already a bit as a (very limited) structured logger, if we look at the file and line insertion capability and the timestamp options. The following is about how the Trice functionality could get extended with structured logging capability without making a breaking change.

## Compile-time Information Examples

*file, line, function, compiler version, module, build time, firmware version, machine name, user name, locale, host OS version, log level, fmt, ...*

These data can be strings or numbers.

## Runtime Information Examples

*uptime, timestamp, hw serial, task ID, stack depth, event count, core ID, position, ...*

In an initial approach we assume, these data do not contain runtime generated strings. If really needed, a derived hash is usable instead.

## Trice Limitations and Special Cases

For performance reasons, Trice was designed to only transmit 0-12 (extendable) numbers of equal bit-width **OR** a single runtime generated string. Firstly we look at only "normal" Trice macros `trice`, `Trice`, `TRice` and exclude the special cases `triceS`, `TriceS`, `TRiceS`. Also we consider just trices without specified bit-width, assume 32-bit and exlude cases like `trice32_4` firstly.

## An Example

User may have written inside *val.c*:

```C
void doStuff( void ){
    // ...
    trice("wrn:MyF=%f, myI=%d", aFloat(4.2), 42);
    // ...
}
```

and (as we know) a `trice insert` command would change that into:

```C
void doStuff( void ){
    // ...
    trice(iD(123), "wrn:MyF=%f, myI=%d", aFloat(4.2), 42);
    // ...
}
```

But a `trice insert` command with context option will, for example, change that line into:

```C
void doStuff( void ){
    // ...
    trice(iD(456), "[log level=wrn][file=\"val.c\"][line=321][function=doStuff][taskID=%x][fmt=\"MyF=%f, myI=%d\"][uptime=%08us]\n", getTaskID(), aFloat(4.2), 42), uptime()) ;
    // ...
}
```

Use these structured logging CLI switches on `trice insert` and `trice clean` (with example values):

```bash
trice insert \
-sLogF="[log level=$channel][file=$file][line=$line][function=$function][taskID=%x][fmt=$fmt][uptime=%08us]\n" \
-sLogV=", getTaskID(), $values, uptime()"
```

It is important to have the colons at the right place to get a correct result after the string concatenation. 

The user has full control and could also use a JSON format. Only the format specifiers are requested to match the passed values, so that the Trice tool can perform a printf.

Adding values like `$line` as strings has performance advantages, but on each such value change is a new Trice ID generated then. Those variables are better inserted as values like this:

```bash
trice insert \
-sLogF="[log level=$channel][file=$file][line=%d][function=$function][taskID=%x][fmt=$fmt][uptime=%08us]\n" \
-sLogV=", $line, getTaskID(), $values, uptime()"
```

Or the user decides to include the normal location information of the Trice tool (`-li` switch).

To achieve a log output in compact JSON *sLogV* is the same, but change the *sLogF*:

```bash
trice insert \
-sLogF=`{"log level":"$channel","file":"val.c","line:"%d","function":"$function","taskID":"%x","fmt":"MyF=%f, myI=%d","uptime":%08u us"}\n` \
-sLogV=", $line, getTaskID(), $values, uptime()"
```

The *sLogF* is now given as [raw string literal](https://go.dev/ref/spec#String_literals) to avoid `\"` all the time. 

Before inserting, the Trice tool will replace the following Trice tool specific variables:

| Variable    | Example             | Comment                                                                                                                                      |
|-------------|---------------------|----------------------------------------------------------------------------------------------------------------------------------------------|
| `$channel`  | `wrn`               | The bare trice format string part until the first colon (`:`), if known as channel value. In the example it is `wrn`.                        |
| `$filename` | `val.c`             | The file name, where the Trice log occures.                                                                                                  |
| `$line`     | `321`               | The file line, where the Trice log occures.                                                                                                  |
| `$function` | `doStuff`           | The function name, where the Trice log occures.                                                                                              |
| `$fmt`      | `MyF=%f, myI=%d`    | The bare Trice format string stripped from the channel specifier including the colon (`:`) according to the Trice rule (lowercase-only ones) |
| `$values`   | `, aFloat(4.2), 42` | The bare Trice statement values.                                                                                                             |

After `trice insert` a log line as compact JSON would look in **C** like

```C
void doStuff( void ){
    // ...
    trice(iD(789), "{\"log level\":\"wrn\",\"file\":\"val.c\",\"line\":\"%d\",\"function\":\"doStuff\",\"taskID\":\"%x\",\"fmt\":\"MyF=%f, myI=%d\",\"uptime\":\"%08u us\"}\n', $line, getTaskID(), aFloat(4.2), 42), uptime());
    // ...
}
```

A `trice clean` command will remove the context information completely including the ID. Please keep in mind, that with `trice insert` as a pre-compile and `trice clean` as post-compile step, the user all the time sees only the original written code

```C
void doStuff( void ){
    // ...
    trice("wrn:MyF=%f, myI=%d", aFloat(4.2), 42);
    // ...
}
```

and the optional `-cache` switch makes things blazing fast.

The appropriate Trice tool log line output would be

```bash
{...}
{"log level":"wrn","file":"val.c","line":"321","function":"doStuff","taskID":"0x123","fmt":"MyF=4.2000, myI=42","uptime":"12345678 us"}
{...}
```

When *sLogF* and *sLogV* are empty strings (default), `trice insert` and `trice clean` commands will work the ususal way. If they are not empty, the `trice insert` command will on each Trice statement use a heuristic to check if the context information was inserted already and update it or otherwise insert it. **ATTENTION:** That will work only if *sLogF* and *sLogV* where not changed by the user inbetween. The same way `trice clean` would remove the context information only, if *sLogF* and *sLogV* kept unchanged. If the user wants to change *sLogF* and *sLogV* during development, first a `trice clean` is needed. Use a `build.sh` script like this:

```bash
#!/bin/bash

# Run "rm -rf ~/.trice/cache/*", after modifying $SLFMT and SLVAL !!!

SLFMT='{"log level":"$channel","file":"main.c","line:"%d","function":"$function","taskID":"%x","fmt":"$fmt","uptime":"%08u us"}\n'
SLVAL=', $line, getTaskID(), $values, uptime()'

trice insert -cache -sLogF="$SLFMT" -sLogV="$SLVAL"
# make
trice clean  -cache -sLogF="$SLFMT" -sLogV="$SLVAL"
```

The `-cache` switch is still experimental - to stay safe use:

```bash
#!/bin/bash
SLFMT='{"log level":"$channel","file":"main.c","line:"%d","function":"$function","taskID":"%x","fmt":"$fmt","uptime":"%08u us"}\n'
SLVAL=', $line, getTaskID(), $values, uptime()'

trice insert -sLogF="$SLFMT" -sLogV="$SLVAL"
# make
trice clean  -sLogF="$SLFMT" -sLogV="$SLVAL"
```

## Questions 

* Probably only one new CLI switch `-sLog` is better and the Trice tool splits the strings internally.
* Should all Trices get handled the same way? Or should we invent a new `strice` and apply `-sLogF` and `-sLogV` only to them?
  * Invent new channels starting with an underscore `_` like "_WRN" just for the structured logging.
* How to deal with an existing or none existing `\n` at the format strings end?
  * There is already a switch `-addNL`, but this has its own application area.
  * We could have a switch `-rmNL`, which removes `\n` from the end of legacy format strings during `trice insert`, if wanted.
* It is also possible to specify `-sLog` differently for different channels. For example as multi switch:
  * `-slog='_ERR:"{"log_level":"$channel","file":"main.c","line:"%d","taskID":"%x","fmt":"$fmt"}\n":", $line, getTaskID(), $values "'` (with location)
  * `-slog='_*:"{"log_level":"$channel","taskID":"%x","fmt":"$fmt"}\n":", getTaskID(), $values "'` (no location)
  
  This would give structure data including file and line for _ERR messages and excluding file and line for _INFO, _WRN and _DEBUG logs and would not touch any other trice logs.
* The star `_*` is used instead of _INFO, _WRN and _DEBUG to let all remaining levels/channels have identical structured data.
* Is is necessary to keep the users original format strings or can we reconstruct them all the time?
  * When using the `-cache` option we have a copy of the original, but else not. 


<!--

for example according to a configuration file.

    A trice insert -runtimeContext command would only add the runtime information trice(iD(124), "wrn:<taskID:%d>MyValueA %d, myValue %d", getTaskID(), 11, 22);

    The compile time information goes only into a compileTimeContext.json file, similar to file and line already now inside li.json and is displayed by the Trice tool according to the configuration.

    So a triceConfiguration.json file could get specified, where the user selects, which compile time and runtime information should be added, how to obtain it and also how this information should look like.

    Example: TRice("MSG:day is %d\n", Val); -> with trice insert -fullContext this gets:

    trice(200), "context: [hw=%x] [core=%x] [log level=MSG] [fn=main] [build time=2025-06-10_12:34:56] [weekday is %d]\n", getHwSerial(), getCoreID(), Val);

Runtime generated strings need their own separate Trice. triceS("name=%20s\n", sVal); type Trice logs can get compile time context added by format string extension, but not get runtime context added. Also including runtime generated strings as runtime context in to "normal" Trice logs is not possible within the same Trice statement. How to handle that in a clean way?

Example: TRiceS("MSG:weekday is %10s\n", sVal); -> with trice insert -fullContext this gets:

trice(201), "runtimeContext: [hw=%x] [core=%x]", getHwSerial(), getCoreID()); TRiceS(iD(203), "compileTimeContext:[log level=MSG] [fn=main] [build time=2025-06-10_12:34:56] [weekday is %10s]\n", sVal);

-->

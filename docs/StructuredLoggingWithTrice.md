# Implementing structured logging as binary logging with Trice (Specification Draft)

Structured logging, in contrast to unformatted logging, adds compile time and runtime information to logs as well as log level information. These data are added automatically to logs. The user should be able to configure, which data get added and also should get control about the data formatting. The generic data insertion allows later an automatic log file analysis and frees the developer from manually typing this, what is also error-prone.

Trice is considerable already a bit as a (very limited) structured logger, if we look at the file and line insertion capability and the timestamp options. The following is about how the Trice functionality could get extended with structured logging capability without making a breaking change.

## Compile-time Information Examples

*file, line, function, compiler version, module, build time, firmware version, machine name, user name, locale, host OS version, log level, fmt, ...*

These data can be strings or numbers.

## Runtime Information Examples

*uptime, timestamp, hw serial, task ID, stack depth, event count, core ID, position, ...*

In a first approach we assume, these data do not contain dynamically generated strings.

## Trice Limitations and Special Cases

For performance reasons, Trice was designed to only transmit 0-12 (extendable) numbers of equal bit-width OR a single runtime generated string. In a first step we consider only "normal" Trice macros `trice`, `Trice`, `TRice` and exclude the special cases `triceS`, `TriceS`, `TRiceS`. Also we consider only trices without specified bit-width, assume 32-bit and exlude cases like `trice32_4` firstly.

## An Assumed Example

User may have wriiten:

```C
void initABC( void ){
    // ...
    trice("wrn:MyF %f, myI %d\n", aFloat(4.2), 42);
    // ...
}
```

and (as we know) a `trice insert` command would change that into:

```C
void initABC( void ){
    // ...
    trice(iD(123), "wrn:MyF %f, myI %d", aFloat(4.2), 42);
    // ...
```

But a `trice insert` command with context option will, for example, change that line into:

```C
void initABC( void ){
    // ...
    trice(iD(456), '[log level=wrn][file="main.c"][line=321][function=initABC][taskID=%d][fmt=MyF %f, myI %d"][uptime=%08us]\n', getTaskID(), aFloat(4.2), 42), uptime()) ;
    // ...
```

A `trice clean` command will remove the context information completely including the ID.

Use these CLI switches together with `trice insert` and `trice clean` (with example values):

* `-contextFmtString="[log level=$channel][file=$file][line=$line][function=$function][taskID=%d][fmt=$fmt][uptime=%08us]\n"`
* `-contextFmtValues=, getTaskID(), $parameters, uptime()`

The user has full control and could also use a JSON format. The Trice tool will replace the following Trice tool specific variables:

| variable name | replacement                                                                                                                                       |
| ------------- | ------------------------------------------------------------------------------------------------------------------------------------------------- |
| $channel      | The bare trice format string part until the first colon (`:`), if known as channel value. In the example it is `wrn`.                             |
| $filename     | The file name, where the Trice log occures.                                                                                                       |
| $line         | The file line, where the Trice log occures.                                                                                                       |
| $function     | The function name, where the Trice log occures.                                                                                                   |
| $fmt          | The bare Trice format string stripped from the channel specifier including the colon (`:`) according to the Trice rule (only lowercase-only ones) |
| $parameters   | The bare Trice statement parameters. In the example above: ", aFloat(42.0), 42"                                                                   |

When *contextFmtString* and *contextFmtValues* are empty strings (default) `trice insert` and `trice clean` commands will work the ususal way.
If they are not empty, the `trice insert` command will on each Trice statement use a heuristic to check if the context information was inserted already and update it or otherwise insert it. That will only work if *contextFmtString* and *contextFmtValues* where not changed inbetween. The same way `trice clean` would remove the context information only, if *contextFmtString* and *contextFmtValues* kept unchanged. If the user wants to change *contextFmtString* and *contextFmtValues* during development first a `trice clean` is needed.

Adding values like `__LINE__` as strings has performance advantages, but on each value change is a new Trice ID generated then. Those variables are better inserted as values like this:

* `-contextFmtString="[log level=$channel][file=$file][line=%d][function=$function][taskID=%d][fmt=$fmt][uptime=%08us]\n"`
* `-contextFmtValues=, __LINE__, getTaskID(), $parameters, uptime()`

Or the user decides to include the normal location information of the Trice tool (`li` switch).

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
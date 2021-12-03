# Handling

- For example change the legacy source code line

```c
printf( "msg: %d Kelvin\n", k );
```

- into

```c
TRICE( "msg: %d Kelvin\n", k );
```

- `trice update` (run it automatically in the tool chain) changes it to  

```c
TRICE( Id(12345), "msg: %d Kelvin\n", k );
```

- and adds the *ID 12345* together with *"msg: %d Kelvin\n"* into a **t**rice **I**D **l**ist, a [JSON](https://www.json.org/json-en.html) reference file named [til.json](../til.json).
- The *12345* is a random or policy generated ID not used so far.
- During compilation the `TRICE` macro is translated to only a *12345* reference and the variable *k*. The format string never sees the target.
- Than, running the embedded device let the **trice** tool receive with `trice log -p COM1` instead of a ordinary terminal program.
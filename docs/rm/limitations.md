# Limitations

<details markdown="1">
<summary>Contents</summary>

---

<!-- mdtoc -->

* [1. Permanent Limitations](#permanent-limitations)
  * [1.1. Limitation TRICE in TRICE not possible](#limitation-trice-in-trice-not-possible)
* [2. Current Limitations](#current-limitations)
  * [2.1. String Concatenation Within TRICE Macros Not Possible](#string-concatenation-within-trice-macros-not-possible)
  * [2.2. Limited Trice Parser Capabilities](#limited-trice-parser-capabilities)
  * [2.3. Special Care Demands](#special-care-demands)
    * [2.3.1. More than 12 printf parameters](#more-than-12-printf-parameters)
    * [2.3.2. Float Numbers](#float-numbers)
    * [2.3.3. Double numbers](#double-numbers)
    * [2.3.4. Runtime Generated Strings](#runtime-generated-strings)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

---

</details>

## 1. <a id="permanent-limitations"></a>Permanent Limitations

### 1.1. <a id="limitation-trice-in-trice-not-possible"></a>Limitation TRICE in TRICE not possible

* No-Good Example:

```C
int f0( void ){ TRICE( "msg:f0\n"); return 0; }
void f1( void ){ TRICE( "No; %d", f0() ); }
```

* This will compile normally but corrupt TRICE output.

The reason is: When f1() gets active, the "No" Trice header is created, than the f0() Trice is executed and afterwards the "No" Trice tail is written. This works well during compile time but causes a mismatch during runtime.

* Workaround:

```C
int f0( void ){ TRICE( "msg:f0\n"); return 0; }
void f1( void ){ int x = f0(); TRICE( "Yes: %d", x ); }
```

## 2. <a id="current-limitations"></a>Current Limitations

### 2.1. <a id="string-concatenation-within-trice-macros-not-possible"></a>String Concatenation Within TRICE Macros Not Possible

String concatenation within TRICE macros does not work. The reason lays inside the way the trice tool parser works:

```C
void f0( void ){ TRICE( "msg:" ## "Hello\n" ); } // ERROR!
```

To implement this would need to build a trice preprocessor or to run the C preprocessor first and to modify the preprocessor output with the trice tool. That would make things unneccessary complicate and fragile for now.

### 2.2. <a id="limited-trice-parser-capabilities"></a>Limited Trice Parser Capabilities

The Trice tool internal parser has only limited capabilities. In works well in most cases, but could lead to problems in some cases. The compiler run will for sure end up with some error messages in the following examples, so the developer can fix the code.

An example, provided by [@KammutierSpule](https://github.com/kammutierspule), is this:

* started from a empty li.json/til.json

```C
void trice0_test() {
    Trice0( "OK");
    Trice( InvalidUse );
    Trice( "%u", Variable );
}
```

* run `trice insert`

```C
void trice0_test() {
    Trice0( iD(2740), "OK"); // ok, iD is added
    Trice( InvalidUse ); // no warning or error
    Trice( "%u", Variable ); // id is not added / inserted
}
```

As said, the compiler will complain about that in any case.

### 2.3. <a id="special-care-demands"></a>Special Care Demands

#### 2.3.1. <a id="more-than-12-printf-parameters"></a>More than 12 printf parameters

* use several printf-calls
* Use triceB and its relatives

#### 2.3.2. <a id="float-numbers"></a>Float Numbers

* surround each with `aFloat()`

#### 2.3.3. <a id="double-numbers"></a>Double numbers

* surround each with `aDouble()` and use the `trice64` macro and relatives

#### 2.3.4. <a id="runtime-generated-strings"></a>Runtime Generated Strings

* Each needs its own `triceS` macro, example:
  * Legacy code:

    ```C
    printf( "Entered name is %20s %30s, favorite numbers %d, %f\n", "Paul", "Luap", 42, 3.14159 );
    ```

  * Trice code:

    ```C
    name = "Paul"; triceS( "Entered name is %20s", name );`
    surname = "Luap";  triceS( " %30s, ", surname );`
    trice( "favorite numbers %d, %f\n", 42, aFloat(3.14159) );`
    ```

The `triceS` macro is ment to be used with strings not known at compile time.

*Usage intention and recommendation:* (given by [@escherstair](https://github.com/escherstair))

```C
char runtime_string[50];
fillRuntimeStringFromSomewhere(runtime_string); // the content of runtime_string is filled at run time
triceS( "msg:This part of the string is known at compile time. This part is dynamic: %s\n", runtime_string);
```

All the string literals (i.e. compile-time known strings) should be put inside the format string.
Only the runtime strings should be used as variables in triceS macro for best performance.

<p align="right">(<a href="#limitations">back to top</a>)</p>


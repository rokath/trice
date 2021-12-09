![./README.media/TriceGirlS.png](./README.media/TriceGirlS.png)

# Trace with `TRICE()` and get `printf()` comfort inside interrupts and everywhere

## Description

`TRICE()` is a comfortable macro, generating tiny C-code for getting PC `printf()` comfort at "speed-of-light" for any micro-controller. It is supported by an in [Go](https://go.dev/) written powerful PC tool **trice** running on many platforms and comes with sample implementations. Features:
- super fast: a `TRICE()` macro is executable in less than 10 clocks
- target and host timestamps
- runtime filterable colored channels
- encryption option
- enable/disable `TRICE()` on file level 
- integrate several target devices in one log file
- transmit over TCP/IP to a remote server 
- any byte capable connection usable
- compared to `printf()` usage, less needed FLASH memory

![./README.media/life0.gif](./README.media/life0.gif)

## Abstract

If you develop software for an embedded system, you need some kind of system feedback. Debuggers are awesome tools, but when it comes to analyze dynamic behavior in the field, they are not usable.

Logging then, usually done with `printf` like functions, gets quick a result after having a `putchar()` implemented. This turns out to be an expensive way in terms of processor clocks and needed FLASH memory, when you regard the library code and all the strings needing FLASH memory space. For small micro-controllers that´s it.

Bigger micro-controllers are coming with embedded trace hardware. To use it, an expensive tool is needed. Useful for analyzing complex multi-tasking systems, but for in-field related issues at least unhandy.

Unhappy with this situation, the developer starts thinking of using digital pins or a spare analog output with an oscilloscope or starts emitting some proprietary LED blinking codes or byte sequences, difficult to interpret.

The *trice* technique tries to fill this gap trying to be minimal invasive and as comfortable as possible.

![./README.media/COLOR_output.PNG](./README.media/COLOR_output.PNG)

## Simplicity is Complicated ...

... we learned from [Rob Pike](https://www.youtube.com/watch?v=rFejpH_tAHM), one of the **Go** inventors and indeed [Go](https://en.wikipedia.org/wiki/Go_(programming_language)) is easy to learn but a very powerful language. The **trice** tool was started in C++ but focusing on content was difficult because of all the C++ nuts and bolts to deal with. After restarting the project in **Go**, the implementation got amusing.

The same perception exists when using *trice* in an embedded project instead of some other technique. One can simply write `TRICE( "Hello %d, hello, ..., hello %d again!", 88, -88 );` or whatever everywhere without thinking about execution time and FLASH memory size.

This may sound like a miracle, but it is not. *Trice* is the result of a long-year dissatisfaction and several attempts to find a loophole to make embedded programming more fun and this way more effective.

## How it works - the main idea

- Executing a `printf()` like function means:
  - Copy format string from FLASH memory into RAM.
  - Parse the format string for format specifiers.
  - Convert parameters according the format specifiers into character sequences.
    - This includes several divisions - costly function calls.
  - Concatenate the parts to an output string and deliver it to the output, what often means copying again.
  - Never ever call a `printf()` like function in time critical code, like an interrupt.
- *Trice*, instead just copies an ID together with the parameters to the output and is done.
- This is goes in about 10 processor clocks. When running on a 64 MHz clock light can travel about 30 meters in that time.
- To achieve that, an automatic pre-compile step is needed, executing a `trice update` command.
  - The trice tool parses the the source code tree for macros like `TRICE( "Hello World" );` and patches them to `TRICE( Id(nnnnn), "Hello World" );`, where `nnnnn` is a 16-bit identifier associated to the format string `"Hello World"`.
  - During compilation the `TRICE()` macro is translated just to the `nnnnn` ID and the optional parameter values. The format string is ignored by the compiler.
  - At runtime just the ID with its parameter values is delivered to the PC. There the **trice** tool receives it, gets the right format string from the reference list and performs the printing task: ![./README.media/triceCOBSBlockDiagram.svg](./README.media/triceCOBSBlockDiagram.svg)
- The **trice** tries to help as much as possible, to let the developer focus on its programming task. For example, the once generated ID is not changed anymore without need. If for example the format string gets changed into `"Hello World!"` a new ID is generated automatically and the reference list gets extended.

## *Trice* features



## References and further reading

- [*Go* home](https://go.dev/)
- [Rob Pike: "Simplicity is Complicated"]((https://www.youtube.com/watch?v=rFejpH_tAHM))
- [*Trice* on Github](https://github.com/rokath/trice)
- [*Trice* example projects](https://github.com/rokath/trice/test)

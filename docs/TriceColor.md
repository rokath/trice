# Color

- Add a channel name as color descriptor in front of each `TRICE` format string like `"wrn:Peng!"`.
- In file [../internal/emitter/lineTransformerANSI.go](../internal/emitter/lineTransformerANSI.go) the colors are changeable and additinal color channels defineable.
- It is possible to concatenete single colorized letters to get output like this:

![./ref/COLOR_output.PNG](./ref/COLOR_output.PNG)

- [../pkg/src/triceCheck.c](../pkg/src/triceCheck.c) contains the code for this example.
- See also [color issues under Windows](./Common.md#color-issues-under-windows)

* Each *trice* format string can optionally start with a `pattern:` like in `TRICE( "msg:Hi!\n" );`.
* The **trice** tool, if knowing `msg:` as pattern, prepends the appropriate color code. It removes the sequence `pattern:`, if it is completely lower case.
* The user can define any pattern with any color code to create colored output with the **trice** tool.
* There is no channel enable switch inside the target code. It would need a back channel and add overhead.
  * A future extension could make that an option.
* The **trice** tool offers the 2 command line switches `-pick` and `-ban` to control channel visualization during runtime.  


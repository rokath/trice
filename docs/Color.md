# Color

- Add a channel name as color descriptor in front of each `TRICE` format string like `"wrn:Peng!"`.
- In file [../internal/emitter/lineTransformerANSI.go](../internal/emitter/lineTransformerANSI.go) the colors are changeable and additinal color channels defineable.
- It is possible to concatenete single colorized letters to get output like this:

![./README.media/COLOR_output.PNG](./README.media/COLOR_output.PNG)

- [../pkg/src/triceCheck.c](../pkg/src/triceCheck.c) contains the code for this example.
- See also [color issues under Windows](./Common.md#color-issues-under-windows)
# LabPlot live demo

This folder contains one LabPlot project for both signal producers. The project
listens on UDP port `9000` for the normalized numeric CSV stream
`time_s,x,y,z`. The project predeclares the four numeric columns and triggers
one initial read while loading so that LabPlot prepares the UDP socket. It
keeps 500 samples, which is ten seconds at 50 Hz. The time plot always shows
this moving ten-second window instead of continuously compressing its curves.

## Quick start

Install LabPlot 2.12 or newer and make sure `labplot` is on `PATH`. If it is
not, set `LABPLOT` to the executable before starting a script. On macOS the
script also checks the standard application bundle location.

From the repository root run either:

```sh
./examples/LabPlotDemo/run_csv.sh
./examples/LabPlotDemo/run_trice.sh
```

The first command sends CSV directly. The second sends binary Trice data to
`tlog`, which decodes it and forwards the same CSV records to LabPlot. The
Trice script waits until LabPlot has opened UDP port `9000` before starting
the producer and decoder. Stop a run with `Ctrl-C`; the producer and decoder
are then stopped with it.

The project opens one worksheet with two plots side by side: a time plot of
`x`, `y`, and `z`, and an `x` versus `y` Lissajous plot. The latter draws the
most recent three seconds on fixed axes. A slow phase modulation in both
producers keeps its shape moving visibly. Open `LabPlotDemo.lml` manually if
LabPlot is already running. The project does not contain machine-specific
file paths.

If `tlog` reports that a `-vis` rule was disabled because writing to port
`9000` was refused, LabPlot was not listening when the decoder started.
Restart `run_trice.sh`; its readiness check normally prevents this condition.
After a visualization rule is disabled, normal logging intentionally resumes,
so `log=drop` no longer applies to that rule.

## Windows

Run the scripts from Git Bash or another POSIX shell and set, for example,
`LABPLOT=/c/Program\ Files/LabPlot/bin/labplot.exe` if automatic discovery
does not find LabPlot. The demo programs and `tlog` must likewise be on
`PATH`, or `TLOG` can point to the decoder executable.

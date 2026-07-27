# DemoPlotData_CSV

`DemoPlotData_CSV` is a small C11 program that generates attractive synthetic
plot data:

```text
time_s,x,y,z
```

`time_s` and all three signal values are `double`. The timestamp is expressed
in seconds. The program runs on Windows, macOS, and Linux and can write to:

- standard output;
- a newly created CSV file;
- a UDP destination, with one CSV record per datagram.

The signal formula is shared conceptually with the companion
`DemoPlotData_Trice` project. This makes it possible to compare a direct CSV
source with data transported and decoded through TRice.

## Build

A C compiler and CMake 3.16 or newer are required.

Linux or macOS:

```sh
chmod +x build.sh
./build.sh
```

Windows from Git Bash:

```sh
./build.sh
```

Windows PowerShell equivalent:

```powershell
cmake -S . -B build
cmake --build build --config Release
```

The local executable is installed into `bin/`. On Windows it is normally:

```powershell
.\bin\DemoPlotData_CSV.exe
```

The equivalent Git Bash path is:

```sh
./bin/DemoPlotData_CSV
```

## Examples

Write a live stream to the terminal at the default 50 Hz:

```sh
./bin/DemoPlotData_CSV
```

Write ten seconds to a fresh CSV file without real-time waiting:

```sh
./bin/DemoPlotData_CSV --rate 50 --samples 500 --no-delay \
  --header --output DemoPlotData_CSV.csv
```

Send live CSV records to UDP port 9000 on the same computer:

```sh
./bin/DemoPlotData_CSV --udp 127.0.0.1 9000
```

PowerShell:

```powershell
.\bin\DemoPlotData_CSV.exe --udp 127.0.0.1 9000
```

Use `--help` for all options.

## Serial Studio visualization

[Serial Studio](https://serial-studio.com/) is a practical cross-platform
viewer for this demo. It can receive newline-terminated CSV directly through
UDP.

### Fastest setup: Quick Plot

1. Start Serial Studio.
2. Select **Quick Plot (Comma Separated Values)**.
3. Select **Network Socket**, then **UDP**.
4. Set the local UDP port to `9000`.
5. Connect and run this generator with `--udp 127.0.0.1 9000`.

Quick Plot treats all four columns as values. For named signals and a proper
timestamp axis, open the included `DemoPlotData.ssproj` instead.

### Prepared project

1. Open `DemoPlotData.ssproj` in Serial Studio.
2. Connect its configured UDP source.
3. Run the generator with `--udp 127.0.0.1 9000`.

The prepared project expects exactly:

```text
seconds,x,y,z
```

Do not use `--header` for the live UDP stream.

## Signal model

For time `t` in seconds:

```text
x = sin(2π · 0.70 · t)
y = sin(2π · 0.91 · t + π/2)
z = 0.6 · sin(2π · 0.13 · t) + 0.2 · x · y + pulse
```

The pulse has a height of `0.8` and lasts for the final `250 ms` of every
eight-second interval.

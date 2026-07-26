# DemoPlotData_Trice

`DemoPlotData_Trice` generates the same synthetic `x`, `y`, and `z` signals as
the sibling `DemoPlotData_CSV` project, but transports them as binary TRice
records.

The program is intended to be placed here:

```text
trice/
├── src/
└── examples/
    ├── DemoPlotData_CSV/
    └── DemoPlotData_Trice/
```

Its CMake project uses the unchanged TRice target library from `../../src`.

## Timestamp

The TRice target timestamp is a `uint32_t` counter in units of **10 ms**:

```text
target timestamp 0, 2, 4, ... at the default 50 Hz
```

Conversions:

```text
seconds      = ts / 100.0
milliseconds = ts * 10
```

Consequently, a `-vis` expression that emits floating-point seconds must use
`ts/100.0`, not `ts*100`.

## Output

Without an output option, the program opens:

```text
DemoPlotData_Trice.bin
```

in the current working directory. The file is opened with `wb`, so every
program start truncates the previous contents and begins a fresh binary stream.

Alternative destinations:

- `--output FILE` writes a fresh binary file with another name.
- `--stdout` writes the binary stream to standard output.
- `--udp HOST PORT` sends one complete TCOBS-framed TRice record per UDP
  datagram.

## Build

A C compiler, CMake 3.16 or newer, and the surrounding TRice repository are
required.

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

The Visual Studio executable is normally:

```powershell
.\build\Release\DemoPlotData_Trice.exe
```

## Examples

Generate the default file continuously:

```sh
DemoPlotData_Trice
```

Generate 500 records quickly:

```sh
DemoPlotData_Trice --samples 500 --no-delay
```

Send the binary TRice stream to a decoder listening on UDP port 9001:

```sh
DemoPlotData_Trice --udp 127.0.0.1 9001
```

Use `--help` for all options.

## Decode the generated file

The project contains a private `til.json` for its fixed `iD(1000)`. From the
TRice repository root, decode the generated file with settings equivalent to:

```sh
trice log \
  -p FILEBUFFER \
  -args examples/DemoPlotData_Trice/DemoPlotData_Trice.bin \
  -pf TCOBS \
  -i examples/DemoPlotData_Trice/til.json
```

For a fresh CSV file with one header line, use the `header` output option on
the visualization rule:

```sh
tlog -p FILEBUFFER \
  -args DemoPlotData_Trice.bin \
  -til ../../../demoTIL.json \
  -ulabel vis_demo \
  -vis='vis_demo:printf("%0.3f,%0.3f,%0.3f,%0.3f\\n",ts/100.0,v0,v1,v2)@log.csv;header="time_s,X,Y,Z\\n";log=drop'
```

File visualization outputs are recreated automatically when `tlog` starts, so
an existing `log.csv` is replaced. The `header` option is written once per
output sink; its value is a quoted Go string, which allows escapes such as
`\\n`.

## Planned `-vis` demo

When the discussed `-vis` functionality is available, use this live pipeline:

1. Open `examples/DemoPlotData_CSV/DemoPlotData.ssproj` in Serial Studio and
   connect its UDP source on port `9000`.
2. Start the TRice decoder from the repository root:

```sh
trice log \
  -p UDP4 \
  -args 127.0.0.1:9001 \
  -pf TCOBS \
  -i examples/DemoPlotData_Trice/til.json \
  -vis='vis_demo:printf("%0.6f,%0.6f,%0.6f,%0.6f\n",ts/100.0,v0,v1,v2)@udp://127.0.0.1:9000,log=drop'
```

3. Start the binary generator:

```sh
examples/DemoPlotData_Trice/build/DemoPlotData_Trice \
  --udp 127.0.0.1 9001
```

The `-vis` expression converts each selected TRice record to the same CSV
format as `DemoPlotData_CSV`:

```text
seconds,x,y,z
```

On a Visual Studio multi-configuration build, the generator path is usually:

```text
examples/DemoPlotData_Trice/build/Release/DemoPlotData_Trice.exe
```

The generator's own `--udp` option transports **binary TRice**, so it must be
received by `trice log` or another TRice decoder. It must not be connected
directly to Serial Studio.

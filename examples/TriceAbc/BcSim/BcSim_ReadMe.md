# BcSim - Broadcast Simulator

`BcSim` is a small, standalone C module for Trice broadcast simulations. It
implements a visible file-backed byte stream that several PC applications can use
as a simple broadcast medium.

The package is intentionally split into two directories:

```text
BcSim/
    BcSim_config.h
    BcSim.h
    BcSim.c

BcSimChk/
    main.c
    build.sh
    build.bat
    demo.sh
```

`BcSim` is the reusable part. It contains only the byte-stream I/O module and its
configuration header. It can later be reused for Trice broadcast-command demo applications
without pulling in the `BcSimChk` example program.

`BcSimChk` is only a check and demonstration program. It builds one binary that can be
started several times with different names.

## Design goal

The module exists to make a Trice broadcast-command simulation easy to understand. It avoids
a TCP hub, UDP multicast, Bluetooth setup, embedded hardware, or board simulator.
All participating applications share one binary append-only file:

```text
bc.bus
```

Every application writes bytes to this file. Every application also polls the
file size and reads bytes that other applications appended.

The important rule is:

```text
bc.bus contains only the bytes passed to bcSimWrite().
```

The file does not contain device names, status strings, timestamps, length
fields, or any other demo metadata. This matters because a later Trice broadcast-command demo
can put real Trice-compatible bytes into `bc.bus` and inspect the same file with
a Trice FILE input mode.

Human-readable diagnostics go into a separate file:

```text
bc.log
```

## Layering

`BcSim` is deliberately below Trice and below packet framing:

```text
bcSimRead/bcSimWrite  ->  shared byte stream over bc.bus
optional decode layer     ->  none, COBS, TCOBS, XTEA, ...
Trice decoder / BC RX    ->  the Trice receive runtime
```

The module does not know about:

```text
Trice IDs
Trice ABC handler tables
Trice ABC stamps
COBS or TCOBS
XTEA
record boundaries
payload types
```

It transports only bytes.

## Public API

```c
int bcSimOpen(BcSim_t* io,
                const char* busPath,
                const char* logPath,
                const char* deviceName);

int bcSimRead(BcSim_t* io,
                uint8_t* p,
                size_t max,
                const char* status);

int bcSimWrite(BcSim_t* io,
                 const uint8_t* p,
                 size_t n,
                 const char* status);

void bcSimClose(BcSim_t* io);

const char* bcSimErrorString(int err);
```

`status` is optional. It is used only in `bc.log`. It may be `NULL` or an empty
string.

## Opening a device-side view

`bcSimOpen()` initializes one device-side handle and sets:

```c
io->readOffset = current_size_of_abc_bus;
```

This is deliberate. A newly started device joins the live bus and does not replay
historical traffic. That is the correct default for a normal device.

A special replay or monitor tool can explicitly set:

```c
io.readOffset = 0;
```

after `bcSimOpen()`. Normal device programs should not expose this as a casual
CLI option because historical own messages cannot be recognized after restart.
The self-echo filter only knows the ranges written by the current process during
the current run.

## Writing

`bcSimWrite()` appends the given byte sequence unchanged to `bc.bus`:

```c
bcSimWrite(&io, bytes, byteCount, "tx-0");
```

The function also remembers the written file-offset range:

```text
[start, start + byteCount)
```

That range is later used by `bcSimRead()` to filter the process' own writes.

## Self-echo filtering

All devices read the same bus file, so a process would naturally see its own
writes again. This module does not filter by byte content and does not require
BC stamps. It filters by bus-file offsets.

When a process writes bytes, the module records the exact file range that was
appended. Later, while reading, bytes in that range are consumed internally but
not returned to the caller.

This is safer than comparing byte sequences because different devices may send
identical byte payloads, and because a process' own write is not necessarily the
next thing it reads.

## Reading

`bcSimRead()` reads newly appended bytes and returns only foreign bytes:

```c
uint8_t buf[1024];
int n = bcSimRead(&io, buf, sizeof(buf), "poll");
```

The returned bytes are not packet-aligned by this module. A call may return a
partial upper-layer packet or several upper-layer packets. The layer above
`BcSim` must buffer and decode its selected stream format.

This is intentional because it is close to real stream transports such as UART,
TCP, or ordinary files.

## Read locking

By default, reads do not acquire the writer lock:

```c
#define BCSIM_READ_USES_LOCK 0
```

This means a reader may observe bytes while another process is appending. The
upper framing/decoder layer must handle incomplete data. This is normally the
best behavior for a stream-oriented demonstration.

For deterministic experiments, read locking can be enabled in
`BcSim/BcSim_config.h` or via compiler flags:

```sh
CFLAGS="-DBCSIM_READ_USES_LOCK=1" ./build.sh
```

## Write locking

Writers are serialized by an atomically created lock directory:

```text
bc.bus.lock/
```

The write sequence is:

```text
1. create bc.bus.lock directory
2. determine current size of bc.bus
3. append the byte sequence to bc.bus
4. remember the own offset range
5. append a TX line to bc.log
6. remove bc.bus.lock directory
```

Creating a directory is used because it is simple and works on local POSIX and
Windows filesystems. If another process already holds the lock, the writer waits
for a short configured interval and then tries again.

## Diagnostic log

`bc.log` is optional. It is independent from `bc.bus` and is meant only for
humans. It contains a short header and one line per logged TX or RX chunk.

Offsets and lengths are decimal and right-aligned. Bytes are hexadecimal and
keep leading zeros:

```text
# BcSim traffic log
# bc.bus is a pure binary byte stream. This text log is diagnostic only.
# offset and len are decimal values. Bytes are hexadecimal %02x values.
#   offset  len device       dir status               bytes
# -------- ---- ------------ --- -------------------- --------------------------------
         0   11 A            TX  tx-0                 91 60 7b 1a 70 37 4b d7 03 40 9d
        11   23 B            TX  tx-0                 25 e1 35 0a d5 0d 20 ef ...
        11   23 A            RX  poll-1               25 e1 35 0a d5 0d 20 ef ...
```

The same bus bytes may appear several times as RX lines because every device has
its own view of the broadcast medium. That is useful for a demonstration: the log
shows which device saw which bytes.

## Building the check program

On Linux, macOS, WSL, MSYS2, MinGW Git Bash, or Cygwin:

```sh
cd BcSimChk
./build.sh
```

With Microsoft `cl.exe` from a Developer Command Prompt:

```bat
cd BcSimChk
build.bat
```

## Running the check demo

The easiest check is:

```sh
cd BcSimChk
./demo.sh
```

The script builds the check binary, removes old `bc.bus` and `bc.log`, starts
four instances of the same binary, waits for them to finish, and then prints the
log and a hex dump of the binary bus file.

Manual run:

```sh
cd BcSimChk
rm -f bc.bus bc.log
rm -rf bc.bus.lock
./build/bcSimDevice -name A
./build/bcSimDevice -name B
./build/bcSimDevice -name C
```

Each instance sends random byte blocks with random lengths between 4 and 32
bytes. This keeps the check program generic and avoids looking like a text-chat
protocol.

## Relation to a future Trice broadcast-command demo

For a real Trice broadcast-command simulation, keep `BcSim` unchanged and replace only the
check program's random payload generation. The future demo application would
produce real Trice-compatible bytes, write them through `bcSimWrite()`, read
foreign bytes through `bcSimRead()`, decode the selected stream format, and
then call `the Trice receive runtime`.

The important property remains:

```text
bc.bus is the complete visible byte stream.
bc.log is only the human-readable side log.
```

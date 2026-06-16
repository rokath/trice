# demoAbcInOut

`demoAbcInOut` is a small, standalone C module for Trice ABC demonstrations. It
implements a visible file-backed byte stream that several PC applications can use
as a simple broadcast medium.

The package is intentionally split into two directories:

```text
Lib/
    demoAbcInOut_config.h
    demoAbcInOut.h
    demoAbcInOut.c

Chk/
    main.c
    build.sh
    build.bat
    demo.sh
```

`Lib` is the reusable part. It contains only the byte-stream I/O module and its
configuration header. It can later be reused for Trice ABC demo applications
without pulling in the `Chk` example program.

`Chk` is only a check and demonstration program. It builds one binary that can be
started several times with different names.

## Design goal

The module exists to make a Trice ABC demonstration easy to understand. It avoids
a TCP hub, UDP multicast, Bluetooth setup, embedded hardware, or board simulator.
All participating applications share one binary append-only file:

```text
abc.bus
```

Every application writes bytes to this file. Every application also polls the
file size and reads bytes that other applications appended.

The important rule is:

```text
abc.bus contains only the bytes passed to demoAbcWrite().
```

The file does not contain device names, status strings, timestamps, length
fields, or any other demo metadata. This matters because a later Trice ABC demo
can put real Trice-compatible bytes into `abc.bus` and inspect the same file with
a Trice FILE input mode.

Human-readable diagnostics go into a separate file:

```text
abc.log
```

## Layering

`demoAbcInOut` is deliberately below Trice and below packet framing:

```text
demoAbcRead/demoAbcWrite  ->  shared byte stream over abc.bus
optional decode layer     ->  none, COBS, TCOBS, XTEA, ...
Trice decoder / ABC RX    ->  TriceAbcOnReceive(...)
```

The module does not know about:

```text
Trice IDs
ABC handler tables
ABC stamps
COBS or TCOBS
XTEA
record boundaries
payload types
```

It transports only bytes.

## Public API

```c
int demoAbcOpen(demoAbcInOut_t* io,
                const char* busPath,
                const char* logPath,
                const char* deviceName);

int demoAbcRead(demoAbcInOut_t* io,
                uint8_t* p,
                size_t max,
                const char* status);

int demoAbcWrite(demoAbcInOut_t* io,
                 const uint8_t* p,
                 size_t n,
                 const char* status);

void demoAbcClose(demoAbcInOut_t* io);

const char* demoAbcErrorString(int err);
```

`status` is optional. It is used only in `abc.log`. It may be `NULL` or an empty
string.

## Opening a device-side view

`demoAbcOpen()` initializes one device-side handle and sets:

```c
io->readOffset = current_size_of_abc_bus;
```

This is deliberate. A newly started device joins the live bus and does not replay
historical traffic. That is the correct default for a normal device.

A special replay or monitor tool can explicitly set:

```c
io.readOffset = 0;
```

after `demoAbcOpen()`. Normal device programs should not expose this as a casual
CLI option because historical own messages cannot be recognized after restart.
The self-echo filter only knows the ranges written by the current process during
the current run.

## Writing

`demoAbcWrite()` appends the given byte sequence unchanged to `abc.bus`:

```c
demoAbcWrite(&io, bytes, byteCount, "tx-0");
```

The function also remembers the written file-offset range:

```text
[start, start + byteCount)
```

That range is later used by `demoAbcRead()` to filter the process' own writes.

## Self-echo filtering

All devices read the same bus file, so a process would naturally see its own
writes again. This module does not filter by byte content and does not require
ABC stamps. It filters by bus-file offsets.

When a process writes bytes, the module records the exact file range that was
appended. Later, while reading, bytes in that range are consumed internally but
not returned to the caller.

This is safer than comparing byte sequences because different devices may send
identical byte payloads, and because a process' own write is not necessarily the
next thing it reads.

## Reading

`demoAbcRead()` reads newly appended bytes and returns only foreign bytes:

```c
uint8_t buf[1024];
int n = demoAbcRead(&io, buf, sizeof(buf), "poll");
```

The returned bytes are not packet-aligned by this module. A call may return a
partial upper-layer packet or several upper-layer packets. The layer above
`demoAbcInOut` must buffer and decode its selected stream format.

This is intentional because it is close to real stream transports such as UART,
TCP, or ordinary files.

## Read locking

By default, reads do not acquire the writer lock:

```c
#define DEMO_ABC_READ_USES_LOCK 0
```

This means a reader may observe bytes while another process is appending. The
upper framing/decoder layer must handle incomplete data. This is normally the
best behavior for a stream-oriented demonstration.

For deterministic experiments, read locking can be enabled in
`Lib/demoAbcInOut_config.h` or via compiler flags:

```sh
CFLAGS="-DDEMO_ABC_READ_USES_LOCK=1" ./build.sh
```

## Write locking

Writers are serialized by an atomically created lock directory:

```text
abc.bus.lock/
```

The write sequence is:

```text
1. create abc.bus.lock directory
2. determine current size of abc.bus
3. append the byte sequence to abc.bus
4. remember the own offset range
5. append a TX line to abc.log
6. remove abc.bus.lock directory
```

Creating a directory is used because it is simple and works on local POSIX and
Windows filesystems. If another process already holds the lock, the writer waits
for a short configured interval and then tries again.

## Diagnostic log

`abc.log` is optional. It is independent from `abc.bus` and is meant only for
humans. It contains a short header and one line per logged TX or RX chunk.

Offsets and lengths are decimal and right-aligned. Bytes are hexadecimal and
keep leading zeros:

```text
# demoAbcInOut traffic log
# abc.bus is a pure binary byte stream. This text log is diagnostic only.
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
cd Chk
./build.sh
```

With Microsoft `cl.exe` from a Developer Command Prompt:

```bat
cd Chk
build.bat
```

## Running the check demo

The easiest check is:

```sh
cd Chk
./demo.sh
```

The script builds the check binary, removes old `abc.bus` and `abc.log`, starts
four instances of the same binary, waits for them to finish, and then prints the
log and a hex dump of the binary bus file.

Manual run:

```sh
cd Chk
rm -f abc.bus abc.log
rm -rf abc.bus.lock
./build/demoAbcDevice -name A
./build/demoAbcDevice -name B
./build/demoAbcDevice -name C
```

Each instance sends random byte blocks with random lengths between 4 and 32
bytes. This keeps the check program generic and avoids looking like a text-chat
protocol.

## Relation to a future Trice ABC demo

For a real Trice ABC demonstration, keep `Lib` unchanged and replace only the
check program's random payload generation. The future demo application would
produce real Trice-compatible bytes, write them through `demoAbcWrite()`, read
foreign bytes through `demoAbcRead()`, decode the selected stream format, and
then call `TriceAbcOnReceive()`.

The important property remains:

```text
abc.bus is the complete visible byte stream.
abc.log is only the human-readable side log.
```

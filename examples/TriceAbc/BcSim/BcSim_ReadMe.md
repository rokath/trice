# BcSim

`BcSim` is a small, standalone C module for simulating a broadcast byte stream
with ordinary files. Several PC applications can append bytes to the same bus
file and can poll the file for bytes appended by other applications.

The module is intentionally protocol-neutral. It does not know message IDs,
packet boundaries, framing, encryption, source addresses, commands, or handlers. It only transports bytes and optionally writes a
human-readable traffic log.

## Files

A typical demo using `BcSim` creates these runtime files:

```text
bc.bus        binary append-only bus file
bc.log        optional human-readable hex log
bc.bus.lock/  temporary writer lock directory
```

`bc.bus` contains exactly the bytes passed to `bcSimWrite()`. No device name,
status text, length field, time value, or other metadata is inserted into the
binary bus file.

`bc.log` is only for humans. It can show which application wrote or read which
bytes, together with an optional status string.

## Directory split

```text
BcSim/
    BcSim_config.h
    BcSim.h
    BcSim.c
    BcSim_ReadMe.md

BcSimChk/
    main.c
    build.sh
    demo.sh
```

`BcSim/` is the reusable library part. `BcSimChk/` is only a small check program
that demonstrates the library with random byte blocks. The check program is not
needed by applications that reuse `BcSim` as their own byte-stream medium.

## Design

Each process owns one `BcSim_t` object.

- `bcSimOpen()` opens a local view of the bus and initializes the read offset to
  the current end of the bus file.
- `bcSimWrite()` appends bytes to the bus file and remembers the written file
  offset range.
- `bcSimRead()` reads newly appended bytes and filters out ranges written by the
  same `BcSim_t` object.
- `bcSimClose()` closes the local view and resets the state.

The self-echo filter is based on file offsets, not on byte comparison. This is
important because two applications may write identical byte sequences, and a
single application may intentionally write the same bytes more than once.

## Writer locking

Writers are serialized with an atomically created lock directory. For a bus file
named `bc.bus`, the lock directory is `bc.bus.lock/`.

A write operation uses this sequence:

```text
create lock directory
get current bus file size
append bytes to bus file
remember own [start,end) range
append optional TX line to log file
remove lock directory
```

Directory creation is used because it is simple and available on common local PC
filesystems. If several processes try to create the same lock directory, exactly
one succeeds and the others retry until the timeout expires.

## Reading

By default, `bcSimRead()` does not acquire the writer lock. It may therefore see
partial data while another process is writing. That is intentional for streamlike
demos: higher protocol layers should be able to buffer incomplete input and
retry when more bytes arrive.

For deterministic checks, define:

```c
#define BCSIM_READ_USES_LOCK 1
```

Then `bcSimRead()` waits for the same lock used by writers.

## Log format

`bc.log` starts with a short header and then uses one line per TX or RX event.
Offsets and lengths are right-aligned without leading zeroes. Bytes are printed
as space-separated two-digit lowercase hexadecimal values.

Example:

```text
# BcSim traffic log
# bus file: bc.bus
# Columns: offset, len, device, direction, status, bytes
#  offset    len  device            dir  status                  bytes
# -------  -----  ----------------  ---  ----------------------  ----------------
      0      12  A                 TX   tx-0                    35 6a 11 8e ...
     12      12  B                 RX   poll-1                  35 6a 11 8e ...
```

## API overview

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
```

All functions return either a non-negative byte count or a negative `BCSIM_ERR_*`
value, except `bcSimClose()`, which has no return value.

## Limitations

`BcSim` is a local demonstration transport. It is not a high-performance IPC
system and not a replacement for a real embedded communication link.

Notable limitations:

- The bus file grows until the demo script removes it.
- The self-echo filter remembers only a finite number of own write ranges.
- A restarted process cannot recognize byte ranges written by a previous
  instance of itself.
- Lock cleanup after a killed writer may require manual removal of the lock
  directory.
- Network filesystems may not provide the same atomicity and notification
  behavior as local filesystems.

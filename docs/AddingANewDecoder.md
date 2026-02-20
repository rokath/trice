# Adding A New Decoder

This note describes how to add another decoder implementation to Trice.

## Goal

A decoder package should:

- Implement the common `decoder.Decoder` interface.
- Reuse shared initialization from `internal/decoder`.
- Register itself so translator selection works automatically.

## Steps

1. Create a new package (for example `internal/myDecoder`).

2. Implement a constructor with the standard signature:

```go
func New(
    w io.Writer,
    lut id.TriceIDLookUp,
    m *sync.RWMutex,
    li id.TriceIDLookUpLI,
    in io.Reader,
    endian bool,
) decoder.Decoder
```

3. Initialize common fields with:

```go
decoder.NewDecoderData(decoder.Config{
    Out:         w,
    LUT:         lut,
    LUTMutex:    m,
    LI:          li,
    In:          in,
    Endian:      endian,
    NeedBuffers: false, // true for framed/packet decoders
})
```

4. Register the decoder in `init()`:

```go
func init() {
    decoder.Register("MY_ENCODING", New)
}
```

5. Ensure package registration runs by importing it for side effects in translator:

```go
import _ "github.com/rokath/trice/internal/myDecoder"
```

Then `decoder.NewForEncoding("MY_ENCODING", ...)` can create it.

## Notes

- Encoding lookup is case-insensitive.
- `Config.Out == nil` is normalized to `io.Discard`.
- `Config.LUTMutex == nil` gets a default mutex.
- Add package-specific tests for constructor behavior and `Read` edge cases.

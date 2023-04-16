# TRICE Target Code Implementation

## TRICE Macro structure

### `TRICE_ENTER`

- Optionally disable global interrupt.
- Prepare `TriceBufferWritePosition` and keep its initial value.

### `TRICE_PUT`

- Use and increment `TriceBufferWritePosition`.

### `TRICE_LEAVE`

- Use `TriceBufferWritePosition` and its initial value for data transfer
- Optionally enable global interrupt.

## `TRICE_STACK_BUFFER`

- `TRICE_ENTER`: Allocate stack
- `TRICE_LEAVE`: call TriceOut()

## `TRICE_DOUBLE_BUFFER`

- `TRICE_ENTER`: Keep TriceBufferWritePosition
- `TRICE_LEAVE`: call TriceOutRtt0()

## `TRICE_STREAM_BUFFER`

- `TRICE_ENTER`: Keep TriceBufferWritePosition and add offset
- `TRICE_LEAVE`: Push start & Len to fifo

The `TRICE_STREAM_BUFFER` method is not optimal. It allocates incremental stream buffer space and each trice location is pushed into a fifo, which is read by a deferred task. Most trices are real short, so a `memcpy` call is better. --> `TRICE_STATIC_BUFFER`

## `TRICE_STATIC_BUFFER`

- This is like `TRICE_STACK_BUFFER` but avoids stack allocation, what is better for many stacks.

## `TRICE_UART_BUFFER`

- Avoid the `memcpy` by direct writing into the UART fifo. This is not good, because it slows 

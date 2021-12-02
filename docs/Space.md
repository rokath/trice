
# Space

- CubeMX generated project without `Trice`: `Program Size: Code=2208 RO-data=236 RW-data=4 ZI-data=1636`  
- Same with default `Trice` instrumentation: `Program Size: Code=2828 RO-data=236 RW-data=44 ZI-data=1836`
- Needed [FLASH memory](https://en.wikipedia.org/wiki/Flash_memory): 620 Bytes
- Needed [RAM](https://en.wikipedia.org/wiki/Random-access_memory): 40 Bytes plus 200 Bytes for the 2 times 100 Bytes double buffer
- With increased/decreased buffers also more/less [RAM](https://en.wikipedia.org/wiki/Random-access_memory) is needed.
- With each additional `TRICE` macro a few additional [FLASH memory](https://en.wikipedia.org/wiki/Flash_memory) bytes are needed.
- No `printf` library code is used anymore.
- No format strings get into the target code anymore.
- In general `Trice` instrumentation **reduces** the needed memory compared to a `printf` implementation.

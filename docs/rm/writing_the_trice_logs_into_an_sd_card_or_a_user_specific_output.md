# Writing the Trice logs into an SD-card (or a user specific output)

<details markdown="1">
<summary>Contents</summary>

---

<!-- mdtoc -->
<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

---

</details>

* Enable `TRICE_DEFERRED_AUXILIARY8` in your project specific _triceConfig.h_ file. 
* Enabling `TRICE_DEFERRED_AUXILIARY8` is possible parallel to any direct and/or deferred output.
* The `TRICE_DEFERRED_OUT_FRAMING` value is used also for the deferred auxiliary writes.
* Consider the value for `TRICE_DEFERRED_TRANSFER_MODE`. The `TRICE_SINGLE_PACK_MODE` would trigger a file write function on each single Trice message. 
* Provide a self-made function like this:

    ```C
    /// mySDcardWrite performs SD-card writing by appending to file myTriceLogs.bin.
    mySDcardWrite(const uint8_t* buf, size_t bufLen){
        ...
    }

    /// Assign this function pointer accordingly.
    UserNonBlockingDeferredWrite8AuxiliaryFn = mySDcardWrite; 
    ```

* If the SD-card write is more effective using 32-bits chunks, consider `TRICE_DEFERRED_AUXILIARY32`, what is recommended also if you use the encryption option.
  * `TRICE_DEFERRED_AUXILIARY32` writes whole 32-bit words and pads the last word with zero bytes when needed.
* There maybe use cases for `TRICE_DIRECT_AUXILIARY8` or `TRICE_DIRECT_AUXILIARY32`, but consider the max write time.
* Placing the files *til.json* and *li.json* anto the SD-card as well might be meaninjful.
* To decode *myTriceLogs.bin* later

    ```bash
    trice log -port FILEBUFFER -args myTriceLogs.bin -hs off
    ```

Related issues/discussions:
[\#253](https://github.com/rokath/trice/discussions/253)
[\#405](https://github.com/rokath/trice/discussions/405)
[\#425](https://github.com/rokath/trice/issues/425)
[\#447](https://github.com/rokath/trice/discussions/447)
[\#537](https://github.com/rokath/trice/discussions/537)

<p align="right">(<a href="#writing-the-trice-logs-into-an-sd-card-or-a-user-specific-output">back to top</a>)</p>


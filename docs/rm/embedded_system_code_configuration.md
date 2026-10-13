# Embedded system code configuration

<details markdown="1">
<summary>Contents</summary>

---

<!-- mdtoc -->
<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

---

</details>

Check comments inside [triceDefaultConfig.h](../../src/triceDefaultConfig.h) and adapt your project configuration like shown in [triceConfig.h](../../examples/F030_inst/Core/Inc/triceConfig.h) as example.

A Trice macro is avoiding all the `printf()` internal overhead (space and time) but is nearly as easy to use. For example instead of writing

```c
printf("time is %d:%d:%d\n", hour, min, sec);
```

you can write

```c
trice8("time is %d:%d:%d\n", hour, min, sec);
```

into a source file of your project. The `8` stands here for 8 bit values (`16`, `32` and `64` also possible). Values of mixed size up to 32-bit size are allowed in one `trice` macro, so you can use Trice consequently to match most cases for the prize of little data overhead.

<p align="right">(<a href="#embedded-system-code-configuration">back to top</a>)</p>

---


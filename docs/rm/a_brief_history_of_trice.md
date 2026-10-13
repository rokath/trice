# A brief history of Trice

<details markdown="1">
<summary>Contents</summary>

---

<!-- mdtoc -->
<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

---

</details>

Developing firmware means to deal also with interrupts and often with timing. How do you check, if an interrupt occurred? OK, increment a counter and display it in a background loop with some printf-like function. What about time measurement? Set a digital output to 1 and 0 and connect a measurement device. Once, developing software for a real-time image processing device, I had no clue where in detail the processing time exploded when the image quality got bad. A spare analog output with a video interrupt synced oscilloscope gave me the needed information, after I changed the analog output on several points in my algorithm. But, hey guys, I want to deal with my programming tasks and do not like all this hassle connecting wires and steer into instruments.

A `printf` is so cool on a PC, developing software there. But an embedded device often cannot use it for performance reasons. My very first attempt was writing the format string `.const` offset together with its values in a FIFO during a log statement and to do the `printf` it in the background. But that is compiler specific. OK the full string address is better but needs buffer space. [Zephyr](https://docs.zephyrproject.org/latest/) for example does something like that calling it "deferred logging".

Then, one day I had the idea to compute short checksums for the format strings in a pre-compile step and to use them as ID in a list together with the format strings. That was a step forward but needed to write a supporting PC program. I did that in C++ in the assumption to get it better done that way. Finally, it worked, but I hated my PC code, as I dislike C++ now because of all its nuts and bolts to handle, accompanied by missing libraries on the next PC. The tool usability was also unhandy and therefore error prone and the need became clear for a full automated solution. Also, what is, if 2 different format strings accidentally generate the same short checksum? There was a way around, but an ID based message filtering will never be possible that way.

The need became clear for controllable IDs and management options. And there was [Go](https://go.dev) now, an as-fast-as-**C** language, easy to learn, promising high programming efficiency and portability. It would be interesting to try it out on a real PC project.

Trying to add tags in form of partial Trice macro names was blowing up the header code amount and was a too rigid design. Which are the right tags? One lucky day I came to the conclusion to handle tags just as format string parts like `"debug:Here we are!\n"` and getting rid of them in the target code this way also giving the user [freedom](../../internal/emitter/lineTransformerANSI.go) to invent any tags.

Another point in the design was the question how to re-sync after data stream interruption, because that happens often during firmware development. Several encodings were tried out and a proprietary escape sequence format and an alternative flexible data format with more ID bits were working reliably but with [COBS](https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing) things got satisfactory. A side result of that trials is the Trice tool option to add different decoders if needed. Now the default Trice message framing is [TCOBSv1](https://github.com/rokath/tcobs) which includes short message compression and this way allows very low transmit bandwidths and/or saves storage, when binary Trice data are stored in Flash memory.

There was a learning **not** to reduce the transmit byte count to an absolute minimum, but to focus more on Trice macro [speed](trice_features_overview.md#execution-speed) and universality. That led to a double buffer on the target side as an alternative to the ring buffer solution. The actual binary [encoding](binary_encoding.md), allowing alongside user protocols, is the result of the optional target timestamps and location info some users asked for, keeping the target code as light as possible. Float and double number support was implementable for free because this work is done mainly on the host side.

Trice grew, and as it got usable I decided to make it Open Source to say "Thank You" to the community this way.

Learning that Trice is also a [baby girl name](https://www.babynamespedia.com/meaning/Trice), our daughter Ida designed the little girl with the pen symbolizing the Trice macro for recording and the eyeglasses standing for the PC tool Trice visualizing the logs.

![./ref/TriceGirlS.png](../ref/TriceGirlS.png)

<p align="right">(<a href="#a-brief-history-of-trice">back to top</a>)</p>


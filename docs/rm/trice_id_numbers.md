# Trice ID Numbers

<details markdown="1">
<summary>Contents</summary>

<!-- mdtoc -->

* [1. ID number selection](#id-number-selection)
  * [1.1. Trice tool internal Method to get fast a random ID](#trice-tool-internal-method-to-get-fast-a-random-id)
* [2. ID number usage and stability](#id-number-usage-and-stability)
* [3. Trice ID 0](#trice-id-0)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>

## 1. <a id="id-number-selection"></a>ID number selection

* The default encoding TREX supports 14-bit IDs, so over 16000 IDs possible. Other encodings can work with other ID sizes.
* `trice("Hi!\n");` ➡ `trice i` ➡ `trice( iD(12345), "Hi!\n");` ➡ `trice c` ➡ `trice("Hi!\n");`
* The **ID** `12345` is a number assigned to `trice( "Hi!\n");` in the above example.
  * It is a so far unused number, according to rules you can control:
    * The `-IDMethod` switch allows a selection method for new IDs.
      * Per default new IDs determined randomly to keep the chance low, that several developers grab the same ID.
      * Example: `trice insert -IDMin 1000 -IDMethod upward` will choose the smallest free ID >= 1000.
        * This allows to use the ID space without wholes.
    * The `-IDMin` and `-IDMax` switches are usable to control the ID range, a new ID is selected from, making it possible to divide the ID space. Each developer can gets it region.
      * Example: `trice insert -IDMin 6000 -IDMax 6999` will choose new randomly IDs only between 6000 and 6999.
* It is possible to give each Trice tag an **ID** range making it possible to implement Trice tag specific runtime on/off on the target side if that is needed. This could be interesting for routing purposes also. Please run `trice help -insert` and read about the `-IDRange` switch for more details.

### 1.1. <a id="trice-tool-internal-method-to-get-fast-a-random-id"></a>Trice tool internal Method to get fast a random ID

* Create Slice with numbers 1...16383
* Remove all used ID numbers from this slice
* Get random number between 0 and len(slice)
* remove this ID from slice and use it as new ID

## 2. <a id="id-number-usage-and-stability"></a>ID number usage and stability

* If you write `trice( "msg:%d", 1);` again on a 2nd location, the copy gets a different **ID**, because each Trice gets its own **ID**.
* If you change `trice( "msg:%d", 1);` to `trice8( "msg:%d", 1);`, to reduce the needed parameter space, a new **ID** is assigned. That is because the parameter bit width is implicit a part of the now changed Trice. If you change that back, the previous **ID** is assigned again.
* If you change `trice( "msg:%d", 1);` to `TRice8( "msg:%d", 1);`, to get a 32-bit stamp, the associated **ID** remains unchanged. That is because the optional stamp is not a part of the Trice itself.
* IDs stay constant and get only changed to solve conflicts.
* To make sure, a single ID will not be changed, you could change it manually to a hexadecimal syntax.
  * This lets the `trice insert` command ignore such Trice macros and therefore a full [til.json](../../demoTIL.json) rebuild will not add them anymore. Generally this should not be done, because this could cause future bugs.
  * It is possible to assign an ID manually as decimal number. It will be added to the ID list automatically during the next `trice i|c` if no conflicts occur.
* If a Trice was deleted inside the source tree (or file removal) the appropriate ID stays inside the ID list.
* If the same string appears again in the same file this ID is active again.
* If a trice occurs more than one time, each occurrence gets a different ID. If then 2 of them disappear, their ID numbers stay in `til.json`. If then one of them comes back, it gets its ID back.

## 3. <a id="trice-id-0"></a>Trice ID 0

* The trice ID 0 is a placeholder for "no ID", which is replaced automatically during the next `trice insert` according to the used trice switches `-IDMethod`, `-IDMin` and `IDMax`.
  * It is sufficient to write the `TRICE` macros just without the `id(0),` `Id(0),` `ID(0),`. It will be inserted automatically according the `-defaultStampSize` switch. With `trice clean` these stay with 0-values in the source code to encode the intended stamp size.
  * It is recommended to use the `trice`, `Trice` and `TRice` macros instead of `TRICE`. They encode the stamp size in their names already. There may be cases, where the user prefers to use the code inserting macros `TRICE` to get maximum performnce.

<p align="right">(<a href="#trice-id-numbers">back to top</a>)</p>


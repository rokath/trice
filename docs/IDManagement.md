# TRICE ID management 

(This document is correct but needs restructuring)

## *Trice* Identifiers

A *Trice* **Id** is needed as shown in the table:

| Write              | After `trice update`          | Remark                                               |
|--------------------|-------------------------------|------------------------------------------------------|
| `TRICE( "Hi!\n");` | `TRICE( Id(12345), "Hi!\n"),` | Run `trice update` automatically in a prebuilt step. |

* No need to write it, but a prebuilt step `trice update` is needed to generate it.
* The **Id** `123435` is a number specific to `"Hi!\n"`.
  * It is a so far unused number, according to rules you can control.
* If you write `TRICE( "Hi!\n");` again on a 2nd location, it gets the same or a different **Id** - as you decide. 
  * Default is *different*.
  * Use  `trice update -sharedIDs` to force te same ID for identical format strings.
* Per default new IDs determined randomly to keep the chance low, that several developers grab the same ID.
* It is possible to divide the ID space - each developer can gets it region.
  * `trice update -IDMin 1000 -IDMax 2000` will choose new IDs only between 1000 and 2000.
* In a future **trice** tool it is possible to give each *trice* channel (see below) an **Id** range making it possible to implement *trice* channel specific runtime on/off.
* New IDs can be also incremental or decremental instead of random.
  * `trice update -IDMin 1000 -IDMethod upward` will choose the smallest free ID >= 1000.
* `TRICE` macros commented out, are visible for the `trice update` command and therefore regarded.
  * `// TRICE( Id(12345), "Hi!\n" );` is still regarded by the `trice u`.
* IDs get changed automatically to solve conflicts only.
* To make sure, a single ID will not be changed, change it to a hexadecimal syntax.
* The ID reference list keeps all obsolete IDs with their format strings allowing compatibility to former firmware versions.
* One can delete the ID reference list. It will be reconstructed automatically from the source tree with the next `trice update` command, but history is lost then.
* Optionally add the (compressed) ID reference list as resource into the target FLASH memory to be sure not to loose it in the next 20 years. 
* `trice h -u` shows relevant ID management options.
* It is also possible to set all IDs in a source tree to **0** - see the full help for details and further options: `trice help -all`.

## ID management internals & hints

- The `COBS` (default) encoding supports 16-bit IDs normally, so over 65000 IDs possible. Other encodings can work with other ID sizes.
- During `trice update` so far unknown IDs are added to the ID list (case new sources added).
- If an ID was deleted inside the source tree (or file removal) the appropriate ID's stays inside the ID list.
- If the same ID appears again the appropriate ID is active again.
- If duplicate ID's with different format strings found inside the source tree (case several developers) one ID is replaced by a new ID. The probability for such case is low, because of the default random ID generation. Also it is possible to split the ID space between several developers using `-IDMin` and `-IDMax`.
- If the format string was modified, the ID stays in the list and a new ID for the changed format string is generated.
- Keeping obsolete IDs makes it more comfortable during development to deal with different firmware variants at the same time.
- This way you can simply copy a `TRICE` statement and modify it without dealing with the ID. The trice tool will do for you.
- The ID list should go into the version control repository of your project.
- Best praxis: Before check-in, delete ID list, restore it from the version control, run `trice update` and check-in the ID list to get rid of the daily development garbage.
- For a firmware release it makes sense to remove all unused IDs (development garbage) from til.json.
  - This could be done by running `trice refresh`.
- During `trice update` TRICE macros, commented out, are treated in the same way as active TRICE macros. Even after deletion their content stays inside til.json. This is intensionally to get best stability.
- The trice tool does treat trice statements inside comments or excluded by compiler switches also.

## ID management options

- The trice ID 0 is a placeholder for "no ID", which is replaced automatically during the next `trice update` according to the used trice switches `-IDMethod`, `-IDMin` and `IDMax`.
  - It is sufficient to write the TRICE macros just without the `Id(0),`. It will be inserted automatically.
- When a TRICE macro got an ID, it is not changed anymore normally. Exceptions:
  - Assumed several developer working on the same project and more than one developer are using the same ID for different TRICEs. Than the later added ID is replaced by a new ID automatically. By using the default `-IDMethod random` the chance for such cases is low. Using the `-IDMin` and `IDMax` switches allows a different ID range for each developer, to avoid automatic ID replacement.
  - When the same TRICE is used several times with different IDs and `trice update -IDreuse force` is called, only the first ID is used for all identical TRICEs.
- It is possible to use several `til.json` files - for example one for each target project but it is easier to maintain only one `til.json` file for all projects.
- The `til.json` file can be deleted and later regenerated from the sources anytime. In that case you get rid of all legacy strings but it is better to keep them for compatibility reasons. Sometimes you get a target board with older firmware and without the old references you cannot read the trice logs.
- A good practice is to keep the `til.json` file under source control. To keep it clean from the daily development garbage one could delete the `til.json`, then check-out again and re-build just before check-in.
- An other option is to delete `til.json` just before a release build and then check-in the new generated `til.json`.
- With `trice zeroSourceTreeIds` all IDs in the given source tree are set to 0. This gives the option afterwards to set-up a new `til.json` according to a different `-IDMethod`, `-IDMin` and `IDMax`.

## Future ideas

- In a config file, each `trice` channel could get assigned to an ID range, allowing channel specific runtime on/off.
- Link a compressed **til.json** file into the target binary and optionally get it back long years later in a safe way.

# TRICE ID management

Normally, when using `flex[L]`encoding, no need to read this.

## ID management internals & hints

- The `flex` and `flexL` (default) encoding supports 20-bit IDs normally, so over 1 Million IDs possible. Short flex trices use 15-bit IDs. Other encodings can work with other ID sizes. The ESC encoding, a tryout implementation, uses 16-bit IDs.
- During `trice update` so far unknown IDs are added to the ID list (case new sources added).
- If an ID was deleted inside the source tree (or file removal) the appropriate ID's stays inside the ID list.
- If the same ID appears again the appropriate ID is aktive again.
- If duplicate ID's with different format strings found inside the source tree (case several developers) one ID is replaced by a new ID. The probability for such case is low, because of the random ID generation. Also it is possible to split the ID space between several developers using `-IDMin` and `-IDMax` or `-IDMinShort` and `-IDMaxShort` .
- If the format string was modified, the ID stays in the list and a new ID for the changed format string is generated.
- Keeping obsolete IDs makes it more comfortable during development to deal with different firmware variants at the same time.
- This way you can simply copy a TRICE* statement and modify it without dealing with the ID. The trice tool will do for you.
- The ID list should go into the version control repository of your project.
- Just before check-in the ID list, one could discard ID list changes and run `trice refresh` to get rid of the dayly development garbage.
- For a firmware release it makes sense to remove all unused IDs (development garbage) from til.json.
  - This could be done by running `trice refresh`.
- During `trice update` TRICE macros commented out are treated in the same way as actice TRICE macros. Even after deletion their content stays inside til.json. This is intensionally to get best stability.
- The trice tool does treat trice statements inside comments or excluded by compiler switches also.

## ID management options

- The trice ID 0 is a placeholder for "no ID", which is replaced automatically during the next `trice update` according to the used trice `-IDMethod`, `-IDMin[Short]` and `IDMax[Short]`.
  - It is sufficient to write the TRICE macros just without the `Id(0),`. It will be inserted automatically.
  - Short trices need a lower case 'id()' but the trice tool will isert it automatically.
- When a TRICE macro got an ID, it is not changed anymore normally. Exceptions:
  - Assumed several developer working on the same project and more than one developer are using the same ID for different TRICEs. Than the later added ID is replaced by a new ID automatically. By using the default `-IDMethod random` the chance for such cases is low. Using `-IDMin[Short]` and `IDMax[Short]` allowes a different ID range for each developer, to avoid automatic ID replacement.
  - When the same TRICE is used several times with different IDs and `trice update -IDreuse force` is called, only the first ID is used for all identical TRICEs.
- It is possible to use several `til.json` files - for example one for each target project but it is easier to mantain only one `til.json` file for all projects.
- The `til.json` file can be deleted and later regenerated from the sources anytime. In that case you get rid of all legacy strings but it is better to keep them for compability reasons. Sometimes you get a target board with older firmware and without the old references you cannot read the trice logs.
- A good practice is to keep the `til.json` file under source control. To keep it clean from the daily development garbage one could delete the `til.json`, then check-out again and re-build just before check-in.
- An other option is to delete `til.json` just before a release build and then check-in the new generated `til.json`.
- The **flex[L]** encodings support 24-bit IDs, what gives an ID space of 1048574 IDs. This way even in a bigger project with, lest say 1000 different TRICEs, the chance to hit a false ID in out-of-sync case is less 1 promille.
- With `trice zeroSourceTreeIds` all IDs in the given source tree are set to 0. This gives the option afterwards to set-up a new `til.json` according to a differend `-IDMethod`, `-IDMin[Short]` and `IDMax[Short]`.

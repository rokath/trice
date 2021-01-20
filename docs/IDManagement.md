# ID management options

- The trice ID 0 is a placeholder for "no ID", which is replaced automatically during the next `trice update` according to the used trice `-IDMethod`, `-IDLowerBound` and `IDUpperBound`.
  - It is sufficient to write the TRICE macros just without the `Id(0),`. It will be inserted automatically.
- When a TRICE macro got an ID, it is not changed anymore normally. Exceptions:
  - Assumed several developer working on the same project and more than one developer are using the same ID for different TRICEs. Than the later added ID is replaced by a new ID automatically. By using the default `-IDMethod random` the chance for such cases is low. Using `-IDLowerBound` and `IDUpperBound` allowes a different ID range for each developer, to avoid automatic ID replacement.
  - When the same TRICE is used several times with different IDs and `trice update -IDreuse force` is called, only the first ID is used for all identical TRICEs.

- It is possible to use several `til.json` files - for example one for each target project but it is easier to mantain only one `til.json` file for all projects.

- The `til.json` file can be deleted and later regenerated from the sources anytime. In that case you get rid of all legacy strings but it is better to keep them for compability reasons. Sometimes you get a target board with older firmware and without the old references you cannot read the trice logs.

- A good practice is to keep the `til.json` file under source control. To keep it clean from the daily development garbage one could delete the `til.json`, then check-out again and re-build just before check-in.

- An other option is to delete `til.json` just before a release build and then check-in the new generated `til.json`.

- The **pack2[L]** encodings support 24-bit IDs, what gives an ID space of 1048574 IDs. This way even in a bigger project with, lest say 1000 different TRICEs, the chance to hit a false ID in out-of-sync case is less 1 promille.

- With `trice zeroSourceTreeIds` all IDs in the given source tree are set to 0. This gives the option afterwards to set-up a new `til.json` according to a differend `-IDMethod`, `-IDLowerBound` and `IDUpperBound`.

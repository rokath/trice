# TRICE ID management hints

> _(Read only you are interested in)_

<!-- vscode-markdown-toc -->
* 1. [Build process](#Buildprocess)
* 2. [*Trice* ID Numbers](#TriceIDNumbers)
	* 2.1. [ID number selection](#IDnumberselection)
	* 2.2. [ID number usage](#IDnumberusage)
	* 2.3. [ID number stability](#IDnumberstability)
	* 2.4. [*Trice* ID 0](#TriceID0)
* 3. [*Trices* inside source code](#Tricesinsidesourcecode)
	* 3.1. [*Trices* in source code comments](#Tricesinsourcecodecomments)
	* 3.2. [Different IDs for same *Trices*](#DifferentIDsforsameTrices)
	* 3.3. [Same IDs for different *Trices*](#SameIDsfordifferentTrices)
	* 3.4. [Adding legacy sources with `trice refresh`](#Addinglegacysourceswithtricerefresh)
* 4. [ID reference list **til.json**](#IDreferencelisttil.json)
	* 4.1. [**til.json** Version control](#til.jsonVersioncontrol)
	* 4.2. [Long Time availability](#LongTimeavailability)

<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

##  1. <a name='Buildprocess'></a>Build process

A *Trice* **ID** is needed as shown in the table:

| Write              | After `trice update`          | Remark                                               |
|--------------------|-------------------------------|------------------------------------------------------|
| `TRICE( "Hi!\n");` | `TRICE( Id(12345), "Hi!\n"),` | Run `trice update` automatically in a prebuilt step. |

* A pre-build step `trice update` generates the `Id(12345)` part. Examples:
  * `trice u` in your projects root expects a til.json file there and checks sources and **til.json** for changes to update.
  * `trice u -v -i ../../../til.json -src ../src -src ../lib/src -src ./` is a typical case as automated pre-build step in your project settings telling **trice** to scan the project dir and two external directories. Even `trice u` is fast, it is generally quicker to search only relevant places.

##  2. <a name='TriceIDNumbers'></a>*Trice* ID Numbers
###  2.1. <a name='IDnumberselection'></a>ID number selection

* The default encoding TREX supports 13-bit IDs, so over 8000 IDs possible. Other encodings can work with other ID sizes.
* The **Id** `1234` is a number assigned to `TRICE( "Hi!\n");` in the above example.
  * It is a so far unused number, according to rules you can control:
    * The `-IDMethod` switch allows a selection method for new IDs.
      * Per default new IDs determined randomly to keep the chance low, that several developers grab the same ID.
      * Example: `trice update -IDMin 1000 -IDMethod upward` will choose the smallest free ID >= 1000.
        * This allows to use the ID space without wholes.
    * The `-IDMin` and `-IDMax` switches are usable to control the ID range, a new ID is selected from, making it possible to divide the ID space. Each developer can gets it region.
      * Example: `trice update -IDMin 6000 -IDMax 6999` will choose new randomly IDs only between 6000 and 6999.
* In a future **trice** tool it can be possible to give each *trice* channel an **ID** range making it possible to implement *Trice* channel specific runtime on/off on the target side if that is needed.

###  2.2. <a name='IDnumberusage'></a>ID number usage

* If you write `TRICE( "Hi!\n");` again on a 2nd location, it gets the same or a different **ID** - as you decide.
  * Default is different: `trice u` assigns a new ID to the same *Trice*.
  * Use  `trice u -sharedIDs` to force ID re-use for added identical *Trices*.
  * `TRICE8( "msg:%d", 1);` and `TRICE16( "msg:%d", 1);` are different *Trices* even the format strings identical.

###  2.3. <a name='IDnumberstability'></a>ID number stability

* IDs stay constant and get only changed to solve conflicts.
* To make sure, a single ID will not be changed, you could change it manually to a hexadecimal syntax.
  * This lets the `trice update` command ignore such `TRICE` macros and therefore a full [til.json](../til.json) rebuild will not add them anymore. Generally this should not be done, because this could cause future bugs.
  * It is possible to assign an ID manually as decimal number. It will be added to the ID list automatically during the next `trice u`.
* If a *Trice* was deleted inside the source tree (or file removal) the appropriate ID stays inside the ID list.
* If the same ID appears again this ID is active again.

###  2.4. <a name='TriceID0'></a>*Trice* ID 0

* The trice ID 0 is a placeholder for "no ID", which is replaced automatically during the next `trice update` according to the used trice switches `-IDMethod`, `-IDMin` and `IDMax`.
  * It is sufficient to write the TRICE macros just without the `id(0),` `Id(0),` `ID(0),`. It will be inserted automatically according the `-stamp` switch.
* With `trice zeroSourceTreeIds` all IDs in the given source tree are set to 0. This gives the option afterwards to set-up a new `til.json` according to a different `-IDMethod`, `-IDMin` and `IDMax`.

##  3. <a name='Tricesinsidesourcecode'></a>*Trices* inside source code

###  3.1. <a name='Tricesinsourcecodecomments'></a>*Trices* in source code comments

* `TRICE` macros commented out, are visible for the `trice update` command and therefore regarded.
  * Example: `// TRICE( Id(12345), "Hi!\n" );` is still regarded by the `trice u`.
* During `trice update` TRICE macros, commented out, are treated in the same way as active TRICE macros. Even after deletion their content stays inside til.json. This is intensionally to get best stability across several firmware versions or variants.
* The trice tool does treat trice statements inside comments or excluded by compiler switches also.

###  3.2. <a name='DifferentIDsforsameTrices'></a>Different IDs for same *Trices*

* When the same *Trice* is used several times with different IDs and `trice update -IDreuse force` is called, only the one ID is used for all identical TRICEs in the source code. The other IDs stay inside **til.json** until they are removed (`trice renew`).

###  3.3. <a name='SameIDsfordifferentTrices'></a>Same IDs for different *Trices*

* If duplicate ID's with different format strings found inside the source tree (case several developers or source code merging) one ID is replaced by a new ID. The probability for such case is low, because of the default random ID generation.
* Also you can simply copy a *Trice* statement and modify it without dealing with the ID.
* The **trice** tool will detect the 2nd (or 3rd) usage of this ID and assign a new one also extending the ID list.
* That is done silently for you during the next `trice update`.

###  3.4. <a name='Addinglegacysourceswithtricerefresh'></a>Adding legacy sources with `trice refresh`

When including legacy library code in several different projects, each with its own **til.json** ID reference list you probably do not want the IDs inside the library code be changed. If you missed the option to use prober ID ranges starting your project  `trice refresh` could help:

```bash
sub-command 'r|refresh': For updating ID list from source files but does not change the source files.
        "trice refresh" will parse source tree(s) for TRICE macros, and refresh/generate the JSON list.
        This command should be run on adding source files to the project before the first time "trice update" is called.
        If the new source files contain TRICE macros with IDs these are added to til.json if not already used.
        Already used IDs are reported, so you have the chance to remove them from til.son and then do "trice u" again.
        This way you can make sure to get the new sources unchanged in your list.
        Already used IDs are replaced by new IDs during the next "trice update", so the old IDs in the list will survive.
        If you do not refresh the list after adding source files and perform an "trice update" new generated IDs could be equal to
        IDs used in the added sources with the result that IDs in the added sources could get changed what you may not want.
        Using "trice u -IDMethod random" (default) makes the chance for such conflicts very low.
        The "refresh" sub-command has no mandatory switches. Omitted optional switches are used with their default parameters.
```

##  4. <a name='IDreferencelisttil.json'></a>ID reference list **til.json**

* The `trice update` command demands a **til.json** file - it will not work without it. That is a safety feature to avoid unwanted file generations. If you sure to create a new **til.json** file create an empty one: `touch til.json`.
* The name **til.json** is a default one. With the command line parameter `-i` you can use any filename.
* It is possible to use several **til.json** files - for example one for each target project but it is easier to maintain only one **til.json** file for all projects.
* The ID reference list keeps all obsolete IDs with their format strings allowing compatibility to former firmware versions.
* One can delete the ID reference list. It will be reconstructed automatically from the source tree with the next `trice update` command, but history is lost then.
* Keeping obsolete IDs makes it more comfortable during development to deal with different firmware variants at the same time.

###  4.1. <a name='til.jsonVersioncontrol'></a>**til.json** Version control

* The ID list should go into the version control repository of your project.
* To keep it clean from the daily development garbage one could delete the **til.json**, then check-out again and re-build just before check-in. A small script could do that.
* For a firmware release it makes sense to remove all unused IDs from til.json.
  * This could be done by running `trice renew`. That is the same as deleting the **til.json** contents and running `trice u`.
* An other option is to delete **til.json** just before a release build and then check-in the new generated **til.json**.

###  4.2. <a name='LongTimeavailability'></a>Long Time availability

* You could place a download link for the **trice** tool and the used **til.json** list.
* Link a compressed/encrypted **til.json** file into the target binary and optionally get it back long years later in a safe way.
* Optionally add the (compressed/encrypted) ID reference list as resource into the target FLASH memory to be sure not to loose it in the next 20 years.

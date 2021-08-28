
# `trice` Config file (not implemented yet, just ideas)
 
- All following information about config files are just ideas.
- The config file can contain one or several sub-commands and flags:
  - If exactly one sub-command in config file: No need to to be specified in the command line.
- The shortcut names are more for the command line and the long names more for the config files.
- Flags need to be associated to a sub-command inside config file.

### Config file options (not implemented yet)

- The **trice** command accepts one and only one sub-command (which can be inside the config file) followed by optional flags: `trice [s|subcommand [-f|flag [value]] [...]]`
- The flags start with a single dash and are boolean or need a value separated by space or `=`.
- Flags in square brackets `[]` are optional. If omitted they have a default value depending on the context.
- Some flags can occur several times. Check with `trice h` for multi-flags.
- Command line sub-command and flags are overwriting config file values.
- A sub-command in the command line is the selected sub-command in config files if there is any.
- You can place different sub-commands with flags in a config file and specify only a sub-command in the command line for selection, like `trice u` and `trice l`.
- `trice l [...]` needs to be started only once for a development session. For example when running `trice u [...]` (in the toolchain), the still active trice logger detects til.json changes and re-reads the list automatically.

### `trice [...]`

Search for config files named *trice.conf* and use the settings there. The search goes on top down places. A value in a more down config overloads previous value settings. Command line values have highest precedence. The top down search places are:

- Directory of `trice` executable
- /
- /current/
- /current/path/
- /current/path/dir/

### `trice h|help [-c|config cfilename] [-i|idlist idfilename]`

Display help and locations of config files named *trice.conf* and all list files *til.json* occurrences in top down config search. If *-c* and|or *-l* is specified, the appropriate filenames are used instead.

- The `-config` option is not implemented yet.
- The `-idlist` option is not implemented yet.

### `trice c|config filename [...]`

Use filename as config file and ignore all config files named *trice.conf*. If filename is *[path/]trice.conf* no other *trice.conf* is evaluated and it is searched only in the current directory or the specified path location. The config file must contain one and only one subcommand or a subcommand must be inside the command line.

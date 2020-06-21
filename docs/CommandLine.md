
# trice Command Line switches
## Current state
- No config file implemented yet.
- All following information about config files are just ideas.
- The config file can contain no, one or several subcommands:
  - If exactly one subcommand in config file: No need to to be specified in the commandline.
## Options
- The **trice** command accepts one sub-command followed by optional parameters: `trice [subcommand [-parameter [value]] [...]]`
- Parameters in square brackets `[]` are optional. If omitted they have a default value. 
- Parameter options [...] are overwriting config file values.

### `trice [...]`
Search for config files named *trice.conf* and use the settings there. The search goes  on top down places. Last config overloads previous settings. Command line values have highest precedence. The top down search places are:
- Directory of `trice` executable
- /
- /current/
- /current/path/
- /current/path/dir/

### `trice c|config filename [...]`
Use filename as config file and ignore all config files named *trice.conf*. If filename is *[path/]trice.conf* no other *trice.conf* is evaluated and it is searched only in the current directory or the specified path location. The config file must contain one and only one subcommand or a subcommand must be inside [...].

### `trice h|help [-c|config filename]`
Display help and locations of config files named trice.conf or all filename occurences in top down config search.

### `trice u|update [-dry-run] [-v|verbose] [-l|list filename] [-src dirname -src filename ...]`
Update IDs in scource code and ID list.
- `[-dry-run]` Do nothing, just show the output.
- `[-v|verbose]` Show more output.
- `[-l|list filename]` Use filename as ID listfile. Default is *til.json* first found on the config file search places.
- `[-src dirname -src filename ...]` Scan all files in dirname and also filename. This is a multi parameter.

###  `trice v|version [-lf|logfile off|filename]`
Show trice version. To log version information in a file use -lf filename. Default is off. This can be useful for automatic logging.

### `trice s|scan`
Scan for available COMports.

### `trice ds|displayserver [-lf|logfile off|filename] [-c|color [off|alternate]] [-s|source IPaddr:IPport]`
Start as display server which can serve as destination for several trice instances.
- `[-lf|logfile off|filename]`
    - Default filename is "2006-01-02_1504-05_trice.log" with the current time. To switch logging off set to "off".
- `[-c|color [off|alternate]]`
  - Switch color off or use alternate color set.

### `trice sd|shutdownServer [-d|destination IPaddr:IPport]`
Send control command to display server for self-shutdown.
This can be handy if display server is not visible or on a remote mashine.

### `trice l|log [-c|config off|filename] [-l|list filename] [-lf|logfile off|filename] [-c|color [off|alternate]] [-format bare|wrap] [-k|key password [-show]] [-prefix off|string][-postfix off|string] [-s|source COM|filename|RND|SIM [-param string]] [-r|remote ds|displayserver|IPaddr:IPport] [-a|autostart cmd]`
Log trice messages from -source and display them line by line. Default destination is current display.
- `[-c|config off|filename]` Default filename is *trice.conf*. It is searched on top down places. Last config overloads previous settings. Commandline params overload config settings and "off" ignores *trice.conf* files.
- `[-lf|logfile off|filename]` Default filename is `2006-01-02_1504-05_trice.log` with current time. To switch logging off set to "off". When -destination switch is active the default is "off", because the destination logs then.
- `[-format bare|wrap]` Default format is bare. That means a basic trice message is 4 bytes: 2 bytes ID and 2 bytes value. It is faster on the target, but cyclic sync messages are reccommended (**TRICE_SYNC**). The wrap format contains bare and 4 bytes additional control information. This allowes automatic host syncing and additional protocols on the same channel.
- `[-k|key password [-show]]` Decrypt encrypted trice messages if password is not "none". The switch -show displays the passphrase.
- `[-prefix off|string]` Add string as prefix to begin of each trice log line. It defaults to the source channel and can suppressed with "off".
- `[-postfix off|string]` Add string as postfix to end of each trice log line. It defaults to the "\n" channel and can suppressed with "off".
- `[-s|source COM|filename|RND|SIM [-param string]]` Use source as trice input. The switch -param is accordingly to source.
  - COMn: COM5 as example. -param "-baud 115200" is default value
  - filename: When filename is an executable it is started with a random temporary logfile, internally read from. -param is accordingly to the executable commadline:
    - When filename is a binary file this file is read and data interpreted according to -format switch.
    - When filename is an ASCII file, lines are diplayed. Can be used with logfiles. Default logging is off in that case.
    - RND: random data are used as input and interpreted according to -format switch. The switch -param is used as control for the random generator.
    - SIM: inputDummy data are used as input and interpreted according to -format switch. The switch -param is used as control for the inputDummy generator. Usable for testing.
- `[-c|color [off|alternate]]` Switch color off or use alternate color set. Ignored when switch -remote is used.
- `[-r|remote ds|displayserver|IPaddr:IPport]` Do not show loglines and send them to a displayserver. Default is localhost:61497.
- `[-a|autostart cmd]` Start `cmd` in background. Try `trice ds` to start displayserver automatcally.
## EXAMPLES:
		 trice log -source COMn -destination localhost:61497
		 trice log -soucre localhost:61497
		 trice displayserver -source localhost:61497
		 trice log -source COMn -param "-baud 115200" -format bare -destination filename
		 trice log -source COMn -baud 115200 -format wrap -destination filename
		 trice log -source filename -format bare -destination filename
		 trice log -source filename -format wrap -destination filename
		 trice log -source C:\\repos\\trice\\third_party\\JLinkRTTLogger.exe" -param "-Device STM32F030R8 -if SWD -RTTChannel 0" -format bare -destination filename
		 trice log -source JLINK -param "-Device STM32F030R8 -if SWD -RTTChannel 0" -format bare -destination filename
		 trice log -source STRTT -param "" -format bare -destination filename
		 trice log -config filename

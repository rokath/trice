# *Trice* configuration

##  1. <a name='TableofContents'></a>Table of Contents

<!-- vscode-markdown-toc -->
* 1. [Table of Contents](#TableofContents)
* 2. [Target](#Target)
	* 2.1. [Target side *Trice* On-Off](#TargetsideTriceOn-Off)
* 3. [Host](#Host)
	* 3.1. [Host side *Trice* On-Off](#HostsideTriceOn-Off)
* 4. [Host Configuration file (Not implemented!)](#HostConfigurationfileNotimplemented)
	* 4.1. [Config file options](#Configfileoptions)
	* 4.2. [`trice [...]`](#trice...)
	* 4.3. [`trice h|help [-c|config cfilename] [-i|idlist idfilename]`](#tricehhelp-cconfigcfilename-iidlistidfilename)
	* 4.4. [`trice c|config filename [...]`](#tricecconfigfilename...)
	* 4.5. [`trice` config file *trice.conf.json*](#triceconfigfiletrice.conf.json)
		* 4.5.1. [Reading stuff](#Readingstuff)
		* 4.5.2. [Rules](#Rules)
		* 4.5.3. [Config file search and parameters priority](#Configfilesearchandparameterspriority)

<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

##  2. <a name='Target'></a>Target Configuration 

All project specific settings are done inside [triceConfig.h](https://github.com/rokath/trice/tree/master/pkg/src/intern/triceConfig.h) which one should copy into the project folder. The [trice.h](../pkg/src/trice.h) and [trice.c](../pkg/src/trice.c) are mend to be used unchanged in several projects.

###  2.1. <a name='TargetsideTriceOn-Off'></a>Target side *Trice* On-Off

* If your code works well after checking, you can add `#define TRICE_OFF` just before the `#include "trice.h"` line and no *trice* code is generated anymore for that file, so no need to delete or comment out `TRICE` macros.
* No runtime On-Off switch is implemented for  several reasons:
  * Would need a control channel to the target.
  * Would add little performance and code overhead.
  * Would sligtly change target timing (testing).
  * User can add its own switches anywhere.
  * The short `TRICE` macro code is negligible.
  * The trice output is encryptable, if needed.
* Because of the low *Trice* bandwidth needs and to keep the target code as clear as possible the runtime On-Off decision should be done by the **trice** tool.
* See also issue [#243](https://github.com/rokath/trice/issues/243).

##  3. <a name='Host'></a>Host

The **trice** tool supports many [command line switches](./TriceUsageGuide.md#9-options-for-trice-tool) which are usable also inside of batch scripts.

###  3.1. <a name='HostsideTriceOn-Off'></a>Host side *Trice* On-Off

* The PC **trice** tool offers command line switches to `-pick` or `-ban` for *trice* channels and will be extended with display switches.
* A **trice** tool logLevel switch is possible too (Issue [#236](https://github.com/rokath/trice/issues/236)).

##  4. <a name='HostConfigurationfileNotimplemented'></a>Host Configuration file (Not implemented!)
 ---

* All following information about config files are just ideas.
* The config file can contain one or several sub-commands and flags:
  * If exactly one sub-command in config file: No need to to be specified in the command line.
* The shortcut names are more for the command line and the long names more for the config files.
* Flags need to be associated to a sub-command inside config file.

###  4.1. <a name='Configfileoptions'></a>Config file options

* The **trice** command accepts one and only one sub-command (which can be inside the config file) followed by optional flags: `trice [s|sub-command [-f|flag [value]] [...]]`
* The flags start with a single dash and are boolean or need a value separated by space or `=`.
* Flags in square brackets `[]` are optional. If omitted they have a default value depending on the context.
* Some flags can occur several times. Check with `trice h` for multi-flags.
* Command line sub-command and flags are overwriting config file values.
* A sub-command in the command line is the selected sub-command in config files if there is any.
* You can place different sub-commands with flags in a config file and specify only a sub-command in the command line for selection, like `trice u` and `trice l`.
* `trice l [...]` needs to be started only once for a development session. For example when running `trice u [...]` (in the toolchain), the still active trice logger detects til.json changes and re-reads the list automatically.

###  4.2. <a name='trice...'></a>`trice [...]`

Search for config files named *trice.conf* and use the settings there. The search goes on top down places. A value in a more down config overloads previous value settings. Command line values have highest precedence. The top down search places are:

* Directory of `trice` executable
* /
* /current/
* /current/path/
* /current/path/dir/

###  4.3. <a name='tricehhelp-cconfigcfilename-iidlistidfilename'></a>`trice h|help [-c|config cfilename] [-i|idlist idfilename]`

Display help and locations of config files named *trice.conf* and all list files *til.json* occurrences in top down config search. If *-c* and|or *-l* is specified, the appropriate filenames are used instead.

* The `-config` option is not implemented yet.
* The `-idlist` option is not implemented yet.

###  4.4. <a name='tricecconfigfilename...'></a>`trice c|config filename [...]`

Use filename as config file and ignore all config files named *trice.conf*. If filename is *[path/]trice.conf* no other *trice.conf* is evaluated and it is searched only in the current directory or the specified path location. The config file must contain one and only one sub-command or a sub-command must be inside the command line.

###  4.5. <a name='triceconfigfiletrice.conf.json'></a>`trice` config file *trice.conf.json*

####  4.5.1. <a name='Readingstuff'></a>Reading stuff

[https://dev.to/ilyakaznacheev/a-clean-way-to-pass-configs-in-a-go-application-1g64](https://dev.to/ilyakaznacheev/a-clean-way-to-pass-configs-in-a-go-application-1g64)

####  4.5.2. <a name='Rules'></a>Rules

* The `trice` command  can be called without parameters.
  * In that case it needs to find at least one config file named *trice.conf.json*.
* The `trice` command can be called with a single sub-command plus an optional dashed parameter list.
  * In that case it looks also for a config file named *trice.conf.json* for additional settings.
  * Settings in the commandline have always higher priority.
  * The search for a config file named *trice.conf.json* can be suppressed by **-no-conf**
* The `trice` command accepts a config *filename.json* followed optionally by a sub-command plus parameters.
  * In that case no search for a config file named *trice.conf.json* is done.
  * Settings in the commandline have always higher priority.

####  4.5.3. <a name='Configfilesearchandparameterspriority'></a>Config file search and parameters priority

* The trice tool starts to search for config files named *trice.conf.json* and loads them one after another in the following order with rising priority:
  * /trice/executable/folder
  * /
  * /path/
  * /path/to/
  * /path/to/current/
  * /path/to/current/folder/
  * Additional a config file can be specified in the command line as first parameter
  * command line parameters
* If the */trice/executable/folder* is part of */path/to/current/folder/*, the config file */trice/executable/folder/trice.conf.json* is loaded twice resulting the the appropriate config priority
* The last sub-command is the winner.
* Parameters of other subcommands are ignored.

# `trice` config file *trice.conf.json* (todo)

## Reading stuff

[https://dev.to/ilyakaznacheev/a-clean-way-to-pass-configs-in-a-go-application-1g64](https://dev.to/ilyakaznacheev/a-clean-way-to-pass-configs-in-a-go-application-1g64)

## Rules

- The `trice` command  can be called without parameters.
  - In that case it needs to find at least one config file named *trice.conf.json*.
- The `trice` command can be called with a single sub-command plus an optional dashed parameter list.
  - In that case it looks also for a config file named *trice.conf.json* for additional settings.
  - Settings in the commandline have always higher priority.
  - The search for a config file named *trice.conf.json* can be suppressed by **-no-conf**
- The `trice` command accepts a config *filename.json* followed optionally by a sub-command plus parameters.
  - In that case no search for a config file named *trice.conf.json* is done.
  - Settings in the commandline have always higher priority.

## Config file search and parameters priority

- The trice tool starts to search for config files named *trice.conf.json* and loads them one after another in the following order with rising priority:
  - /trice/executable/folder
  - /
  - /path/
  - /path/to/
  - /path/to/current/
  - /path/to/current/folder/
  - Additional a config file can be specified in the command line as first parameter
  - command line parameters
- If the */trice/executable/folder* is part of */path/to/current/folder/*, the config file */trice/executable/folder/trice.conf.json* is loaded twice resulting the the appropriate config priority
- The last sub-command is the winner.
- Parameters of other subcommands are ignored.
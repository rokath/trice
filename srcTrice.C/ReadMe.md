# C sources (instrumentation code)
## Needed stuff
- trice.h must be included in source files using TRICE macros.
- trice.c needs to be added to your project if trices are enabled.
- triceConfig.h should be copied separately in each project. It contains project specific settings.
## Add Ons
- triceCheck.* is just for demonstration purposes and not needed.
- xteaCrypto.c is needed as part of your project only if you wich to use the encryption option.
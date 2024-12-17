JLinkGDBServer \
-device STM32G0B1RE \
-if SWD \
-speed 4000 \
-notimeout \
-noir \
-nohalt \
-noreset \
-RTTTelnetPort 19021 \

# JLinkGDBServer \
# -endian little \
# -nolocalhostonly \
# -port 3333 \
# -telnetport 2333 \
# -notimeout \
# -select USB \
# -strict \
# -RTTTelnetPort 4444 \

# -settingsfile Selects the J-Link Settings File.
# -jlinkscriptfile “C:\My.JLinkScript” (7.13)
# -x Executes a gdb file on first connection.
# -xc Executes a gdb file on every connection.

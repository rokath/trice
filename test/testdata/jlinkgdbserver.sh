JLinkGDBServer \
-device STM32F030R8 \
-if SWD \
-speed 4000 \
-notimeout \
-noir \
-nohalt \
-noreset \
-RTTTelnetPort 19021 \



# JLinkGDBServer \
# -device STM32F030R8 \
# -endian little \
# -if SWD \
# -speed 4000 \
# -nolocalhostonly \
# -port 3333 \
# -telnetport 2333 \
# -noir \
# -nohalt \
# -notimeout \
# -noreset \
# -select USB \
# -strict \
# -RTTTelnetPort 4444 \

# -settingsfile Selects the J-Link Settings File.
# -jlinkscriptfile “C:\My.JLinkScript” (7.13)
# -x Executes a gdb file on first connection.
# -xc Executes a gdb file on every connection.

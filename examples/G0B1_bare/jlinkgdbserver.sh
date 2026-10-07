#!/usr/bin/env bash
# Start a SEGGER J-Link GDB server for this example's connected board.
# Usage: ./jlinkgdbserver.sh. Requires the JLinkGDBServer command in PATH.
# Leave this terminal open and connect your debugger from another terminal/IDE.
# -device selects the MCU; -if SWD selects the debug connection.
# -speed 4000 selects a 4 MHz debug clock; the RTT telnet port is 19021.
# -nohalt and -noreset keep this startup from stopping or resetting the firmware.
# Stop the server with Ctrl-C. Change the device only when using another MCU.

JLinkGDBServer \
  -device STM32G0B1RE \
  -if SWD \
  -speed 4000 \
  -notimeout \
  -noir \
  -nohalt \
  -noreset \
  -RTTTelnetPort 19021

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

dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.68)
AC_INIT([cobs], [0.5.0]) #set project name and version

# Put autotools auxiliary files in subdirectories to reduce clutter:
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CC

#AC_CANONICAL_SYSTEM

# Put configuration results here, so we can easily #include them:
AC_CONFIG_HEADERS([config.h])

#version numbers

AC_SUBST([LIB_SO_VERSION], [2:0:0])

# Enable "automake" to simplify creating makefiles:
AM_INIT_AUTOMAKE([foreign subdir-objects -Wall -Werror -Wno-portability])
AM_SILENT_RULES([yes])
AC_CONFIG_FILES([Makefile
		cobs.pc])

#dnl this allows us specify individual linking flags for each target
AM_PROG_CC_C_O

dnl Initialize Libtool
LT_INIT

dnl Check if Libtool is present
dnl Libtool is used for building share libraries
AC_PROG_LIBTOOL

AC_OUTPUT


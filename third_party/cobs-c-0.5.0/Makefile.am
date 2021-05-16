
#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = lib@PACKAGE_NAME@.la

#######################################
# Build information for each library

library_include_at_parsedir=$(includedir)/@PACKAGE_NAME@
library_include_at_parse_HEADERS = cobs.h cobsr.h
lib@PACKAGE_NAME@_la_SOURCES = cobs.c cobsr.c
lib@PACKAGE_NAME@_la_LDFLAGS = -version-info @LIB_SO_VERSION@

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = @PACKAGE_NAME@.pc


#######################################
# Tests

TESTS = cobs-test

check_PROGRAMS = cobs-test

cobs_test_SOURCES = tests-c/main.c
cobs_test_LDADD = lib@PACKAGE_NAME@.la

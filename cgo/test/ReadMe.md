# Testing the `trice` C-code

<!-- vscode-markdown-toc -->
- [Testing the `trice` C-code](#testing-the-trice-c-code)
  - [1. Folder information](#1-folder-information)
  - [2. Package specific information](#2-package-specific-information)
  - [3. todo](#3-todo)

<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

##  1. <a name='Folderinformation'></a>Folder information

- The packages are helper for testing the target C-code.
- In each package a different triceConfig.h is used.

##  2. <a name='Packagespecificinformation'></a>Package specific information

- Each C function gets a Go wrapper which ist tested in appropriate test functions.
- For some reason inside the trice_test.go an 'import "C"' is not possible.
- All C-files in the packages folder referring to the trice sources this way avoiding code duplication.
- The Go functions defined in the packages are not exported. They are called by the Go test functions in this package.
- This way the package test functions are executing the trice C-code compiled with the triceConfig.h there.

##  3. <a name='todo'></a>todo

- repair cgo0_tcobs
- repair cgo1_tcobs
- cgo2_tcobs: ref_cobs.c_?
- Parse "C:\repos\trice\cgo\test\cgo2_tcobs\triceCheck.c" and complete TestTriceCheck
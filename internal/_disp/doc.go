// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package disp is for dispatching and displaying trice log lines.
//
// Currently it can be build in 3 variants: (without spaces before build)
// '- build ansi' -> use ansiOut.go (default)
// '+ build noclor' -> use noColorOut.go to disable color code
// '+ build color' -> use colorOut.go (obsolete)
package disp

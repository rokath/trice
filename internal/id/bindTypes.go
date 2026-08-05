// SPDX-License-Identifier: MIT

package id

import "os"

// bindFileClass records which tool owns the active Trice sites in one physical file.
type bindFileClass string

const (
	bindFileNone   bindFileClass = "none"
	bindFileInsert bindFileClass = "insert-owned"
	bindFileBound  bindFileClass = "bind-owned"
	bindFileMixed  bindFileClass = "mixed"
)

// bindSiteMode describes how the target-side descriptor supplies its TID expression.
type bindSiteMode string

const (
	bindSiteAuto    bindSiteMode = "TRICE_BIND_AUTO"
	bindSiteReplace bindSiteMode = "TRICE_BIND_REPLACE"
)

// bindFileInput is the immutable result of the parallel source-tree read phase.
type bindFileInput struct {
	path string
	info os.FileInfo
	data []byte
}

// bindInclude records one physical preprocessor include and, for sidecars, its identity.
type bindInclude struct {
	start     int
	end       int
	afterLine int
	line      int
	name      string
	key       string
	isSidecar bool
}

// bindSite records parser-derived source identity and the later insert-derived ID assignment.
type bindSite struct {
	line        int
	column      int
	loc         [7]int
	macro       string
	format      string
	mode        bindSiteMode
	wrapper     string
	id          TriceID
	comment     string
	wasExplicit bool
}

// bindFilePlan contains all read-only analysis and all bytes intended for the commit phase.
type bindFilePlan struct {
	path           string
	info           os.FileInfo
	original       []byte
	final          []byte
	class          bindFileClass
	includes       []bindInclude
	sites          []bindSite
	key            string
	sidecarName    string
	sidecarPath    string
	sidecarContent []byte
	includeAdded   bool
	configChanged  bool
	newIDs         int
	reusedIDs      int
	diagnostics    []bindDiagnostic
}

// bindDiagnostic is sortable so parallel analysis never changes diagnostic order.
type bindDiagnostic struct {
	path    string
	line    int
	message string
}

// bindWrite is one content-dependent atomic replacement planned after successful analysis.
type bindWrite struct {
	path string
	data []byte
	perm os.FileMode
	kind string
}

// bindOriginalFile is the rollback snapshot for one destination in the deterministic commit phase.
type bindOriginalFile struct {
	existed bool
	data    []byte
	perm    os.FileMode
}

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

// bindInclude records one physical preprocessor include and any recognized
// owner-sidecar or compact rebase-helper identity.
type bindInclude struct {
	start     int
	end       int
	afterLine int
	line      int
	name      string
	key       string
	isSidecar bool
	isRebase  bool
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

	// definitionName is non-empty only for a Trice site owned by a supported
	// statement-macro definition. The ordinal is local to that definition.
	definitionName    string
	definitionOrdinal int
	// sourceOrdinal distinguishes direct sites that share one physical line.
	sourceOrdinal int
	// counterSelected records that at least one generated invocation region
	// selects this logical site through the local counter-rebase path.
	counterSelected bool
}

// bindSiteReference identifies one logical Trice site after deterministic plan sorting.
type bindSiteReference struct {
	plan int
	site int
}

// bindMacroDefinition describes one complete physical #define directive and
// the shared-parser Trice sites found in its replacement list.
type bindMacroDefinition struct {
	name                    string
	start                   int
	end                     int
	replacementStart        int
	line                    int
	functionLike            bool
	siteIndexes             []int
	hasTokenPaste           bool
	hasFormatStringify      bool
	hasTerminatingSemicolon bool
	hasCounter              bool
}

// bindWrapperDefinition is the project-wide identity of one supported logging wrapper.
type bindWrapperDefinition struct {
	name      string
	plan      int
	macro     int
	siteRefs  []bindSiteReference
	duplicate bool
}

// bindWrapperInvocation records one standalone wrapper expansion in user source.
type bindWrapperInvocation struct {
	name            string
	start           int
	end             int
	line            int
	column          int
	comment         string
	macroTerminated bool
}

// bindSiteDescriptor maps one ordinary File-Key-plus-line expansion to a logical site.
type bindSiteDescriptor struct {
	line    int
	column  int
	ref     bindSiteReference
	comment string
}

// bindRebaseRegion describes one generated, statement-local counter scope.
type bindRebaseRegion struct {
	scope      string
	line       int
	column     int
	start      int
	end        int
	expansions []bindSiteReference
}

// bindRebaseArtifact describes one generated helper header that moves verbose
// preprocessor state management out of an owner source file.
type bindRebaseArtifact struct {
	name    string
	path    string
	kind    string
	scope   string
	content []byte
}

// bindProjectModel is rebuilt after every in-memory source transformation so
// all references use the final physical lines seen by the target compiler.
type bindProjectModel struct {
	wrappers map[string]bindWrapperDefinition
}

// bindFilePlan contains all read-only analysis and all bytes intended for the commit phase.
type bindFilePlan struct {
	path                    string
	info                    os.FileInfo
	original                []byte
	final                   []byte
	class                   bindFileClass
	includes                []bindInclude
	sites                   []bindSite
	macroDefinitions        []bindMacroDefinition
	invocations             []bindWrapperInvocation
	descriptors             []bindSiteDescriptor
	regions                 []bindRebaseRegion
	existingRebaseArtifacts []bindRebaseArtifact
	rebaseArtifacts         []bindRebaseArtifact
	managedOffset           int
	key                     string
	sidecarName             string
	sidecarPath             string
	sidecarContent          []byte
	includeAdded            bool
	configChanged           bool
	newIDs                  int
	reusedIDs               int
	diagnostics             []bindDiagnostic
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
	// remove distinguishes a planned deletion from an empty-file replacement.
	remove bool
}

// bindOriginalFile is the rollback snapshot for one destination in the deterministic commit phase.
type bindOriginalFile struct {
	existed bool
	data    []byte
	perm    os.FileMode
}

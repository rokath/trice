package id

import (
	"log"
	"os"
	"regexp"
)

var (
	Verbose                      bool            // Verbose gives more information on output if set. The value is injected from main packages.
	DryRun                       bool            // DryRun if set, inhibits real changes
	FnJSON                       = "til.json"    // FnJSON is the filename for the JSON formatted ID list.
	LIFnJSON                     string          // LIFnJSON is the filename for the JSON formatted location information list.
	Min                          = TriceID(1000) // Min is the smallest allowed ID for normal trices.
	Max                          = TriceID(7999) // Max is the biggest allowed ID for normal trices.
	SearchMethod                 = "random"      // SearchMethod is the next ID search method.
	LIPathKind                   string          // LIPathKind controls how to store paths inside li.json: base, relative, full
	Srcs                         ArrayFlag       // Srcs gets multiple files or directories.
	ExcludeSrcs                  ArrayFlag       // ExcludeSrcs is an ArrayFlag representing source files to be excluded from processing.
	IDRange                      ArrayFlag       // IDPolicy gets ID ranges for Trice ID message channels like "err:".
	IDData                       idData
	matchSourceFile              = regexp.MustCompile(patSourceFile)
	matchNbID                    = regexp.MustCompile(patNbID)
	matchNb                      = regexp.MustCompile(patNb)
	matchFmtString               = regexp.MustCompile(patFmtString)
	matchNextFormatSpecifier     = regexp.MustCompile(patNextFormatSpecifier)
	matchSpacesWithOptionalComma = regexp.MustCompile(patSpacesWithOptionalComma)
	ExtendMacrosWithParamCount   bool

	TriceAliases       ArrayFlag // Holds trice() aliases.
	TriceSAliases      ArrayFlag // Holds triceS() aliases.
	matchNbTRICE       *regexp.Regexp
	matchTypNameTRICE  *regexp.Regexp
	matchAnyTriceStart *regexp.Regexp

	// DefaultTriceBitWidth tells the bit width of TRICE macros having no bit width in their names, like TRICE32 or TRICE8.
	//
	// One target can use only one bit width for bare TRICE macros and the setting inside the target code must match DefaultTriceBitWidth.
	DefaultTriceBitWidth = "32"

	// DefaultStampSize is used for written uppercase TRICE macros without ID|Id|id at all, to determine
	// if `trice insert` should put ID(n) for 32 OR Id(n) for 16 OR id(n) for 0 bis stamps.
	// Normally use TRice, Trice or trice to select the stamp size or write TRICE(ID(0)... or TRICE(Id(0)... or TRICE(id(0)... .
	DefaultStampSize = 32

	// StampSizeId gets set according to the DefaultStampSize value.
	StampSizeId string

	// SpaceInsideParenthesis set true, if your code autoformatter sets a space after opening braces.
	SpaceInsideParenthesis = false

	// TriceCacheEnabled needs to be true to use the Trice cache.
	TriceCacheEnabled bool

	// UserHomeDir needs to be changed for cache tests.
	UserHomeDir string
)

const (
	insertedCacheFolderName = "inserted"
	cleanedCacheFolderName  = "cleaned"
)

func init() {
	var err error
	UserHomeDir, err = os.UserHomeDir()
	if err != nil {
		log.Fatal(err)
	}

	// Initialize matchNbTRICE, matchTypNameTRICE, matchAnyTriceStart
	ProcessAliases()
}

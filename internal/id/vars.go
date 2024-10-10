package id

import (
	"log"
	"os"
	"regexp"
)

var (
	Verbose                  bool            // Verbose gives more information on output if set. The value is injected from main packages.
	DryRun                   bool            // DryRun if set, inhibits real changes
	FnJSON                   = "til.json"    // FnJSON is the filename for the JSON formatted ID list.
	LIFnJSON                 string          // LIFnJSON is the filename for the JSON formatted location information list.
	Min                      = TriceID(1000) // Min is the smallest allowed ID for normal trices.
	Max                      = TriceID(7999) // Max is the biggest allowed ID for normal trices.
	SearchMethod             = "random"      // SearchMethod is the next ID search method.
	LiPathIsRelative         bool            // LiPathIsRelative causes to store relative paths instead of base names only inside li.json.
	Srcs                     ArrayFlag       // Srcs gets multiple files or directories.
	IDPolicy                 ArrayFlag       // IDPolicy gets ID ranges for Trice ID message channels like "err:".
	IDData                   idData
	matchSourceFile          = regexp.MustCompile(patSourceFile)
	matchNbTRICE             = regexp.MustCompile(patNbTRICE)
	matchNbID                = regexp.MustCompile(patNbID)
	matchNb                  = regexp.MustCompile(patNb)
	matchTypNameTRICE        = regexp.MustCompile(patTypNameTRICE)
	matchFmtString           = regexp.MustCompile(patFmtString)
	matchNextFormatSpecifier = regexp.MustCompile(patNextFormatSpecifier)
	//  matchFullAnyTrice        = regexp.MustCompile(patFullAnyTrice)
	//  matchTriceNoLen          = regexp.MustCompile(patTriceNoLen)
	//  matchIDInsideTrice       = regexp.MustCompile(patIDInsideTrice)
	matchAnyTriceStart = regexp.MustCompile(patAnyTriceStart)
	//matchNumber              = regexp.MustCompile(patNumber)
	//matchIncludeTriceHeader  = regexp.MustCompile(patIncludeTriceHeader)
	ExtendMacrosWithParamCount bool

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

	// SpaceAfterTriceOpeningBrace set true, if your code autoformatter sets a space after opening braces.
	SpaceAfterTriceOpeningBrace = false

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
}

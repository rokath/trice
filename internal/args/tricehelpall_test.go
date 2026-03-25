// SPDX-License-Identifier: MIT

package args

import (
	"os"
	"strings"
	"testing"
)

func TestHelpAll(t *testing.T) {
	got, err := RenderHelpText("-all")
	if err != nil {
		t.Fatalf("unexpected help error: %v", err)
	}
	for _, snippet := range []string{
		"syntax: 'trice sub-command' [params]",
		"sub-command 'ds|displayServer': Starts a display server.",
		"sub-command 'h|help': For command line usage.",
		"sub-command 'l|log': For displaying trice logs coming from port.",
		"sub-command 'a|add': Use for adding library source files containing already trice IDs to your project.",
		"sub-command 'g|gen|generate': Use for creating colors or support files.",
		"sub-command 's|scan': Shows available serial ports)",
		"sub-command 'sd|shutdown': Ends display server at IPA:IPP, works also on a remote machine.",
		"sub-command 'ver|version': For displaying version information.",
		"sub-command 'i|insert': For updating til.json and inserting IDs into source files.",
	} {
		if !strings.Contains(got, snippet) {
			t.Fatalf("help -all output misses %q\n%s", snippet, got)
		}
	}
}

func TestGeneratedHelpAllDocIsUpToDate(t *testing.T) {
	got, err := RenderHelpText("-all")
	if err != nil {
		t.Fatalf("unexpected help error: %v", err)
	}

	const generatedFile = "../../docs/ref/trice-help-all.txt"
	wantBytes, err := os.ReadFile(generatedFile)
	if err != nil {
		t.Fatalf("read generated help file %s: %v", generatedFile, err)
	}
	want := string(wantBytes)

	if got != want {
		t.Fatalf("generated help file %s is outdated, run 'go generate ./internal/args'", generatedFile)
	}
}

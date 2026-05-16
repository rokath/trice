// SPDX-License-Identifier: MIT

package id_test

import (
	"testing"

	"github.com/rokath/trice/internal/args"
	"github.com/rokath/trice/internal/id"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// TestIssue648InsertCleanPreservesOpeningWhitespace verifies that generated IDs
// do not normalize user whitespace directly after the opening Trice parenthesis.
func TestIssue648InsertCleanPreservesOpeningWhitespace(t *testing.T) {
	tests := []struct {
		name         string
		source       string
		insertFlags  []string
		cleanFlags   []string
		wantInserted string
		wantCleaned  string
	}{
		{
			name:         "newline",
			source:       "trice(\n\"message must be in new line\");",
			wantInserted: "trice(\niD(999), \"message must be in new line\");",
			wantCleaned:  "trice(\n\"message must be in new line\");",
		},
		{
			name:         "single_space",
			source:       "trice( \"message keeps leading space\");",
			wantInserted: "trice( iD(999), \"message keeps leading space\");",
			wantCleaned:  "trice( \"message keeps leading space\");",
		},
		{
			name:         "tab",
			source:       "trice(\t\"message keeps leading tab\");",
			wantInserted: "trice(\tiD(999), \"message keeps leading tab\");",
			wantCleaned:  "trice(\t\"message keeps leading tab\");",
		},
		{
			name:         "compact_default",
			source:       "trice(\"message stays compact\");",
			wantInserted: "trice(iD(999), \"message stays compact\");",
			wantCleaned:  "trice(\"message stays compact\");",
		},
		{
			name:         "wide_keeps_existing_space",
			source:       "trice( \"wide keeps existing space\");",
			insertFlags:  []string{"-w"},
			cleanFlags:   []string{"-w"},
			wantInserted: "trice( iD( 999 ), \"wide keeps existing space\");",
			wantCleaned:  "trice( \"wide keeps existing space\");",
		},
		{
			name:         "wide_adds_fallback_space",
			source:       "trice(\"wide adds fallback space\");",
			insertFlags:  []string{"-w"},
			cleanFlags:   []string{"-w"},
			wantInserted: "trice( iD( 999 ), \"wide adds fallback space\");",
			wantCleaned:  "trice( \"wide adds fallback space\");",
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			defer id.Setup(t)()

			src := tt.name + ".c"
			require.NoError(t, id.FSys.WriteFile(src, []byte(tt.source), 0o777))

			insertArgs := append([]string{
				"trice", "insert",
				"-src", src,
				"-IDMin", "100",
				"-IDMax", "999",
				"-IDMethod", "downward",
				"-til", id.FnJSON,
				"-li", id.LIFnJSON,
			}, tt.insertFlags...)
			require.NoError(t, args.Handler(id.W, id.FSys, insertArgs))

			inserted, err := id.FSys.ReadFile(src)
			require.NoError(t, err)
			assert.Equal(t, tt.wantInserted, string(inserted))

			cleanArgs := append([]string{
				"trice", "clean",
				"-src", src,
				"-til", id.FnJSON,
				"-li", id.LIFnJSON,
			}, tt.cleanFlags...)
			require.NoError(t, args.Handler(id.W, id.FSys, cleanArgs))

			cleaned, err := id.FSys.ReadFile(src)
			require.NoError(t, err)
			assert.Equal(t, tt.wantCleaned, string(cleaned))
		})
	}
}

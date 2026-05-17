// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestIssue634MaskTriceInsertDisabledRegions(t *testing.T) {
	tests := []struct {
		name          string
		source        string
		wantContains  []string
		wantNotFound  []string
		wantNewlineCt int
	}{
		{
			name: "off_on_section",
			source: strings.Join([]string{
				`trice("before");`,
				`// TRICE_INSERT_OFF`,
				`trice("skip");`,
				`// TRICE_INSERT_ON`,
				`trice("after");`,
			}, "\n"),
			wantContains:  []string{triceInsertOffMarker, triceInsertOnMarker, `trice("before");`, `trice("after");`},
			wantNotFound:  []string{`trice("skip");`},
			wantNewlineCt: 4,
		},
		{
			name: "off_to_file_end",
			source: strings.Join([]string{
				`trice("before");`,
				`TRICE_INSERT_OFF`,
				`trice("skip");`,
			}, "\n"),
			wantContains:  []string{triceInsertOffMarker, `trice("before");`},
			wantNotFound:  []string{`trice("skip");`},
			wantNewlineCt: 2,
		},
		{
			name: "marker_inside_string_is_ignored",
			source: strings.Join([]string{
				`puts("TRICE_INSERT_OFF");`,
				`trice("kept");`,
			}, "\n"),
			wantContains:  []string{`puts("TRICE_INSERT_OFF");`, `trice("kept");`},
			wantNewlineCt: 1,
		},
		{
			name: "identifier_prefix_is_ignored",
			source: strings.Join([]string{
				`MY_TRICE_INSERT_OFF`,
				`trice("kept");`,
			}, "\n"),
			wantContains:  []string{`MY_TRICE_INSERT_OFF`, `trice("kept");`},
			wantNewlineCt: 1,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got := maskTriceInsertDisabledRegions(tt.source)
			assert.Len(t, got, len(tt.source))
			assert.Equal(t, tt.wantNewlineCt, strings.Count(got, "\n"))
			for _, want := range tt.wantContains {
				assert.Contains(t, got, want)
			}
			for _, notFound := range tt.wantNotFound {
				assert.NotContains(t, got, notFound)
			}
		})
	}
}

func TestIssue634RefreshRespectsMarkers(t *testing.T) {
	source := strings.Join([]string{
		`trice(iD(11), "active");`,
		`// TRICE_INSERT_OFF`,
		`trice(iD(22), "skip");`,
		`// TRICE_INSERT_ON`,
		`trice(iD(33), "after");`,
		``,
	}, "\n")

	ilu := make(TriceIDLookUp)
	flu := make(triceFmtLookUp)
	lim := make(TriceIDLookUpLI)
	var out bytes.Buffer
	refreshIDs(&out, "issue634_refresh.c", source, ilu, flu, lim)

	assert.Contains(t, ilu, TriceID(11))
	assert.Contains(t, ilu, TriceID(33))
	assert.NotContains(t, ilu, TriceID(22))
	assert.Contains(t, lim, TriceID(11))
	assert.Contains(t, lim, TriceID(33))
	assert.NotContains(t, lim, TriceID(22))
}

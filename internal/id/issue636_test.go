// SPDX-License-Identifier: MIT

package id_test

import (
	"bytes"
	"testing"

	"github.com/rokath/trice/internal/args"
	. "github.com/rokath/trice/internal/id"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestIssue636InsertErrorReturnsWithoutEmptyingFiles(t *testing.T) {
	tests := []struct {
		name  string
		files []struct {
			name         string
			source       string
			allowedAfter []string
		}
	}{
		{
			name: "single_file_error_after_partial_insert",
			files: []struct {
				name         string
				source       string
				allowedAfter []string
			}{
				{
					name:   "bad.c",
					source: "trice(\"first ok\");\nTRice(\"bad %d\", 1, 2);\n",
				},
			},
		},
		{
			name: "multi_file_error_keeps_sources_non_empty",
			files: []struct {
				name         string
				source       string
				allowedAfter []string
			}{
				{
					name:   "good.c",
					source: "trice(\"good\");\n",
					allowedAfter: []string{
						"trice(\"good\");\n",
						"trice(iD(1200), \"good\");\n",
					},
				},
				{
					name:   "bad.c",
					source: "TRice(\"bad %d\", 1, 2);\n",
				},
			},
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			defer Setup(t)()

			require.NoError(t, FSys.WriteFile(FnJSON, []byte("{}"), 0o777))
			require.NoError(t, FSys.WriteFile(LIFnJSON, []byte("{}"), 0o777))

			cliArgs := []string{
				"trice", "insert",
				"-IDMin", "1200",
				"-IDMax", "1299",
				"-IDMethod", "upward",
				"-til", FnJSON,
				"-li", LIFnJSON,
			}

			for _, file := range tt.files {
				require.NoError(t, FSys.WriteFile(file.name, []byte(file.source), 0o777))
				cliArgs = append(cliArgs, "-src", file.name)
			}

			var out bytes.Buffer
			err := args.Handler(&out, FSys, cliArgs)
			require.Error(t, err)
			assert.Contains(t, out.String(), "format specifier count 1 != parameter count 2")

			for _, file := range tt.files {
				got, err := FSys.ReadFile(file.name)
				require.NoError(t, err)
				assert.NotEmpty(t, got)
				if len(file.allowedAfter) == 0 {
					assert.Equal(t, file.source, string(got))
					continue
				}
				assert.Contains(t, file.allowedAfter, string(got))
			}

			til, err := FSys.ReadFile(FnJSON)
			require.NoError(t, err)
			assert.Equal(t, "{}", string(til))

			li, err := FSys.ReadFile(LIFnJSON)
			require.NoError(t, err)
			assert.Equal(t, "{}", string(li))
		})
	}
}

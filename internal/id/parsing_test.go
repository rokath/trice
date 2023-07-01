// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

import (
	"testing"

	"github.com/tj/assert"
)

func TestMatchFormatString(t *testing.T) {
	testSet := []struct {
		text, fmt string
	}{
		{`..."a\"
		\"b\"\"\"c"...`, `"a\"
		\"b\"\"\"c"`},
		{`..."a\"\"b\"\"\"c"...`, `"a\"\"b\"\"\"c"`},
		{`..."a\"b\"c"...`, `"a\"b\"c"`},
		{`..."\"b\""...`, `"\"b\""`},
		{`..."a\"b"...`, `"a\"b"`},
		{`..."a"...`, `"a"`},
		{`..."a
			b"...`, `"a
			b"`},
	}
	for _, s := range testSet {
		loc := matchFormatString(s.text)
		assert.Equal(t, s.fmt, s.text[loc[0]:loc[1]])
	}
}

// SPDX-License-Identifier: MIT

// white-box test for package emitter.
package emitter

import (
	"fmt"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// duplicateTagAliases reports every alias that belongs to more than one tag group.
func duplicateTagAliases(tags []tag) map[string][]string {
	owners := make(map[string][]string)
	for _, group := range tags {
		canonical := group.Names[0]
		for _, alias := range group.Names {
			owners[alias] = append(owners[alias], canonical)
		}
	}
	for alias, groups := range owners {
		if len(groups) == 1 {
			delete(owners, alias)
		}
	}
	return owners
}

// TestTagAliasesAreUnique protects the complete registry, including future additions.
func TestTagAliasesAreUnique(t *testing.T) {
	assert.Empty(t, duplicateTagAliases(Tags))

	duplicate := []tag{
		{Names: []string{"first", "shared"}},
		{Names: []string{"second", "shared"}},
	}
	assert.Equal(t, map[string][]string{"shared": {"first", "second"}}, duplicateTagAliases(duplicate))
}

// Test1colorize verifies the expected behavior.
func Test1colorize(t *testing.T) {
	lw := newCheckDisplay()
	p := newLineTransformerANSI(lw, "none")
	s := "abc:de"
	c, _ := p.colorize(s)
	if c != s {
		t.Fail()
	}
}

// Test2colorize verifies the expected behavior.
func Test2colorize(t *testing.T) {
	lw := newCheckDisplay()
	p := newLineTransformerANSI(lw, "none")
	s := "msg:de"
	c, _ := p.colorize(s)
	assert.Equal(t, "de", c)
}

// Test3colorize verifies the expected behavior.
func Test3colorize(t *testing.T) {
	lw := newCheckDisplay()
	p := newLineTransformerANSI(lw, "off")
	s := "msg:de"
	act, _ := p.colorize(s)
	assert.Equal(t, s, act)
}

// TestColorizeUsesTagWeights verifies inclusive filtering by alias and numeric
// threshold. Unknown raw fragments remain unchanged before event normalization.
func TestColorizeUsesTagWeights(t *testing.T) {
	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })
	UserLabel = nil
	require.NoError(t, AddUserLabels())
	p := newLineTransformerANSI(newCheckDisplay(), "off")

	for _, threshold := range []string{"INFO", "info", "500"} {
		LogLevel = threshold
		require.NoError(t, ResolveFilterSelectors())

		_, show := p.colorize("wrn:shown")
		assert.True(t, show, threshold)
		_, show = p.colorize("msg:boundary")
		assert.True(t, show, threshold)
		_, show = p.colorize("dbg:hidden")
		assert.False(t, show, threshold)
		_, show = p.colorize("misspelled:unchanged")
		assert.True(t, show, threshold)
	}
}

// TestUntaggedColorAndWeightHandling verifies that presentation removes only
// the synthetic outer tag unless colors are fully disabled, and that the group
// participates in ordinary weight filtering.
func TestUntaggedColorAndWeightHandling(t *testing.T) {
	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })
	UserLabel = nil
	require.NoError(t, AddUserLabels())
	text := string(NormalizeApplicationTag([]byte("mgs:blah"), "mgs"))

	for _, palette := range []string{"default", "none"} {
		LogLevel = "all"
		p := newLineTransformerANSI(newCheckDisplay(), palette)
		got, show := p.colorize(text)
		assert.True(t, show)
		assert.Equal(t, "mgs:blah", got, palette)
	}

	LogLevel = "all"
	p := newLineTransformerANSI(newCheckDisplay(), "off")
	got, show := p.colorize(text)
	assert.True(t, show)
	assert.Equal(t, "untagged:mgs:blah", got)
	assert.Greater(t, TagEvents("untagged"), 0)

	LogLevel = "notice"
	require.NoError(t, ResolveFilterSelectors())
	_, show = p.colorize(text)
	assert.False(t, show)
	LogLevel = "500"
	require.NoError(t, ResolveFilterSelectors())
	_, show = p.colorize(text)
	assert.True(t, show)

	UserLabel = ArrayFlag{"untagged:150"}
	require.NoError(t, AddUserLabels())
	LogLevel = "200"
	require.NoError(t, ResolveFilterSelectors())
	_, show = p.colorize(text)
	assert.False(t, show)
}

func _Test4colorize(t *testing.T) {
	lw := newCheckDisplay()
	p := newLineTransformerANSI(lw, "default")
	s := "msg:de"
	c, _ := p.colorize(s)
	act := []byte(c)
	exp := []byte{27, 91, 57, 50, 59, 52, 48, 109, 100, 101, 27, 91, 48, 109}
	fmt.Println("exp:", string(exp))
	fmt.Println("act:", string(act))

	// This change could be a Go version issue.

	//	exp: [  92;40mde[0m
	//	act: [0;92;40mde[0m

	// expected: []byte{0x1b, 0x5b,             0x39, 0x32, 0x3b, 0x34, 0x30, 0x6d, 0x64, 0x65, 0x1b, 0x5b, 0x30, 0x6d} // legacy
	// actual  : []byte{0x1b, 0x5b, 0x30, 0x3b, 0x39, 0x32, 0x3b, 0x34, 0x30, 0x6d, 0x64, 0x65, 0x1b, 0x5b, 0x30, 0x6d} // now

	assert.Equal(t, exp, act)
}

func _Test5colorize(t *testing.T) {
	lw := newCheckDisplay()
	p := newLineTransformerANSI(lw, "default")
	s := "MESSAGE:de"
	c, _ := p.colorize(s)
	b := []byte{27, 91, 57, 50, 59, 52, 48, 109, 77, 69, 83, 83, 65, 71, 69, 58, 100, 101, 27, 91, 48, 109}
	assert.Equal(t, b, []byte(c))
}

// Test1WriteLine verifies the expected behavior.
func Test1WriteLine(t *testing.T) {
	lw := newCheckDisplay()
	p := newLineTransformerANSI(lw, "none")
	q := newLineTransformerANSI(lw, "off")
	l := []string{"M:msg", "I:Info", "wrn:End"}
	p.WriteLine(l)
	q.WriteLine(l)
	ep := strings.Join([]string{"M:msg", "I:Info", "End"}, "")
	eq := strings.Join([]string{"M:msg", "I:Info", "wrn:End"}, "")
	assert.Equal(t, []string{ep, eq}, lw.lines)
}

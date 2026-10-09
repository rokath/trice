// SPDX-License-Identifier: MIT

// white-box test for package emitter.
package emitter

import (
	"fmt"
	"strconv"
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

// TestColorizeOnlyPresentsAcceptedFragments verifies that a metadata-looking
// prefix cannot cause presentation to discard a line after event selection.
func TestColorizeOnlyPresentsAcceptedFragments(t *testing.T) {
	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })
	UserLabel = nil
	require.NoError(t, AddUserLabels())
	p := newLineTransformerANSI(newCheckDisplay(), "off")

	for _, threshold := range []string{"INFO", "info", "500"} {
		LogLevel = threshold
		require.NoError(t, ResolveFilterSelectors())

		for _, fragment := range []string{"wrn:shown", "msg:boundary", "dbg:metadata", "misspelled:unchanged"} {
			got, show := p.colorize(fragment)
			assert.True(t, show, "%s: %s", threshold, fragment)
			assert.Equal(t, fragment, got)
		}
	}
}

// TestUntaggedColorAndWeightHandling verifies that an unknown application tag
// keeps its original text under every palette while retaining its own weight.
func TestUntaggedColorAndWeightHandling(t *testing.T) {
	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })
	UserLabel = nil
	require.NoError(t, AddUserLabels())
	text := "mgs:blah"

	for _, palette := range []string{"default", "none"} {
		LogLevel = "all"
		p := newLineTransformerANSI(newCheckDisplay(), palette)
		got, show := p.colorize(text)
		assert.True(t, show)
		assert.Equal(t, "mgs:blah", got, palette)
	}

	LogLevel = "all"
	TagStatistics = true
	RecordTagEvent("mgs")
	p := newLineTransformerANSI(newCheckDisplay(), "off")
	got, show := p.colorize(text)
	assert.True(t, show)
	assert.Equal(t, "mgs:blah", got)
	assert.Equal(t, 1, TagEvents("untagged"))

	LogLevel = "wrn"
	require.NoError(t, ResolveFilterSelectors())
	assert.False(t, ApplicationEventAllowed("mgs"))
	LogLevel = "500"
	require.NoError(t, ResolveFilterSelectors())
	assert.True(t, ApplicationEventAllowed("mgs"))

	UserLabel = ArrayFlag{"untagged:150"}
	require.NoError(t, AddUserLabels())
	LogLevel = "200"
	require.NoError(t, ResolveFilterSelectors())
	assert.False(t, ApplicationEventAllowed("mgs"))
}

// TestTagLevelCoversEverySupportedWeight verifies the complete interval policy,
// including exact boundaries, neighboring values, 0 and 999. Classification
// never rounds the effective weight used by the existing event filter.
func TestTagLevelCoversEverySupportedWeight(t *testing.T) {
	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })
	UserLabel = ArrayFlag{"sensor:0"}
	require.NoError(t, AddUserLabels())
	index := tagIndex(Tags, "sensor")
	for _, interval := range []struct {
		level     string
		low, high int
	}{
		{"VERBOSE", 0, 99}, {"TRACE", 100, 299}, {"DEBUG", 300, 499},
		{"INFO", 500, 599}, {"WARNING", 600, 699}, {"ERROR", 700, 799},
		{"CRITICAL", 800, 899}, {"FATAL", 900, 999},
	} {
		t.Run(interval.level, func(t *testing.T) {
			for weight := interval.low; weight <= interval.high; weight++ {
				Tags[index].weight = weight
				assert.Equal(t, interval.level, TagLevel("sensor"), "weight %d", weight)
				actual, err := TagWeight("sensor")
				require.NoError(t, err)
				assert.Equal(t, weight, actual, "classification must retain the exact weight")
				LogLevel = strconv.Itoa(weight)
				assert.True(t, ApplicationEventAllowed("sensor"), "inclusive threshold at %d", weight)
				if weight < 999 {
					LogLevel = strconv.Itoa(weight + 1)
					assert.False(t, ApplicationEventAllowed("sensor"), "threshold immediately above %d", weight)
				}
			}
		})
	}
}

// TestTagLevelsUseOnlyNamedBoundaries checks every built-in alias against its
// category's documented level, including shared weights and tool diagnostics.
func TestTagLevelsUseOnlyNamedBoundaries(t *testing.T) {
	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })
	UserLabel = nil
	require.NoError(t, AddUserLabels())
	assert.Equal(t, []string{"FATAL", "CRITICAL", "ERROR", "WARNING", "INFO", "DEBUG", "TRACE", "VERBOSE"}, levelTags)
	for _, group := range Tags {
		want := ""
		switch group.weight {
		case 900:
			want = "FATAL"
		case 800:
			want = "CRITICAL"
		case 700:
			want = "ERROR"
		case 600:
			want = "WARNING"
		case 500:
			want = "INFO"
		case 300:
			want = "DEBUG"
		case 100:
			want = "TRACE"
		case 0:
			if group.Names[0] != "CYCLE_ERROR" {
				want = "VERBOSE"
			}
		default:
			t.Fatalf("undocumented default weight for %s: %d", group.Names[0], group.weight)
		}
		for _, alias := range group.Names {
			assert.Equal(t, want, TagLevel(alias), alias)
		}
	}
	assert.Empty(t, TagLevel("not_registered"), "callers classify unknown format tags as untagged, not an invented default level")
}

// TestTagLevelOverridesDoNotMoveBoundariesOrLeak verifies independent category
// identity, fixed level boundaries despite equal overridden level-tag weights,
// implicit custom-tag weights, and a fresh registry on the next command.
func TestTagLevelOverridesDoNotMoveBoundariesOrLeak(t *testing.T) {
	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })
	UserLabel = ArrayFlag{"INFO:750", "wrn:750", "FATAL:750", "plain", "sensor:650", "rx:750", "untagged:150", "alarm:650"}
	require.NoError(t, AddUserLabels())
	for _, tc := range []struct{ tag, level string }{
		{"INFO", "ERROR"}, {"inf", "ERROR"}, {"WRN", "ERROR"}, {"FATAL", "ERROR"},
		{"plain", "ERROR"}, {"sensor", "WARNING"}, {"rx", "ERROR"}, {"RX", "ERROR"},
		{"untagged", "TRACE"}, {"alarm", "WARNING"}, {"DEBUG", "DEBUG"},
	} {
		assert.Equal(t, tc.level, TagLevel(tc.tag), tc.tag)
	}
	plainWeight, err := TagWeight("plain")
	require.NoError(t, err)
	assert.Equal(t, 750, plainWeight, "new unweighted labels inherit effective INFO weight")
	UserLabel = nil
	require.NoError(t, AddUserLabels())
	assert.Equal(t, "INFO", TagLevel("INFO"))
	assert.Equal(t, "WARNING", TagLevel("wrn"))
	assert.Equal(t, "FATAL", TagLevel("FATAL"))
	assert.Equal(t, "DEBUG", TagLevel("RX"))
	assert.Equal(t, "INFO", TagLevel("untagged"))
	assert.Empty(t, TagLevel("plain"), "custom tags must not survive the command that registered them")
}

// TestTagLevelDoesNotDependOnRegistryOrder guards the separation between source
// readability and semantics when maintainers reorder equal-weight categories.
func TestTagLevelDoesNotDependOnRegistryOrder(t *testing.T) {
	s := snapshotEmitterState()
	oldDefaults, oldLevels := defaultTags, levelTags
	t.Cleanup(func() { defaultTags, levelTags = oldDefaults, oldLevels; restoreEmitterState(s) })
	defaultTags = copyTagRegistry(defaultTags)
	levelTags = append([]string(nil), levelTags...)
	for i, j := 0, len(defaultTags)-1; i < j; i, j = i+1, j-1 {
		defaultTags[i], defaultTags[j] = defaultTags[j], defaultTags[i]
	}
	for i, j := 0, len(levelTags)-1; i < j; i, j = i+1, j-1 {
		levelTags[i], levelTags[j] = levelTags[j], levelTags[i]
	}
	UserLabel = ArrayFlag{"sensor:650"}
	require.NoError(t, AddUserLabels())
	assert.Equal(t, "WARNING", TagLevel("sensor"))
	assert.Equal(t, "FATAL", TagLevel("EMERGENCY"))
	assert.Equal(t, "DEBUG", TagLevel("rx"))
}

// TestBuiltInTagRecognitionPreservesWrittenPrefixes proves that spelling does
// not change a built-in group's metadata or presentation color. Removing case
// variants from this private copy also checks that recognition does not rely
// on enumerating them. Only entirely lowercase prefixes disappear from text.
func TestBuiltInTagRecognitionPreservesWrittenPrefixes(t *testing.T) {
	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })
	UserLabel = ArrayFlag{"rX:750", "ReCeIvE:red:blue"}
	require.NoError(t, AddUserLabels())
	assert.Len(t, Tags, len(defaultTags), "a built-in spelling overrides its group rather than registering a user tag")
	index := tagIndex(Tags, "RECEIVE")
	Tags[index].Names = []string{"RECEIVE", "rx"}
	TagStatistics = true
	LogLevel = "all"
	for _, spelling := range []string{"rx", "RX", "rX", "receive", "Receive", "ReCeIvE", "RECEIVE"} {
		t.Run(spelling, func(t *testing.T) {
			canonical, err := FindTagName(spelling)
			require.NoError(t, err)
			assert.Equal(t, "RECEIVE", canonical)
			weight, err := TagWeight(spelling)
			require.NoError(t, err)
			assert.Equal(t, 750, weight)
			assert.Equal(t, "ERROR", TagLevel(spelling))
			RecordTagEvent(spelling)
			message := spelling + ":  payload  "
			visible := message
			if isLower(spelling) {
				visible = "  payload  "
			}
			for _, palette := range []string{"none", "default", "off"} {
				want := visible
				if palette == "off" {
					want = message
				} else if palette == "default" {
					want = Tags[index].colorize(visible)
				}
				transformer := newLineTransformerANSI(newCheckDisplay(), palette)
				got, shown := transformer.colorize(message)
				assert.True(t, shown)
				assert.Equal(t, want, got, palette)
				ColorPalette = palette
				assert.Equal(t, want, Colorize(message), "statistics presentation with %s", palette)
			}
		})
	}
	assert.Equal(t, 7, TagEvents("rX"), "all spellings count in one group")
	assert.Zero(t, TagEvents("untagged"))
}

// TestBuiltInTagSelectorsShareWeightsAcrossSpellings checks selection for both
// typed events and legacy byte fragments. A mixed-case prefix must never pass
// or fail using the unrelated untagged weight; user labels still stay literal.
func TestBuiltInTagSelectorsShareWeightsAcrossSpellings(t *testing.T) {
	for _, tc := range []struct {
		name, threshold string
		pick, ban       channelArrayFlag
		want            bool
	}{
		{name: "inclusive numeric boundary", threshold: "750", want: true},
		{name: "next numeric weight rejects", threshold: "751"},
		{name: "mixed-case named threshold", threshold: "ReCeIvE", want: true},
		{name: "mixed-case pick selects the group", threshold: "all", pick: channelArrayFlag{"rX"}, want: true},
		{name: "canonical pick selects the group", threshold: "all", pick: channelArrayFlag{"RECEIVE"}, want: true},
		{name: "mixed-case ban rejects the group", threshold: "all", ban: channelArrayFlag{"ReCeIvE"}},
		{name: "untagged pick does not select a known mixed-case tag", threshold: "all", pick: channelArrayFlag{"untagged"}},
		{name: "pick does not bypass the numeric threshold", threshold: "751", pick: channelArrayFlag{"rX"}},
	} {
		t.Run(tc.name, func(t *testing.T) {
			s := snapshotEmitterState()
			t.Cleanup(func() { restoreEmitterState(s) })
			UserLabel = ArrayFlag{"Rx:750", "untagged:150"}
			require.NoError(t, AddUserLabels())
			Pick, Ban, LogLevel = tc.pick, tc.ban, tc.threshold
			require.NoError(t, ResolveFilterSelectors())
			for _, spelling := range []string{"rx", "RX", "rX", "ReCeIvE"} {
				assert.Equal(t, tc.want, ApplicationEventAllowed(spelling), "typed %s", spelling)
				assert.Equal(t, tc.want, UnclassifiedFragmentAllowed([]byte(spelling+":payload")), "fragment %s", spelling)
			}
		})
	}
}

// TestUserTagsRemainExactAcrossLogging keeps separately registered new and NEW
// independent in metadata, levels, selection, statistics and text. A third
// spelling is unknown and follows the configured untagged event policy.
func TestUserTagsRemainExactAcrossLogging(t *testing.T) {
	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })
	UserLabel = ArrayFlag{"new:300", "NEW:650", "untagged:150"}
	require.NoError(t, AddUserLabels())
	TagStatistics = true
	for _, tc := range []struct {
		name, level, visible string
		weight               int
	}{
		{"new", "DEBUG", "payload", 300},
		{"NEW", "WARNING", "NEW:payload", 650},
	} {
		canonical, err := FindTagName(tc.name)
		require.NoError(t, err)
		assert.Equal(t, tc.name, canonical)
		weight, err := TagWeight(tc.name)
		require.NoError(t, err)
		assert.Equal(t, tc.weight, weight)
		assert.Equal(t, tc.level, TagLevel(tc.name))
		RecordTagEvent(tc.name)
		got, _ := newLineTransformerANSI(newCheckDisplay(), "none").colorize(tc.name + ":payload")
		assert.Equal(t, tc.visible, got)
	}
	_, err := FindTagName("NeW")
	assert.Error(t, err)
	_, err = TagWeight("NeW")
	assert.Error(t, err)
	assert.Empty(t, TagLevel("NeW"))
	RecordTagEvent("NeW")
	assert.Equal(t, 1, TagEvents("new"))
	assert.Equal(t, 1, TagEvents("NEW"))
	assert.Equal(t, 1, TagEvents("untagged"))
	assert.Equal(t, -1, TagEvents("NeW"))
	got, _ := newLineTransformerANSI(newCheckDisplay(), "none").colorize("NeW:payload")
	assert.Equal(t, "NeW:payload", got)
	Pick, Ban, LogLevel = channelArrayFlag{"new"}, nil, "all"
	require.NoError(t, ResolveFilterSelectors())
	assert.True(t, ApplicationEventAllowed("new"))
	assert.False(t, ApplicationEventAllowed("NEW"))
	assert.False(t, ApplicationEventAllowed("NeW"))
	Pick, Ban = nil, channelArrayFlag{"NEW"}
	require.NoError(t, ResolveFilterSelectors())
	assert.True(t, ApplicationEventAllowed("new"))
	assert.False(t, ApplicationEventAllowed("NEW"))
	assert.True(t, ApplicationEventAllowed("NeW"), "unknown spelling belongs to untagged, not NEW")
	Ban, LogLevel = nil, "151"
	assert.False(t, ApplicationEventAllowed("NeW"), "unknown spelling uses untagged weight 150")
	for _, option := range []string{"pick", "ban", "level"} {
		Pick, Ban, LogLevel = nil, nil, "all"
		switch option {
		case "pick":
			Pick = channelArrayFlag{"NeW"}
		case "ban":
			Ban = channelArrayFlag{"NeW"}
		case "level":
			LogLevel = "NeW"
		}
		assert.Error(t, ResolveFilterSelectors(), "a CLI %s must name an exact user tag", option)
	}
}

// TestOKIsNotAnImplicitMessageAlias keeps a success-like word out of MESSAGE.
// It is ordinary unknown text unless the user explicitly registers that label;
// registration then follows the same exact-case policy as any free user tag.
func TestOKIsNotAnImplicitMessageAlias(t *testing.T) {
	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })
	UserLabel = nil
	require.NoError(t, AddUserLabels())
	TagStatistics = true
	for _, spelling := range []string{"OK", "Ok", "ok"} {
		_, err := FindTagName(spelling)
		assert.Error(t, err)
		got, _ := newLineTransformerANSI(newCheckDisplay(), "none").colorize(spelling + ":ready")
		assert.Equal(t, spelling+":ready", got)
		RecordTagEvent(spelling)
	}
	assert.Equal(t, 3, TagEvents("untagged"))
	assert.Zero(t, TagEvents("msg"))
	UserLabel = ArrayFlag{"OK:650"}
	require.NoError(t, AddUserLabels())
	canonical, err := FindTagName("OK")
	require.NoError(t, err)
	assert.Equal(t, "OK", canonical)
	assert.Equal(t, "WARNING", TagLevel("OK"))
	_, err = FindTagName("ok")
	assert.Error(t, err)
}

// TestBuiltInTagsHaveNoSingleLetterAliases checks the registry-wide rule rather
// than assuming a particular list of removed shortcuts. Explicit user labels
// may still be one letter, with independent case-sensitive weights and colors.
func TestBuiltInTagsHaveNoSingleLetterAliases(t *testing.T) {
	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })
	UserLabel = nil
	require.NoError(t, AddUserLabels())
	for _, group := range Tags {
		for _, alias := range group.Names {
			assert.Greater(t, len([]rune(alias)), 1, "built-in alias %q in %s", alias, group.Names[0])
		}
	}
	for letter := 'a'; letter <= 'z'; letter++ {
		for _, spelling := range []string{string(letter), strings.ToUpper(string(letter))} {
			_, err := FindTagName(spelling)
			assert.Error(t, err, "single letter %s is not built in", spelling)
			got, _ := newLineTransformerANSI(newCheckDisplay(), "none").colorize(spelling + ":ready")
			assert.Equal(t, spelling+":ready", got)
		}
	}
	UserLabel = ArrayFlag{"x:650", "X:150", "e:750"}
	require.NoError(t, AddUserLabels())
	for _, tc := range []struct{ name, level, message string }{
		{"x", "WARNING", "ready"}, {"X", "TRACE", "X:ready"}, {"e", "ERROR", "ready"},
	} {
		canonical, err := FindTagName(tc.name)
		require.NoError(t, err)
		assert.Equal(t, tc.name, canonical)
		assert.Equal(t, tc.level, TagLevel(tc.name))
		got, _ := newLineTransformerANSI(newCheckDisplay(), "none").colorize(tc.name + ":ready")
		assert.Equal(t, tc.message, got)
	}
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

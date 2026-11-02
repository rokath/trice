// SPDX-License-Identifier: MIT

package id

import (
	"fmt"
	"io"
	"regexp"
	"sort"
	"strings"
	"sync/atomic"

	"github.com/rokath/trice/internal/emitter"
)

// aliasDefinition is command-local policy; none of it participates in TIL keys.
type aliasDefinition struct {
	stringPayload bool
	tag           string
	used          atomic.Bool
}

// aliasDefinitions is replaced before processing starts and read concurrently
// by source workers. Only each definition's usage counter may change later.
var aliasDefinitions = map[string]*aliasDefinition{}

// aliasIdentifier deliberately accepts only the portable C identifier subset.
var aliasIdentifier = regexp.MustCompile(`^[A-Za-z_][A-Za-z0-9_]*$`)

// prepareAliasDefinitions validates even unused options before publishing them.
func prepareAliasDefinitions() error {
	next := make(map[string]*aliasDefinition)
	for _, family := range []struct {
		values        []string
		stringPayload bool
		option        string
	}{{TriceAliases, false, "-alias"}, {TriceSAliases, true, "-salias"}} {
		for _, value := range family.values {
			name, assignment, explicit := strings.Cut(value, ":")
			if !aliasIdentifier.MatchString(name) || strings.HasPrefix(strings.ToLower(name), "trice") {
				return fmt.Errorf("invalid %s %q: expected a C identifier not starting with trice", family.option, value)
			}
			canonical := ""
			if explicit {
				var err error
				canonical, err = emitter.CanonicalTagDefinition(assignment)
				if err != nil {
					return fmt.Errorf("invalid %s %q: %w", family.option, value, err)
				}
			}
			previous, ok := next[name]
			if !ok {
				previous = &aliasDefinition{stringPayload: family.stringPayload}
				next[name] = previous
			}
			if previous.stringPayload != family.stringPayload {
				return fmt.Errorf("alias %q is registered under both -alias and -salias", name)
			}
			if canonical != "" && previous.tag != "" && canonical != previous.tag {
				return fmt.Errorf("alias %q has conflicting tags %s and %s", name, previous.tag, canonical)
			}
			if canonical != "" {
				previous.tag = canonical
			}
		}
	}
	aliasDefinitions = next
	return nil
}

// AliasTagNames supplies implicit registrations without modifying CLI values.
func AliasTagNames() []string {
	var names []string
	for _, definition := range aliasDefinitions {
		if definition.tag != "" {
			names = append(names, definition.tag)
		}
	}
	sort.Strings(names)
	return names
}

// storedTriceFormat removes source-parser state while preserving exact identity.
func storedTriceFormat(t TriceFmt) TriceFmt {
	if t.Alias != "" {
		t.Type, t.Alias = t.Alias, ""
	}
	return t
}

// PayloadFormat derives a disposable native view. Raw Type/Strg remain the
// equality key; complete historical SAlias wrappers also work with native types.
func PayloadFormat(t TriceFmt) (TriceFmt, error) {
	if t.Alias != "" || strings.HasPrefix(strings.ToLower(t.Type), "trice") {
		return t, nil
	}
	if !aliasIdentifier.MatchString(t.Type) {
		return t, fmt.Errorf("invalid alias Type %q", t.Type)
	}
	start, end := strings.HasPrefix(t.Strg, SAliasStrgPrefix), strings.HasSuffix(t.Strg, SAliasStrgSuffix)
	if start != end {
		return t, fmt.Errorf("alias %q has a partial SAlias wrapper", t.Type)
	}
	t.Alias, t.Type = t.Type, "trice"
	if start {
		t.Type = "triceS"
	}
	return t, nil
}

// processingFormat exposes the received string payload as %s, rather than
// treating a stored C argument list as a printf template in generated readers.
func processingFormat(t TriceFmt) TriceFmt {
	t, _ = PayloadFormat(t)
	if isSAliasEncodedString(t.Strg) {
		t.Strg = "%s"
	}
	return t
}

// ValidateAliases checks a complete replacement before any live map is changed.
func (ilu TriceIDLookUp) ValidateAliases() error {
	ids := make([]int, 0, len(ilu))
	for n := range ilu {
		ids = append(ids, int(n))
	}
	sort.Ints(ids)
	for _, n := range ids {
		if _, err := PayloadFormat(ilu[TriceID(n)]); err != nil {
			return fmt.Errorf("TIL ID %d: %w", n, err)
		}
	}
	return nil
}

// AliasEvent describes classification and the only prefix eligible for removal.
// Prefix is static provenance, never a received runtime string.
type AliasEvent struct {
	Alias  bool
	Tag    string
	Prefix string
	Reason string
}

// ResolveAliasEvent applies override, frozen name heuristic, first static
// prefix, then untagged. It does not interpret payload width from alias names.
func ResolveAliasEvent(raw TriceFmt) AliasEvent {
	t, err := PayloadFormat(raw)
	if err != nil || t.Alias == "" {
		return AliasEvent{}
	}
	event := AliasEvent{Alias: true, Tag: "untagged"}
	format := aliasStaticFormat(raw, t)
	if prefix, _, ok := strings.Cut(format, ":"); ok {
		if _, err := emitter.FindTagName(prefix); err == nil {
			event.Prefix = prefix
		}
	}
	if definition := aliasDefinitions[t.Alias]; definition != nil {
		definition.used.Store(true)
		if definition.tag != "" {
			event.Tag, event.Reason = definition.tag, "explicit override"
			return event
		}
	}
	tag, reason := aliasNameTag(t.Alias)
	if tag != "" {
		event.Tag, event.Reason = tag, reason
		return event
	}
	if event.Prefix != "" {
		event.Tag, _ = emitter.FindTagName(event.Prefix)
		event.Reason = "first static prefix"
	} else {
		event.Reason = "no name match or registered first static prefix"
	}
	event.Reason += "; " + reason
	return event
}

// aliasStaticFormat shares the bounded first-argument interpretation between
// host classification and CE selection without modifying the stored wrapper.
func aliasStaticFormat(raw, payload TriceFmt) string {
	format := raw.Strg
	if payload.Type == "triceS" {
		if !strings.HasPrefix(format, SAliasStrgPrefix) || !strings.HasSuffix(format, SAliasStrgSuffix) {
			format = ""
		} else {
			format = strings.TrimSpace(strings.TrimSuffix(strings.TrimPrefix(format, SAliasStrgPrefix), SAliasStrgSuffix))
			// A later literal in an assertion expression is not a format prefix.
			if !strings.HasPrefix(format, `"`) {
				format = ""
			} else if loc := matchStringLiteral(format); loc != nil && loc[0] == 0 {
				format = format[1 : loc[1]-1]
			} else {
				format = ""
			}
		}
	}
	return format
}

// ExplainAliases reports policy once per dictionary entry and unused options.
func ExplainAliases(w io.Writer, ilu TriceIDLookUp) {
	if !Verbose {
		return
	}
	ids := make([]int, 0, len(ilu))
	for n := range ilu {
		ids = append(ids, int(n))
	}
	sort.Ints(ids)
	for _, n := range ids {
		raw := ilu[TriceID(n)]
		event := ResolveAliasEvent(raw)
		if !event.Alias {
			continue
		}
		payload, _ := PayloadFormat(raw)
		fmt.Fprintf(w, "Alias ID %d %s: kind=%s tag=%s (%s)\n", n, raw.Type, payload.Type, event.Tag, event.Reason)
		if event.Prefix != "" {
			if tag, _ := emitter.FindTagName(event.Prefix); tag != event.Tag {
				fmt.Fprintf(w, "Hint: alias %s keeps conflicting static prefix %q as message text.\n", raw.Type, event.Prefix)
			}
		}
	}
	names := make([]string, 0, len(aliasDefinitions))
	for name := range aliasDefinitions {
		names = append(names, name)
	}
	sort.Strings(names)
	for _, name := range names {
		if definition := aliasDefinitions[name]; !definition.used.Load() {
			kind := "trice"
			if definition.stringPayload {
				kind = "triceS"
			}
			tag, reason := aliasNameTag(name)
			if definition.tag != "" {
				tag, reason = definition.tag, "explicit override"
			}
			if tag == "" {
				tag = "untagged"
			}
			fmt.Fprintf(w, "Alias %s: valid unused registration (kind=%s tag=%s; %s)\n", name, kind, tag, reason)
		}
	}
}

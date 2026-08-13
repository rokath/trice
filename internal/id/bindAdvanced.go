// SPDX-License-Identifier: MIT

package id

import (
	"fmt"
	"regexp"
	"sort"
	"strings"
)

const (
	bindRebaseBeginMarker   = "/* trice-bind: generated rebase begin "
	bindRebaseEndMarker     = "/* trice-bind: generated rebase end "
	bindRebaseBlockEnd      = "/* trice-bind: generated rebase block end */"
	bindRebaseIncludeMarker = "// trice-bind: generated rebase "
)

var (
	// bindRebaseGeneratedInclude recognizes the owner-sidecar line inside the
	// legacy verbose source representation retained for migration compatibility.
	// Horizontal directive whitespace may be normalized by C formatters and is
	// therefore not part of the generated ownership identity.
	bindRebaseGeneratedInclude = regexp.MustCompile(`^[\t ]*#[\t ]*include[\t ]+"trice_[A-Za-z0-9_]+_(K[0-9A-F]{16})\.h"[\t ]+// trice-bind: generated rebase (begin|end)[\t ]*$`)
	// bindRebaseCompactInclude recognizes the complete current source boundary
	// and captures identities that must agree before it may be removed. As with
	// the legacy form, formatter-controlled horizontal whitespace is ignored.
	bindRebaseCompactInclude = regexp.MustCompile(`^[\t ]*#[\t ]*include[\t ]+"(trice_[A-Za-z0-9_]+_(K[0-9A-F]{16})_(R[0-9]+)_(begin|end)\.h)"[\t ]+// trice-bind: generated rebase (begin|end) (K[0-9A-F]{16}_R[0-9]+)[\t ]*$`)
	// bindRebaseHelperInclude catches edited helper includes that no longer
	// match the exact removable form above.
	bindRebaseHelperInclude = regexp.MustCompile(`^[\t ]*#[\t ]*include[\t ]+"trice_[A-Za-z0-9_]+_K[0-9A-F]{16}_R[0-9]+_(begin|end)\.h"`)
	bindRebaseScope         = regexp.MustCompile(`^K[0-9A-F]{16}_R[0-9]+$`)
)

// bindGeneratedBlock identifies one complete generator-owned source artifact,
// either a legacy verbose block or one current compact include line.
type bindGeneratedBlock struct {
	start      int
	end        int
	kind       string
	scope      string
	line       int
	helperName string
}

// bindOccurrence is one direct Trice call or one supported wrapper invocation.
type bindOccurrence struct {
	start           int
	end             int
	line            int
	column          int
	refs            []bindSiteReference
	comment         string
	wrapper         bool
	macroTerminated bool
}

// stripBindRebaseArtifacts removes only complete, structurally valid blocks
// emitted by trice bind. Malformed markers remain byte-for-byte untouched and
// become fatal diagnostics so user text can never be removed heuristically.
func stripBindRebaseArtifacts(path string, source []byte) ([]byte, []bindGeneratedBlock, []bindDiagnostic, bool) {
	blocks, diagnostics := scanBindRebaseArtifacts(path, source)
	if len(diagnostics) > 0 {
		return source, nil, diagnostics, false
	}
	if len(blocks) == 0 {
		return source, nil, nil, false
	}

	result := append([]byte(nil), source...)
	removals := append([]bindGeneratedBlock(nil), blocks...)
	sort.Slice(removals, func(i, j int) bool { return removals[i].start > removals[j].start })
	for _, block := range removals {
		result = append(result[:block.start], result[block.end:]...)
	}
	return result, blocks, nil, true
}

// scanBindRebaseArtifacts validates generator markers and returns their exact
// source spans. Remigration reuses the spans for precise line relocation.
func scanBindRebaseArtifacts(path string, source []byte) ([]bindGeneratedBlock, []bindDiagnostic) {
	text := string(source)
	var blocks []bindGeneratedBlock
	var diagnostics []bindDiagnostic
	for position := 0; position < len(text); {
		end := lineEndIncludingNewline(text, position)
		rawLine := strings.TrimRight(text[position:end], "\r\n")
		if helperName, _, scope, kind, ok := parseBindCompactRebaseInclude(rawLine); ok {
			blocks = append(blocks, bindGeneratedBlock{
				start:      position,
				end:        end,
				kind:       kind,
				scope:      scope,
				line:       sourceLine(text, position),
				helperName: helperName,
			})
			position = end
			continue
		}
		if strings.Contains(rawLine, bindRebaseIncludeMarker) || bindRebaseHelperInclude.MatchString(rawLine) {
			diagnostics = append(diagnostics, bindDiagnostic{
				path:    path,
				line:    sourceLine(text, position),
				message: "generated Trice bind rebase include does not match the generated directive structure",
			})
			position = end
			continue
		}

		line := strings.TrimSpace(rawLine)
		kind, scope, ok := parseBindRebaseMarker(line)
		if !ok {
			if line == bindRebaseBlockEnd {
				diagnostics = append(diagnostics, bindDiagnostic{
					path:    path,
					line:    sourceLine(text, position),
					message: "orphaned generated Trice bind rebase block terminator",
				})
			}
			position = end
			continue
		}

		blockEnd := end
		foundEnd := false
		for blockEnd < len(text) {
			next := lineEndIncludingNewline(text, blockEnd)
			candidate := strings.TrimSpace(strings.TrimRight(text[blockEnd:next], "\r\n"))
			if candidate == bindRebaseBlockEnd {
				block := bindGeneratedBlock{
					start: position,
					end:   next,
					kind:  kind,
					scope: scope,
					line:  sourceLine(text, position),
				}
				blocks = append(blocks, block)
				if message := validateBindRebaseBlock(text[block.start:block.end], block); message != "" {
					diagnostics = append(diagnostics, bindDiagnostic{path: path, line: block.line, message: message})
				}
				blockEnd = next
				foundEnd = true
				break
			}
			blockEnd = next
		}
		if !foundEnd {
			diagnostics = append(diagnostics, bindDiagnostic{
				path:    path,
				line:    sourceLine(text, position),
				message: fmt.Sprintf("generated Trice bind rebase %s block %s has no terminator", kind, scope),
			})
			break
		}
		position = blockEnd
	}

	type scopeState struct {
		begin *bindGeneratedBlock
		end   *bindGeneratedBlock
	}
	states := make(map[string]scopeState)
	for index := range blocks {
		block := &blocks[index]
		state := states[block.scope]
		switch block.kind {
		case "begin":
			if state.begin != nil {
				diagnostics = append(diagnostics, bindDiagnostic{path: path, line: block.line, message: "duplicate generated Trice bind rebase begin for " + block.scope})
			}
			state.begin = block
		case "end":
			if state.end != nil {
				diagnostics = append(diagnostics, bindDiagnostic{path: path, line: block.line, message: "duplicate generated Trice bind rebase end for " + block.scope})
			}
			state.end = block
		}
		states[block.scope] = state
	}
	for scope, state := range states {
		switch {
		case state.begin == nil:
			diagnostics = append(diagnostics, bindDiagnostic{path: path, line: state.end.line, message: "generated Trice bind rebase end has no begin for " + scope})
		case state.end == nil:
			diagnostics = append(diagnostics, bindDiagnostic{path: path, line: state.begin.line, message: "generated Trice bind rebase begin has no end for " + scope})
		case state.begin.start >= state.end.start:
			diagnostics = append(diagnostics, bindDiagnostic{path: path, line: state.end.line, message: "generated Trice bind rebase end precedes its begin for " + scope})
		}
	}
	activeScope := ""
	for _, block := range blocks {
		switch block.kind {
		case "begin":
			if activeScope != "" {
				diagnostics = append(diagnostics, bindDiagnostic{path: path, line: block.line, message: "nested generated Trice bind rebase begin for " + block.scope})
			}
			activeScope = block.scope
		case "end":
			if activeScope != block.scope {
				diagnostics = append(diagnostics, bindDiagnostic{path: path, line: block.line, message: "generated Trice bind rebase end is out of order for " + block.scope})
			}
			activeScope = ""
		}
	}
	if len(diagnostics) > 0 {
		return nil, diagnostics
	}
	return blocks, nil
}

// parseBindCompactRebaseInclude accepts only a generator-owned helper include
// whose filename, phase comment, file key, and scope encode the same identity.
func parseBindCompactRebaseInclude(line string) (name, key, scope, kind string, ok bool) {
	match := bindRebaseCompactInclude.FindStringSubmatch(line)
	if len(match) != 7 {
		return "", "", "", "", false
	}
	name, key = match[1], match[2]
	ordinal, nameKind, commentKind, commentScope := match[3], match[4], match[5], match[6]
	if nameKind != commentKind || commentScope != key+"_"+ordinal {
		return "", "", "", "", false
	}
	return name, key, commentScope, nameKind, true
}

// validateBindRebaseBlock accepts only the exact generator-owned directive
// sequence. Marker-like user comments can therefore never authorize deletion.
func validateBindRebaseBlock(source string, block bindGeneratedBlock) string {
	normalized := strings.ReplaceAll(source, "\r\n", "\n")
	lines := strings.Split(strings.TrimSuffix(normalized, "\n"), "\n")
	var expected []string
	if block.kind == "begin" {
		expected = []string{
			fmt.Sprintf("%s%s */", bindRebaseBeginMarker, block.scope),
			"#define TRICE_BIND_REBASE_SCOPE " + block.scope,
			"#define TRICE_BIND_REBASE_BEGIN",
			"",
			"#undef TRICE_BIND_REBASE_BEGIN",
			bindRebaseBlockEnd,
		}
	} else {
		expected = []string{
			fmt.Sprintf("%s%s */", bindRebaseEndMarker, block.scope),
			"#define TRICE_BIND_REBASE_END",
			"",
			"#undef TRICE_BIND_REBASE_END",
			bindRebaseBlockEnd,
		}
	}
	if len(lines) != len(expected) {
		return fmt.Sprintf("generated Trice bind rebase %s block %s has an unexpected directive count", block.kind, block.scope)
	}
	for index := range expected {
		if expected[index] == "" {
			match := bindRebaseGeneratedInclude.FindStringSubmatch(lines[index])
			if len(match) != 3 || match[1] != strings.SplitN(block.scope, "_R", 2)[0] || match[2] != block.kind {
				return fmt.Sprintf("generated Trice bind rebase %s block %s has an invalid sidecar include", block.kind, block.scope)
			}
			continue
		}
		if lines[index] != expected[index] {
			return fmt.Sprintf("generated Trice bind rebase %s block %s does not match the generated directive structure", block.kind, block.scope)
		}
	}
	return ""
}

// parseBindRebaseMarker recognizes only the exact comments emitted below.
func parseBindRebaseMarker(line string) (kind, scope string, ok bool) {
	for _, candidate := range []struct {
		prefix string
		kind   string
	}{
		{prefix: bindRebaseBeginMarker, kind: "begin"},
		{prefix: bindRebaseEndMarker, kind: "end"},
	} {
		if strings.HasPrefix(line, candidate.prefix) && strings.HasSuffix(line, " */") {
			scope = strings.TrimSuffix(strings.TrimPrefix(line, candidate.prefix), " */")
			if bindRebaseScope.MatchString(scope) {
				return candidate.kind, scope, true
			}
		}
	}
	return "", "", false
}

// scanBindMacroDefinitions reads complete continued #define directives while
// leaving all Trice-call recognition to scanBindSites and matchTrice.
func scanBindMacroDefinitions(source string) []bindMacroDefinition {
	var definitions []bindMacroDefinition
	for start := 0; start < len(source); {
		end := lineEndIncludingNewline(source, start)
		logicalEnd := end
		for bindPhysicalLineContinues(source[start:logicalEnd]) && logicalEnd < len(source) {
			logicalEnd = lineEndIncludingNewline(source, logicalEnd)
		}
		logical := source[start:logicalEnd]
		masked := stripCComments(logical)
		position := skipBindSpace(masked, 0)
		if position >= len(masked) || masked[position] != '#' {
			start = logicalEnd
			continue
		}
		position = skipBindSpace(masked, position+1)
		if !bindIdentifierAt(masked, position, "define") {
			start = logicalEnd
			continue
		}
		position = skipBindSpace(masked, position+len("define"))
		nameStart := position
		for position < len(masked) && isCIdentifierByte(masked[position]) {
			position++
		}
		if position == nameStart || !bindIdentifierStart(masked[nameStart]) {
			start = logicalEnd
			continue
		}
		definition := bindMacroDefinition{
			name:  strings.TrimSpace(masked[nameStart:position]),
			start: start,
			end:   logicalEnd,
			line:  sourceLine(source, start),
		}
		definition.replacementStart = start + position
		if position < len(masked) && masked[position] == '(' {
			definition.functionLike = true
			closing := findBindClosingParen(logical, position+1)
			if closing >= 0 {
				definition.replacementStart = start + closing + 1
			}
		}
		replacement := source[definition.replacementStart:definition.end]
		code := maskBindCommentsAndLiterals(replacement)
		definition.hasTokenPaste = strings.Contains(code, "##")
		definition.hasTerminatingSemicolon = strings.HasSuffix(strings.TrimSpace(code), ";")
		definition.hasCounter = bindContainsIdentifier(code, "__COUNTER__")
		definitions = append(definitions, definition)
		start = logicalEnd
	}
	return definitions
}

// bindPhysicalLineContinues reports a valid backslash continuation at the
// last non-whitespace byte of one physical line.
func bindPhysicalLineContinues(line string) bool {
	line = strings.TrimRight(line, "\r\n")
	line = strings.TrimRight(line, " \t")
	return strings.HasSuffix(line, "\\")
}

// skipBindSpace skips ordinary whitespace and source line continuations.
func skipBindSpace(source string, position int) int {
	for position < len(source) {
		switch source[position] {
		case ' ', '\t', '\r', '\n', '\f', '\v':
			position++
		case '\\':
			if position+1 < len(source) && source[position+1] == '\n' {
				position += 2
				continue
			}
			if position+2 < len(source) && source[position+1] == '\r' && source[position+2] == '\n' {
				position += 3
				continue
			}
			return position
		default:
			return position
		}
	}
	return position
}

// bindIdentifierAt verifies an exact C identifier without accepting a prefix.
func bindIdentifierAt(source string, position int, identifier string) bool {
	if !strings.HasPrefix(source[position:], identifier) {
		return false
	}
	end := position + len(identifier)
	return end == len(source) || !isCIdentifierByte(source[end])
}

// bindIdentifierStart reports whether a byte may begin a C identifier.
func bindIdentifierStart(value byte) bool {
	return value == '_' || 'A' <= value && value <= 'Z' || 'a' <= value && value <= 'z'
}

// bindContainsIdentifier distinguishes a preprocessor identifier from a
// longer application name that merely contains the same byte sequence.
func bindContainsIdentifier(source, identifier string) bool {
	for position := 0; position < len(source); {
		if !bindIdentifierStart(source[position]) {
			position++
			continue
		}
		start := position
		for position < len(source) && isCIdentifierByte(source[position]) {
			position++
		}
		if source[start:position] == identifier {
			return true
		}
	}
	return false
}

// maskBindCommentsAndLiterals blanks non-code bytes while preserving offsets
// and line endings for wrapper invocation and unsafe-context analysis.
func maskBindCommentsAndLiterals(source string) string {
	out := []byte(source)
	var inBlock, inLine, inString, inChar, escaped bool
	for index := 0; index < len(source); index++ {
		value := source[index]
		switch {
		case inLine:
			if value == '\n' {
				inLine = false
			} else if value != '\r' {
				out[index] = ' '
			}
		case inBlock:
			if value != '\n' && value != '\r' {
				out[index] = ' '
			}
			if value == '*' && index+1 < len(source) && source[index+1] == '/' {
				out[index+1] = ' '
				inBlock = false
				index++
			}
		case inString:
			if value != '\n' && value != '\r' {
				out[index] = ' '
			}
			if escaped {
				escaped = false
			} else if value == '\\' {
				escaped = true
			} else if value == '"' {
				inString = false
			}
		case inChar:
			if value != '\n' && value != '\r' {
				out[index] = ' '
			}
			if escaped {
				escaped = false
			} else if value == '\\' {
				escaped = true
			} else if value == '\'' {
				inChar = false
			}
		default:
			switch {
			case value == '/' && index+1 < len(source) && source[index+1] == '/':
				out[index], out[index+1] = ' ', ' '
				inLine = true
				index++
			case value == '/' && index+1 < len(source) && source[index+1] == '*':
				out[index], out[index+1] = ' ', ' '
				inBlock = true
				index++
			case value == '"':
				out[index] = ' '
				inString = true
			case value == '\'':
				out[index] = ' '
				inChar = true
			}
		}
	}
	return string(out)
}

// findBindClosingParen matches a call parenthesis while ignoring comments and literals.
func findBindClosingParen(source string, position int) int {
	masked := maskBindCommentsAndLiterals(source)
	depth := 1
	for index := position; index < len(masked); index++ {
		switch masked[index] {
		case '(':
			depth++
		case ')':
			depth--
			if depth == 0 {
				return index
			}
		}
	}
	return -1
}

// analyzeBindProject discovers supported wrapper definitions and invocations
// across the complete sorted source set. It does not allocate any Trice IDs.
func analyzeBindProject(plans []bindFilePlan, diagnose bool) (bindProjectModel, []bindDiagnostic) {
	model := bindProjectModel{wrappers: make(map[string]bindWrapperDefinition)}
	var diagnostics []bindDiagnostic
	definitionsByName := make(map[string][]bindWrapperDefinition)

	for planIndex := range plans {
		plan := &plans[planIndex]
		plan.macroDefinitions = scanBindMacroDefinitions(maskTriceInsertDisabledRegions(string(plan.final)))
		plan.invocations = nil
		plan.descriptors = nil
		plan.regions = nil
		plan.managedOffset = -1
		for siteIndex := range plan.sites {
			plan.sites[siteIndex].definitionName = ""
			plan.sites[siteIndex].definitionOrdinal = 0
			plan.sites[siteIndex].sourceOrdinal = 0
			plan.sites[siteIndex].counterSelected = false
		}

		for siteIndex := range plan.sites {
			site := &plan.sites[siteIndex]
			definitionIndex := bindContainingMacro(plan.macroDefinitions, site.loc[0])
			if definitionIndex < 0 {
				if !site.wasExplicit {
					plan.managedOffset = bindEarlierOffset(plan.managedOffset, site.loc[0])
				}
				continue
			}
			definition := &plan.macroDefinitions[definitionIndex]
			site.definitionName = definition.name
			site.definitionOrdinal = len(definition.siteIndexes)
			definition.siteIndexes = append(definition.siteIndexes, siteIndex)
			if bindSiteExtendsFormatWithStringification(string(plan.final), *site) {
				definition.hasFormatStringify = true
			}
			plan.managedOffset = bindEarlierOffset(plan.managedOffset, definition.start)
		}

		lineOrdinals := make(map[int]int)
		for siteIndex := range plan.sites {
			site := &plan.sites[siteIndex]
			if site.definitionName == "" {
				site.sourceOrdinal = lineOrdinals[site.line]
				lineOrdinals[site.line]++
			}
		}

		for macroIndex := range plan.macroDefinitions {
			definition := &plan.macroDefinitions[macroIndex]
			if bindIsUserTriceName(definition.name) && !bindRegisteredAlias(definition.name) {
				if diagnose {
					diagnostics = append(diagnostics, bindDiagnostic{path: plan.path, line: definition.line, message: "indirect redefinition of Trice macro " + definition.name + " is not supported by bind"})
				}
				continue
			}
			if len(definition.siteIndexes) == 0 {
				continue
			}
			wrapper := bindWrapperDefinition{name: definition.name, plan: planIndex, macro: macroIndex}
			for _, siteIndex := range definition.siteIndexes {
				wrapper.siteRefs = append(wrapper.siteRefs, bindSiteReference{plan: planIndex, site: siteIndex})
			}
			definitionsByName[definition.name] = append(definitionsByName[definition.name], wrapper)
			if !diagnose {
				continue
			}
			if !definition.functionLike {
				diagnostics = append(diagnostics, bindDiagnostic{path: plan.path, line: definition.line, message: "logging wrapper " + definition.name + " must be a function-like statement macro"})
			}
			if definition.hasTokenPaste {
				diagnostics = append(diagnostics, bindDiagnostic{path: plan.path, line: definition.line, message: "logging wrapper " + definition.name + " uses unsupported token pasting"})
			}
			if definition.hasFormatStringify {
				diagnostics = append(diagnostics, bindDiagnostic{path: plan.path, line: definition.line, message: "logging wrapper " + definition.name + " uses unsupported stringification to extend a Trice format string"})
			}
			if bindRegisteredAlias(definition.name) {
				diagnostics = append(diagnostics, bindDiagnostic{path: plan.path, line: definition.line, message: "logging wrapper " + definition.name + " conflicts with a configured Trice alias"})
			}
			for _, siteIndex := range definition.siteIndexes {
				if plan.sites[siteIndex].wasExplicit {
					diagnostics = append(diagnostics, bindDiagnostic{path: plan.path, line: plan.sites[siteIndex].line, message: "logging wrapper " + definition.name + " contains an explicit non-zero Trice ID; use trice insert/clean or remove the explicit ID"})
				}
			}
		}
	}

	for name, definitions := range definitionsByName {
		model.wrappers[name] = definitions[0]
		if len(definitions) < 2 {
			continue
		}
		wrapper := model.wrappers[name]
		wrapper.duplicate = true
		model.wrappers[name] = wrapper
		if diagnose {
			locations := make([]string, 0, len(definitions))
			for _, definition := range definitions {
				macro := plans[definition.plan].macroDefinitions[definition.macro]
				locations = append(locations, fmt.Sprintf("%s:%d", plans[definition.plan].path, macro.line))
			}
			diagnostics = append(diagnostics, bindDiagnostic{message: fmt.Sprintf("logging wrapper %s has multiple source definitions: %s", name, strings.Join(locations, ", "))})
		}
	}

	if diagnose {
		for _, wrapper := range model.wrappers {
			plan := &plans[wrapper.plan]
			definition := plan.macroDefinitions[wrapper.macro]
			replacement := string(plan.final[definition.replacementStart:definition.end])
			if nested := bindNestedWrapperName(replacement, model.wrappers); nested != "" {
				kind := "nested"
				if nested == wrapper.name {
					kind = "recursive"
				}
				diagnostics = append(diagnostics, bindDiagnostic{path: plan.path, line: definition.line, message: fmt.Sprintf("%s logging wrapper %s invokes %s; nested and recursive wrappers are not supported", kind, wrapper.name, nested)})
			}
		}
		for planIndex := range plans {
			plan := &plans[planIndex]
			for macroIndex := range plan.macroDefinitions {
				definition := plan.macroDefinitions[macroIndex]
				if len(definition.siteIndexes) != 0 {
					continue
				}
				replacement := string(plan.final[definition.replacementStart:definition.end])
				if nested := bindNestedWrapperName(replacement, model.wrappers); nested != "" {
					diagnostics = append(diagnostics, bindDiagnostic{
						path:    plan.path,
						line:    definition.line,
						message: fmt.Sprintf("nested logging wrapper %s invokes %s; nested and recursive wrappers are not supported", definition.name, nested),
					})
				}
			}
		}
	}

	for planIndex := range plans {
		plan := &plans[planIndex]
		invocations, invocationDiagnostics := scanBindWrapperInvocations(planIndex, plans, model)
		plan.invocations = invocations
		for _, invocation := range invocations {
			plan.managedOffset = bindEarlierOffset(plan.managedOffset, invocation.start)
		}
		if diagnose {
			diagnostics = append(diagnostics, invocationDiagnostics...)
		}
		if len(invocations) == 0 {
			continue
		}
		switch plan.class {
		case bindFileNone:
			plan.class = bindFileBound
		case bindFileInsert:
			plan.class = bindFileMixed
			if diagnose {
				diagnostics = append(diagnostics, bindDiagnostic{path: plan.path, message: "file mixes explicit non-zero IDs with bind-owned logging-wrapper invocations"})
			}
		}
	}
	return model, diagnostics
}

// bindSiteExtendsFormatWithStringification distinguishes preprocessing that
// changes a recognized static format from safe #parameter use in later data
// arguments or unrelated statements in the same wrapper replacement.
func bindSiteExtendsFormatWithStringification(source string, site bindSite) bool {
	closing := findBindClosingParen(source, site.loc[2])
	if closing < 0 || site.loc[6] < 0 || site.loc[6] >= closing {
		return false
	}
	masked := maskBindCommentsAndLiterals(source)
	position := skipBindSpace(masked, site.loc[6])
	return position < closing && masked[position] == '#' && (position+1 >= closing || masked[position+1] != '#')
}

// bindContainingMacro returns the definition whose replacement owns position.
func bindContainingMacro(definitions []bindMacroDefinition, position int) int {
	for index, definition := range definitions {
		if definition.replacementStart <= position && position < definition.end {
			return index
		}
	}
	return -1
}

// bindEarlierOffset keeps the earliest non-negative source position.
func bindEarlierOffset(current, candidate int) int {
	if current < 0 || candidate < current {
		return candidate
	}
	return current
}

// bindIsUserTriceName uses the configured shared matcher for exact macro names.
func bindIsUserTriceName(name string) bool {
	location := matchTypNameTRICE.FindStringIndex(name)
	return location != nil && location[0] == 0 && location[1] == len(name)
}

// bindRegisteredAlias prevents one token from being both a direct alias and a wrapper.
func bindRegisteredAlias(name string) bool {
	for _, alias := range append(append(ArrayFlag(nil), TriceAliases...), TriceSAliases...) {
		if alias == name {
			return true
		}
	}
	return false
}

// bindNestedWrapperName finds a direct call of any project logging wrapper.
func bindNestedWrapperName(replacement string, wrappers map[string]bindWrapperDefinition) string {
	masked := maskBindCommentsAndLiterals(replacement)
	for position := 0; position < len(masked); {
		if !bindIdentifierStart(masked[position]) {
			position++
			continue
		}
		start := position
		for position < len(masked) && isCIdentifierByte(masked[position]) {
			position++
		}
		name := masked[start:position]
		if _, exists := wrappers[name]; !exists {
			continue
		}
		if next := skipBindSpace(masked, position); next < len(masked) && masked[next] == '(' {
			return name
		}
	}
	return ""
}

// scanBindWrapperInvocations finds standalone source-level expansions without
// attempting to emulate preprocessing or to parse Trice calls independently.
func scanBindWrapperInvocations(planIndex int, plans []bindFilePlan, model bindProjectModel) ([]bindWrapperInvocation, []bindDiagnostic) {
	plan := &plans[planIndex]
	source := string(plan.final)
	masked := maskBindCommentsAndLiterals(maskTriceInsertDisabledRegions(source))
	var invocations []bindWrapperInvocation
	var diagnostics []bindDiagnostic
	for position := 0; position < len(masked); {
		if !bindIdentifierStart(masked[position]) {
			position++
			continue
		}
		start := position
		for position < len(masked) && isCIdentifierByte(masked[position]) {
			position++
		}
		name := masked[start:position]
		wrapper, exists := model.wrappers[name]
		if !exists || bindContainingMacro(plan.macroDefinitions, start) >= 0 {
			continue
		}
		definition := plans[wrapper.plan].macroDefinitions[wrapper.macro]
		if wrapper.plan == planIndex {
			if start < definition.end {
				continue
			}
		}
		opening := skipBindSpace(masked, position)
		if opening >= len(masked) || masked[opening] != '(' {
			continue
		}
		closing := findBindClosingParen(source, opening+1)
		if closing < 0 {
			diagnostics = append(diagnostics, bindDiagnostic{path: plan.path, line: sourceLine(source, start), message: "logging wrapper invocation " + name + " has no matching closing parenthesis"})
			continue
		}
		semicolon := skipBindSpace(masked, closing+1)
		line := sourceLine(source, start)
		end := closing + 1
		macroTerminated := false
		if semicolon < len(masked) && masked[semicolon] == ';' {
			end = semicolon + 1
		} else if !definition.hasTerminatingSemicolon {
			diagnostics = append(diagnostics, bindDiagnostic{path: plan.path, line: line, message: "logging wrapper " + name + " must be used as a standalone statement ending in ';' at the invocation or in the macro replacement"})
			continue
		} else {
			macroTerminated = true
		}
		if !bindStandaloneCallContext(masked, start) {
			diagnostics = append(diagnostics, bindDiagnostic{path: plan.path, line: line, message: "logging wrapper " + name + " is not in a supported standalone statement context"})
			continue
		}
		invocations = append(invocations, bindWrapperInvocation{
			name:            name,
			start:           start,
			end:             end,
			line:            line,
			column:          sourceColumn(source, start),
			comment:         strings.Join(strings.Fields(source[start:end]), " "),
			macroTerminated: macroTerminated,
		})
		position = end
	}
	sort.SliceStable(invocations, func(i, j int) bool { return invocations[i].start < invocations[j].start })
	return invocations, diagnostics
}

// bindStandaloneCallContext accepts statement starts, labels, and the body of
// an if/while condition while rejecting arguments, initializers, and returns.
func bindStandaloneCallContext(masked string, position int) bool {
	lineStartPosition := lineStart(masked, position)
	previous := position - 1
	for previous >= lineStartPosition && (masked[previous] == ' ' || masked[previous] == '\t') {
		previous--
	}
	if previous < lineStartPosition {
		return true
	}
	switch masked[previous] {
	case ';', '{', '}', ':', ')':
		return true
	}
	if isCIdentifierByte(masked[previous]) {
		end := previous + 1
		for previous >= lineStartPosition && isCIdentifierByte(masked[previous]) {
			previous--
		}
		word := masked[previous+1 : end]
		return word == "else" || word == "do"
	}
	return false
}

// planBindExecution classifies ordinary descriptors and counter-rebase regions.
func planBindExecution(plans []bindFilePlan, model bindProjectModel, diagnose bool) []bindDiagnostic {
	var diagnostics []bindDiagnostic
	for planIndex := range plans {
		for siteIndex := range plans[planIndex].sites {
			plans[planIndex].sites[siteIndex].counterSelected = false
		}
	}
	for planIndex := range plans {
		plan := &plans[planIndex]
		plan.descriptors = nil
		plan.regions = nil
		if plan.class != bindFileBound || plan.key == "" {
			continue
		}

		var occurrences []bindOccurrence
		for siteIndex := range plan.sites {
			site := &plan.sites[siteIndex]
			if site.wasExplicit || site.definitionName != "" {
				continue
			}
			end := bindDirectCallEnd(string(plan.final), *site)
			occurrences = append(occurrences, bindOccurrence{
				start:   site.loc[0],
				end:     end,
				line:    site.line,
				column:  site.column,
				refs:    []bindSiteReference{{plan: planIndex, site: siteIndex}},
				comment: site.comment,
			})
		}
		for _, invocation := range plan.invocations {
			wrapper, exists := model.wrappers[invocation.name]
			if !exists {
				continue
			}
			occurrences = append(occurrences, bindOccurrence{
				start:           invocation.start,
				end:             invocation.end,
				line:            invocation.line,
				column:          invocation.column,
				refs:            append([]bindSiteReference(nil), wrapper.siteRefs...),
				comment:         invocation.comment,
				wrapper:         true,
				macroTerminated: invocation.macroTerminated,
			})
		}
		sort.SliceStable(occurrences, func(i, j int) bool { return occurrences[i].start < occurrences[j].start })
		macroTerminatedEnds := make(map[int]bool)
		for _, occurrence := range occurrences {
			if occurrence.macroTerminated {
				macroTerminatedEnds[occurrence.end] = true
			}
		}
		// Overlapping tokens cannot receive independent preprocessor boundaries;
		// retain the diagnostic while excluding them from in-memory rendering.
		invalidOccurrences := make(map[int]bool)
		for index := 1; index < len(occurrences); index++ {
			previous := occurrences[index-1]
			current := occurrences[index]
			if current.start >= previous.end {
				continue
			}
			invalidOccurrences[previous.start] = true
			invalidOccurrences[current.start] = true
			if diagnose {
				diagnostics = append(diagnostics, bindDiagnostic{
					path:    plan.path,
					line:    current.line,
					message: "overlapping Trice or logging-wrapper invocations cannot be assigned independent bind descriptors",
				})
			}
		}

		byLine := make(map[int][]bindOccurrence)
		var lines []int
		for _, occurrence := range occurrences {
			if invalidOccurrences[occurrence.start] {
				continue
			}
			if _, exists := byLine[occurrence.line]; !exists {
				lines = append(lines, occurrence.line)
			}
			byLine[occurrence.line] = append(byLine[occurrence.line], occurrence)
		}
		sort.Ints(lines)
		regionOrdinal := 0
		for _, line := range lines {
			group := byLine[line]
			requiresCounter := len(group) > 1
			for _, occurrence := range group {
				if len(occurrence.refs) > 1 || occurrence.wrapper && sourceLine(string(plan.final), occurrence.end-1) != occurrence.line {
					requiresCounter = true
				}
			}
			if !requiresCounter {
				occurrence := group[0]
				plan.descriptors = append(plan.descriptors, bindSiteDescriptor{line: line, column: occurrence.column, ref: occurrence.refs[0], comment: occurrence.comment})
				continue
			}

			valid := true
			regionStartPosition := lineStart(string(plan.final), group[0].start)
			lastOccurrenceEnd := group[0].end
			for _, occurrence := range group[1:] {
				if occurrence.end > lastOccurrenceEnd {
					lastOccurrenceEnd = occurrence.end
				}
			}
			regionEndPosition := lineEndIncludingNewline(string(plan.final), lastOccurrenceEnd-1)
			if regionEndPosition == len(plan.final) && (len(plan.final) == 0 || plan.final[len(plan.final)-1] != '\n') {
				valid = false
				if diagnose {
					diagnostics = append(diagnostics, bindDiagnostic{path: plan.path, line: line, message: "counter-selected Trice statement region must end with a newline so generated rebase includes remain removable"})
				}
			}
			if !bindRebaseLineIsIndependent(string(plan.final), regionStartPosition, regionEndPosition, group[0].start, macroTerminatedEnds) {
				valid = false
				if diagnose {
					diagnostics = append(diagnostics, bindDiagnostic{path: plan.path, line: line, message: "counter-selected Trice statement region is not an independent block item; generated rebase directives cannot safely surround a control-flow continuation or closing outer scope"})
				}
			}
			regionCode := maskBindCommentsAndLiterals(string(plan.final[regionStartPosition:regionEndPosition]))
			if bindContainsIdentifier(regionCode, "__COUNTER__") {
				valid = false
				if diagnose {
					diagnostics = append(diagnostics, bindDiagnostic{path: plan.path, line: line, message: "counter-selected region contains an additional explicit __COUNTER__; use trice insert/clean or remove it"})
				}
			}
			if bindRebaseRegionContainsDirective(regionCode) {
				valid = false
				if diagnose {
					diagnostics = append(diagnostics, bindDiagnostic{path: plan.path, line: line, message: "counter-selected Trice statement region contains a preprocessing directive and cannot be surrounded safely"})
				}
			}
			for _, occurrence := range occurrences {
				if occurrence.line == line || occurrence.start < regionStartPosition || occurrence.start >= regionEndPosition {
					continue
				}
				valid = false
				if diagnose {
					diagnostics = append(diagnostics, bindDiagnostic{
						path:    plan.path,
						line:    occurrence.line,
						message: "a multi-line logging-wrapper invocation shares its boundary line with another Trice site; place the sites on separate lines",
					})
				}
			}

			var expansions []bindSiteReference
			for _, occurrence := range group {
				if occurrence.end <= occurrence.start || occurrence.end > regionEndPosition || !bindStandaloneCallContext(maskBindCommentsAndLiterals(string(plan.final)), occurrence.start) {
					valid = false
					if diagnose {
						diagnostics = append(diagnostics, bindDiagnostic{path: plan.path, line: line, message: "counter-selected Trice calls and logging wrappers must be complete standalone statements"})
					}
				}
				for _, ref := range occurrence.refs {
					site := &plans[ref.plan].sites[ref.site]
					if site.mode != bindSiteAuto || site.wasExplicit {
						valid = false
						if diagnose {
							diagnostics = append(diagnostics, bindDiagnostic{path: plan.path, line: line, message: fmt.Sprintf("counter-selected site %s uses %s; id(0), Id(0), ID(0), and explicit IDs require trice insert/clean or separate statement lines", site.comment, site.mode)})
						}
					}
					if site.definitionName != "" {
						definitionPlan := &plans[ref.plan]
						definitionIndex := bindContainingMacro(definitionPlan.macroDefinitions, site.loc[0])
						if definitionIndex >= 0 && definitionPlan.macroDefinitions[definitionIndex].hasCounter {
							valid = false
							if diagnose {
								diagnostics = append(diagnostics, bindDiagnostic{path: definitionPlan.path, line: site.line, message: "logging wrapper " + site.definitionName + " contains an additional __COUNTER__ inside a required rebase expansion"})
							}
						}
					}
					expansions = append(expansions, ref)
				}
			}
			if !valid {
				continue
			}
			for _, ref := range expansions {
				plans[ref.plan].sites[ref.site].counterSelected = true
			}
			plan.regions = append(plan.regions, bindRebaseRegion{
				scope:      fmt.Sprintf("%s_R%d", plan.key, regionOrdinal),
				line:       line,
				column:     group[0].column,
				start:      regionStartPosition,
				end:        regionEndPosition,
				expansions: expansions,
			})
			regionOrdinal++
		}
	}
	return diagnostics
}

// bindRebaseRegionContainsDirective rejects directives inside a multi-line
// invocation because directive handling within macro arguments is non-portable.
func bindRebaseRegionContainsDirective(masked string) bool {
	for start := 0; start < len(masked); {
		end := lineEndIncludingNewline(masked, start)
		if strings.HasPrefix(strings.TrimSpace(masked[start:end]), "#") {
			return true
		}
		start = end
	}
	return false
}

// bindRebaseLineIsIndependent rejects candidate regions whose surrounding
// C/C++ syntax would bind to, or leave the scope of, declarations emitted by
// BEGIN and END. The generator rejects these cases instead of changing control
// flow by inserting a declaration between two dependent statements.
func bindRebaseLineIsIndependent(source string, start, end, firstOccurrence int, macroTerminatedEnds map[int]bool) bool {
	masked := maskBindCommentsAndLiterals(source)
	lineCode := masked[start:end]
	depth := 0
	for index := 0; index < len(lineCode); index++ {
		switch lineCode[index] {
		case '{':
			depth++
		case '}':
			depth--
			if depth < 0 {
				return false
			}
		}
	}

	prefix := strings.TrimSpace(masked[start:firstOccurrence])
	if bindIdentifierAt(prefix, 0, "else") || bindIdentifierAt(prefix, 0, "catch") {
		return false
	}

	previous := start - 1
	for {
		for previous >= 0 && strings.ContainsRune(" \t\r\n\f\v", rune(masked[previous])) {
			previous--
		}
		if previous < 0 {
			break
		}
		previousStart := lineStart(masked, previous)
		previousEnd := lineEndIncludingNewline(source, previousStart)
		previousLine := strings.TrimSpace(source[previousStart:previousEnd])
		_, _, _, _, isRebaseInclude := parseBindCompactRebaseInclude(previousLine)
		if strings.HasPrefix(previousLine, "#") && (bindSidecarName.MatchString(previousLine) || isRebaseInclude) {
			previous = previousStart - 1
			continue
		}
		break
	}
	if previous < 0 {
		return true
	}
	if macroTerminatedEnds[previous+1] {
		return true
	}
	if isInsideMacroDefinition(source, previous) {
		return true
	}
	previousLine := strings.TrimSpace(masked[lineStart(masked, previous) : previous+1])
	if strings.HasPrefix(previousLine, "#") {
		return true
	}
	if masked[previous] == '}' && bindIdentifierAt(prefix, 0, "while") {
		return false
	}
	switch masked[previous] {
	case ';', '{', '}':
		return true
	default:
		return false
	}
}

// bindDirectCallEnd returns the byte immediately after an optional statement semicolon.
func bindDirectCallEnd(source string, site bindSite) int {
	closing := findBindClosingParen(source, site.loc[2])
	if closing < 0 {
		return site.loc[6]
	}
	end := closing + 1
	for end < len(source) && (source[end] == ' ' || source[end] == '\t') {
		end++
	}
	if end < len(source) && source[end] == ';' {
		end++
	}
	return end
}

// applyBindRebaseRegions inserts compact generated includes in reverse source order so
// all offsets calculated by the analysis remain valid.
func applyBindRebaseRegions(plans []bindFilePlan) {
	for planIndex := range plans {
		plan := &plans[planIndex]
		if len(plan.regions) == 0 {
			continue
		}
		regions := append([]bindRebaseRegion(nil), plan.regions...)
		sort.Slice(regions, func(i, j int) bool { return regions[i].start > regions[j].start })
		for _, region := range regions {
			newline := sourceNewline(string(plan.final))
			beginName := bindRebaseArtifactName(plan.sidecarName, plan.key, region.scope, "begin")
			endName := bindRebaseArtifactName(plan.sidecarName, plan.key, region.scope, "end")
			begin := renderBindRebaseSourceInclude("begin", region.scope, beginName, newline)
			end := renderBindRebaseSourceInclude("end", region.scope, endName, newline)
			updated := make([]byte, 0, len(plan.final)+len(begin)+len(end))
			updated = append(updated, plan.final[:region.start]...)
			updated = append(updated, begin...)
			updated = append(updated, plan.final[region.start:region.end]...)
			updated = append(updated, end...)
			updated = append(updated, plan.final[region.end:]...)
			plan.final = updated
		}
	}
}

// renderBindRebaseSourceInclude emits one exact, removable phase boundary.
func renderBindRebaseSourceInclude(kind, scope, helperName, newline string) []byte {
	return []byte(fmt.Sprintf("#include \"%s\" %s%s %s%s", helperName, bindRebaseIncludeMarker, kind, scope, newline))
}

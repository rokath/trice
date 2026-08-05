// SPDX-License-Identifier: MIT

package id

import (
	"fmt"
	"path/filepath"
	"regexp"
	"sort"
	"strconv"
	"strings"
)

var (
	// bindIncludeDirective matches physical include directives without interpreting preprocessing conditions.
	bindIncludeDirective = regexp.MustCompile(`(?m)^[\t ]*#[\t ]*include\b[^\r\n]*`)
	// bindSidecarName extracts the generated flat sidecar name and its embedded file key.
	bindSidecarName = regexp.MustCompile(`"(trice_[A-Za-z0-9_]+_(K[0-9A-F]{16})\.h)"`)
	// bindIDWrapper extracts the case-sensitive legacy stamp wrapper selected by insert.
	bindIDWrapper = regexp.MustCompile(`\b(?:iD|id|Id|ID)\b`)
	// bindSidecarKeyDefinition validates the identity declared by an existing generated sidecar.
	bindSidecarKeyDefinition = regexp.MustCompile(`(?m)^[\t ]*#[\t ]*define[\t ]+TRICE_BIND_FILE_KEY[\t ]+(K[0-9A-F]{16})\b`)
	// bindSidecarRouteDefinition validates the expansion-time route declared for the sidecar key.
	bindSidecarRouteDefinition = regexp.MustCompile(`(?m)^[\t ]*#[\t ]*define[\t ]+TRICE_BIND_ROUTE_(K[0-9A-F]{16})[\t ]+BIND\b`)
)

// analyzeBindFile classifies one source using the shared Trice matcher and marker masking.
func analyzeBindFile(input bindFileInput) bindFilePlan {
	plan := bindFilePlan{
		path:     input.path,
		info:     input.info,
		original: input.data,
		final:    input.data,
	}
	plan.includes = scanBindIncludes(string(input.data))
	plan.sites, plan.diagnostics = scanBindSites(input.path, string(input.data))

	var hasBindSite, hasExplicitSite bool
	for _, site := range plan.sites {
		if site.wasExplicit {
			hasExplicitSite = true
		} else {
			hasBindSite = true
		}
	}
	switch {
	case hasBindSite && hasExplicitSite:
		plan.class = bindFileMixed
		plan.diagnostics = append(plan.diagnostics, bindDiagnostic{path: input.path, message: "file mixes explicit non-zero IDs with ID-free or zero-placeholder Trice sites"})
	case hasBindSite:
		plan.class = bindFileBound
	case hasExplicitSite:
		plan.class = bindFileInsert
	case hasSidecarInclude(plan.includes):
		plan.class = bindFileBound
	default:
		plan.class = bindFileNone
	}

	var sidecarNames = make(map[string]string)
	for _, include := range plan.includes {
		if include.isSidecar {
			sidecarNames[include.name] = include.key
		}
	}
	if len(sidecarNames) > 1 {
		plan.diagnostics = append(plan.diagnostics, bindDiagnostic{path: input.path, message: "file includes conflicting Trice bind sidecars"})
	}
	names := make([]string, 0, len(sidecarNames))
	for name := range sidecarNames {
		names = append(names, name)
	}
	sort.Strings(names)
	if len(names) > 0 {
		plan.sidecarName = names[0]
		plan.key = sidecarNames[plan.sidecarName]
	}
	if plan.class == bindFileInsert && plan.key != "" {
		plan.diagnostics = append(plan.diagnostics, bindDiagnostic{path: input.path, message: "insert-owned file still contains a Trice bind sidecar include"})
	}
	return plan
}

// scanBindIncludes returns all physical includes and recognizes bind sidecars independently of comments.
func scanBindIncludes(source string) []bindInclude {
	locations := bindIncludeDirective.FindAllStringIndex(stripCComments(source), -1)
	includes := make([]bindInclude, 0, len(locations))
	for _, location := range locations {
		lineText := source[location[0]:location[1]]
		include := bindInclude{
			start:     location[0],
			end:       location[1],
			afterLine: lineEndIncludingNewline(source, location[1]),
			line:      sourceLine(source, location[0]),
		}
		match := bindSidecarName.FindStringSubmatch(lineText)
		if len(match) == 3 {
			include.name = match[1]
			include.key = match[2]
			include.isSidecar = true
		}
		includes = append(includes, include)
	}
	return includes
}

// scanBindSites uses matchTrice and the insert marker mask to retain exactly the existing parser surface.
func scanBindSites(path, source string) (sites []bindSite, diagnostics []bindDiagnostic) {
	masked := maskTriceInsertDisabledRegions(source)
	rest := masked
	offset := 0
	bindSitesPerLine := make(map[int]int)
	for {
		loc, matchIssues := matchTriceWithIssues(rest)
		for _, issue := range matchIssues {
			position := offset + issue.offset
			diagnostics = append(diagnostics, bindDiagnostic{path: path, line: sourceLine(source, position), message: issue.message})
		}
		if loc == nil {
			break
		}
		var absolute [7]int
		for i := range absolute {
			absolute[i] = offset + loc[i]
		}
		site := bindSite{
			line:    sourceLine(source, absolute[0]),
			column:  sourceColumn(source, absolute[0]),
			loc:     absolute,
			macro:   source[absolute[0]:absolute[1]],
			comment: bindSourceComment(source, absolute),
		}

		var trice TriceFmt
		trice.Type = site.macro
		resolveTriceAlias(&trice)
		if trice.isSAlias() {
			trice.Strg = SAliasStrgPrefix + source[absolute[5]:absolute[6]] + SAliasStrgSuffix
		} else if absolute[5] < absolute[6] {
			trice.Strg = source[absolute[5]+1 : absolute[6]-1]
		}
		site.format = trice.Strg

		if absolute[3] == absolute[4] {
			site.mode = bindSiteAuto
			site.wrapper = bindDefaultWrapper(trice.Type)
		} else {
			idText := source[absolute[3]:absolute[4]]
			wrapper := bindIDWrapper.FindString(idText)
			number := matchNb.FindString(idText)
			if wrapper == "" || number == "" {
				diagnostics = append(diagnostics, bindDiagnostic{path: path, line: site.line, message: "Trice ID wrapper does not contain one decimal ID"})
			} else {
				value, err := strconv.Atoi(number)
				if err != nil {
					diagnostics = append(diagnostics, bindDiagnostic{path: path, line: site.line, message: fmt.Sprintf("invalid Trice ID %q", number)})
				} else {
					site.id = TriceID(value)
					site.wrapper = wrapper
					site.wasExplicit = value > 0
					site.mode = bindSiteReplace
				}
			}
		}

		if isInsideMacroDefinition(source, absolute[0]) {
			diagnostics = append(diagnostics, bindDiagnostic{path: path, line: site.line, message: "Trice site inside a preprocessor macro definition is not supported by bind"})
		}
		if !site.wasExplicit {
			bindSitesPerLine[site.line]++
			if bindSitesPerLine[site.line] == 2 {
				diagnostics = append(diagnostics, bindDiagnostic{path: path, line: site.line, message: "multiple bindable Trice sites on one physical source line are not supported"})
			}
		}
		sites = append(sites, site)
		offset = absolute[6]
		rest = masked[offset:]
	}
	return sites, diagnostics
}

// bindDefaultWrapper mirrors writeID's established lowercase and uppercase stamp selection.
func bindDefaultWrapper(macro string) string {
	if len(macro) > 2 && macro[2] == 'i' {
		return "iD"
	}
	switch DefaultStampSize {
	case 16:
		return "Id"
	case 32:
		return "ID"
	default:
		return "id"
	}
}

// hasSidecarInclude reports whether direct source text already establishes bind ownership.
func hasSidecarInclude(includes []bindInclude) bool {
	for _, include := range includes {
		if include.isSidecar {
			return true
		}
	}
	return false
}

// hasTriceBindSidecarInclude is shared with insert so bound sources are never instrumented accidentally.
func hasTriceBindSidecarInclude(source string) bool {
	return hasSidecarInclude(scanBindIncludes(source))
}

// bindSidecarFilename creates the specified readable name while the random key provides identity.
func bindSidecarFilename(sourcePath, key string) string {
	base := filepath.Base(sourcePath)
	var normalized strings.Builder
	for _, r := range base {
		if r == '_' || '0' <= r && r <= '9' || 'A' <= r && r <= 'Z' || 'a' <= r && r <= 'z' {
			normalized.WriteRune(r)
		} else {
			normalized.WriteByte('_')
		}
	}
	return "trice_" + normalized.String() + "_" + key + ".h"
}

// addBindInclude applies the conservative placement rule and returns a re-parsed final source.
func addBindInclude(plan *bindFilePlan) {
	if plan.key == "" || plan.sidecarName == "" || len(plan.sites) == 0 {
		return
	}
	if hasSidecarInclude(plan.includes) {
		validateActiveBindIncludes(plan)
		return
	}
	firstSite := plan.sites[0]
	insertAt := lineStart(string(plan.final), firstSite.loc[0])
	for _, include := range plan.includes {
		if include.start > firstSite.loc[0] {
			plan.diagnostics = append(plan.diagnostics, bindDiagnostic{
				path:    plan.path,
				line:    firstSite.line,
				message: fmt.Sprintf("cannot place bind include safely before a later include; add #include \"%s\" manually as the last include before this file's Trice calls", plan.sidecarName),
			})
			return
		}
		insertAt = include.afterLine
	}
	newline := sourceNewline(string(plan.final))
	includeLine := fmt.Sprintf("#include \"%s\" // trice-bind: keep as last include before this file's Trice calls", plan.sidecarName)
	prefix := ""
	if insertAt > 0 && plan.final[insertAt-1] != '\n' && plan.final[insertAt-1] != '\r' {
		prefix = newline
	}
	insert := []byte(prefix + includeLine + newline)
	updated := make([]byte, 0, len(plan.final)+len(insert))
	updated = append(updated, plan.final[:insertAt]...)
	updated = append(updated, insert...)
	updated = append(updated, plan.final[insertAt:]...)
	plan.final = updated
	plan.includeAdded = true
	plan.includes = scanBindIncludes(string(plan.final))
	plan.sites, _ = scanBindSites(plan.path, string(plan.final))
	validateActiveBindIncludes(plan)
}

// validateActiveBindIncludes requires the owner's sidecar to be the last include before every managed site.
func validateActiveBindIncludes(plan *bindFilePlan) {
	for _, site := range plan.sites {
		if site.wasExplicit {
			continue
		}
		var active *bindInclude
		for i := range plan.includes {
			include := &plan.includes[i]
			if include.start >= site.loc[0] {
				break
			}
			active = include
		}
		if active == nil || !active.isSidecar || active.key != plan.key || active.name != plan.sidecarName {
			plan.diagnostics = append(plan.diagnostics, bindDiagnostic{
				path:    plan.path,
				line:    site.line,
				message: fmt.Sprintf("file key is not active unambiguously; add #include \"%s\" as the last include before this Trice site", plan.sidecarName),
			})
		}
	}
}

// validateExistingSidecar checks that an existing generated file declares the key encoded in its name.
func validateExistingSidecar(path, key string, content []byte) []bindDiagnostic {
	keyDefinitions := bindSidecarKeyDefinition.FindAllSubmatch(content, -1)
	if len(keyDefinitions) == 0 {
		return []bindDiagnostic{{path: path, message: "existing Trice bind sidecar has no valid TRICE_BIND_FILE_KEY definition"}}
	}
	var diagnostics []bindDiagnostic
	for _, definition := range keyDefinitions {
		if string(definition[1]) != key {
			diagnostics = append(diagnostics, bindDiagnostic{path: path, message: fmt.Sprintf("sidecar declares key %s but its owner include uses %s", definition[1], key)})
		}
	}
	routeDefinitions := bindSidecarRouteDefinition.FindAllSubmatch(content, -1)
	if len(routeDefinitions) == 0 {
		diagnostics = append(diagnostics, bindDiagnostic{path: path, message: "existing Trice bind sidecar has no valid BIND route definition"})
	}
	for _, definition := range routeDefinitions {
		if string(definition[1]) != key {
			diagnostics = append(diagnostics, bindDiagnostic{path: path, message: fmt.Sprintf("sidecar declares route for key %s but its owner include uses %s", definition[1], key)})
		}
	}
	return diagnostics
}

// bindSourceComment normalizes one source call without allowing a comment to continue a macro line.
func bindSourceComment(source string, loc [7]int) string {
	closing := findClosingParentis(source, loc[2])
	if closing < loc[0] {
		closing = loc[6]
	}
	end := closing + 1
	if end < len(source) && source[end] == ';' {
		end++
	}
	call := source[loc[0]:end]
	if newline := strings.IndexAny(call, "\r\n"); newline >= 0 {
		call = strings.TrimSpace(call[:newline]) + " ..."
	} else {
		call = strings.TrimSpace(call)
	}
	call = strings.TrimSuffix(call, "\\")
	return strings.Join(strings.Fields(call), " ")
}

// isInsideMacroDefinition follows physical continuation lines back to the controlling directive.
func isInsideMacroDefinition(source string, position int) bool {
	start := lineStart(source, position)
	for start > 0 {
		previousEnd := start
		previousStart := lineStart(source, start-1)
		previous := strings.TrimRight(source[previousStart:previousEnd], "\r\n\t ")
		if !strings.HasSuffix(previous, "\\") {
			break
		}
		start = previousStart
	}
	lineEnd := strings.IndexAny(source[start:], "\r\n")
	if lineEnd < 0 {
		lineEnd = len(source)
	} else {
		lineEnd += start
	}
	line := strings.TrimSpace(source[start:lineEnd])
	if !strings.HasPrefix(line, "#") {
		return false
	}
	line = strings.TrimSpace(strings.TrimPrefix(line, "#"))
	return strings.HasPrefix(line, "define") && (len(line) == len("define") || line[len("define")] == ' ' || line[len("define")] == '\t')
}

// sourceLine returns the one-based physical line at a byte position.
func sourceLine(source string, position int) int {
	return 1 + strings.Count(source[:position], "\n")
}

// sourceColumn returns the one-based byte column used for deterministic diagnostics.
func sourceColumn(source string, position int) int {
	return position - lineStart(source, position) + 1
}

// lineStart returns the byte position immediately after the preceding newline.
func lineStart(source string, position int) int {
	if position > len(source) {
		position = len(source)
	}
	if index := strings.LastIndex(source[:position], "\n"); index >= 0 {
		return index + 1
	}
	return 0
}

// lineEndIncludingNewline returns the first byte after one complete physical line.
func lineEndIncludingNewline(source string, position int) int {
	for position < len(source) && source[position] != '\n' && source[position] != '\r' {
		position++
	}
	if position < len(source) && source[position] == '\r' {
		position++
	}
	if position < len(source) && source[position] == '\n' {
		position++
	}
	return position
}

// sourceNewline preserves CRLF projects and otherwise emits the portable LF separator.
func sourceNewline(source string) string {
	if strings.Contains(source, "\r\n") {
		return "\r\n"
	}
	return "\n"
}

// SPDX-License-Identifier: MIT

package vis

import (
	"errors"
	"fmt"
	"io"
	"net"
	"os"
	"path/filepath"
	"sort"
	"strconv"
	"strings"
	"sync"

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/id"
	"github.com/spf13/afero"
)

// Router evaluates enabled -vis rules synchronously after normal pick/ban filtering.
type Router struct {
	rules      []*runtimeRule
	sinks      map[string]*sharedSink
	diagnostic io.Writer
	verbose    bool
}

// runtimeRule augments one parsed rule with candidate, stamp, sink, and disable state.
type runtimeRule struct {
	parsedRule
	number              int
	enabled             bool
	candidates          map[id.TriceID]candidateState
	compatibleCount     int
	sinkState           *sharedSink
	genericStampBits    int
	genericStampFirstID id.TriceID
	router              *Router
}

// candidateFingerprint detects a changed LUT entry when its next runtime record arrives.
type candidateFingerprint struct {
	typeName string
	format   string
}

// candidateState retains one independent compatibility result for a rule and ID.
type candidateState struct {
	fingerprint candidateFingerprint
	compatible  bool
	reason      string
}

// sharedSink serializes records from every rule targeting one normalized destination.
type sharedSink struct {
	display string
	writer  io.WriteCloser
	failed  bool
	rules   []*runtimeRule
	mutex   sync.Mutex // serializes record writes with command-shutdown closure
}

// sinkDescription resolves a configured destination before an operating-system handle is opened.
type sinkDescription struct {
	key     string
	display string
	open    func() (io.WriteCloser, error)
}

// NewRouter parses all rules, classifies the current LUT, and opens every needed sink.
//
// Syntax and sink initialization errors are fatal to command startup. A rule
// with no compatible historical candidate is instead disabled with a prominent
// warning so unrelated normal logging and other rules can continue.
func NewRouter(
	diagnostic io.Writer,
	fileSystem *afero.Afero,
	lut id.TriceIDLookUp,
	lutMutex *sync.RWMutex,
	specifications []string,
	verbose bool,
) (*Router, error) {
	if len(specifications) == 0 {
		return nil, nil
	}
	if diagnostic == nil {
		diagnostic = io.Discard
	}
	if fileSystem == nil {
		fileSystem = &afero.Afero{Fs: afero.NewOsFs()}
	}
	if lutMutex == nil {
		lutMutex = new(sync.RWMutex)
	}

	router := &Router{
		sinks:      make(map[string]*sharedSink),
		diagnostic: diagnostic,
		verbose:    verbose,
	}
	for i, specification := range specifications {
		parsed, err := parseRule(specification)
		if err != nil {
			return nil, fmt.Errorf("-vis rule %d %q: %w", i+1, specification, err)
		}
		router.rules = append(router.rules, &runtimeRule{
			parsedRule: parsed,
			number:     i + 1,
			enabled:    true,
			candidates: make(map[id.TriceID]candidateState),
			router:     router,
		})
	}

	// Classify a stable snapshot so concurrent LUT refresh cannot mix old and new entries.
	lutMutex.RLock()
	snapshot := make(id.TriceIDLookUp, len(lut))
	for triceID, triceFormat := range lut {
		snapshot[triceID] = triceFormat
	}
	lutMutex.RUnlock()

	ids := make([]int, 0, len(snapshot))
	for triceID := range snapshot {
		ids = append(ids, int(triceID))
	}
	sort.Ints(ids)
	for _, rule := range router.rules {
		for _, numericID := range ids {
			triceID := id.TriceID(numericID)
			triceFormat := snapshot[triceID]
			if !rule.matchesFormat(triceFormat.Strg) {
				continue
			}
			rule.classifyCandidate(triceID, triceFormat)
		}
		switch {
		case len(rule.candidates) == 0:
			rule.disable(fmt.Sprintf("no til.json entry matches tag %q", rule.tag))
		case rule.compatibleCount == 0:
			rule.disable(fmt.Sprintf(
				"no compatible til.json candidate remains for tag %q; candidates: %s",
				rule.tag, formatCandidateDetails(rule.candidates),
			))
		}
	}

	for _, rule := range router.rules {
		if !rule.enabled {
			continue
		}
		description, err := describeSink(fileSystem, rule.sink)
		if err != nil {
			_ = router.Close()
			return nil, fmt.Errorf("-vis rule %d %q: %w", rule.number, rule.raw, err)
		}
		sink := router.sinks[description.key]
		if sink == nil {
			writer, err := description.open()
			if err != nil {
				_ = router.Close()
				return nil, fmt.Errorf(
					"-vis rule %d %q: cannot open sink %q: %w",
					rule.number, rule.raw, rule.sink, err,
				)
			}
			sink = &sharedSink{
				display: description.display,
				writer:  writer,
			}
			router.sinks[description.key] = sink
		}
		rule.sinkState = sink
		sink.rules = append(sink.rules, rule)
	}
	return router, nil
}

// Close releases each shared file or UDP destination exactly once.
func (router *Router) Close() error {
	if router == nil {
		return nil
	}
	keys := make([]string, 0, len(router.sinks))
	for key := range router.sinks {
		keys = append(keys, key)
	}
	sort.Strings(keys)
	var closeErrors []error
	for _, key := range keys {
		sink := router.sinks[key]
		sink.mutex.Lock()
		if sink.writer == nil {
			sink.mutex.Unlock()
			continue
		}
		if err := sink.writer.Close(); err != nil {
			closeErrors = append(closeErrors, fmt.Errorf("close visualization sink %q: %w", sink.display, err))
		}
		sink.writer = nil
		sink.mutex.Unlock()
	}
	return errors.Join(closeErrors...)
}

// Process attempts every matching rule and reports whether normal output should be dropped.
//
// partialLine is true when the normal line composer already contains an earlier
// Trice, which makes this record part of an unsupported multi-Trice line.
func (router *Router) Process(record decoder.VisRecord, partialLine bool) bool {
	if router == nil {
		return false
	}
	dropNormalOutput := false
	for _, rule := range router.rules {
		if !rule.enabled || !rule.matchesFormat(record.Format) {
			continue
		}
		if !rule.ensureCandidate(record) {
			continue
		}
		if !record.SingleLine || partialLine {
			rule.verbosef(
				"ignored ID %d because it does not form one self-contained single-Trice log line",
				record.ID,
			)
			continue
		}
		if !rule.acceptsStamp(record) {
			continue
		}

		encoded, err := rule.encode(record)
		if err != nil {
			rule.disable(fmt.Sprintf("runtime encoder failure for ID %d: %v", record.ID, err))
			continue
		}
		if err := rule.write(encoded); err != nil {
			rule.failSharedSink(record.ID, err)
			continue
		}
		if rule.policy == outputDrop {
			dropNormalOutput = true
		}
	}
	return dropNormalOutput
}

// matchesFormat applies the exact tag-plus-colon selector to the original LUT format.
func (rule *runtimeRule) matchesFormat(format string) bool {
	return strings.HasPrefix(format, rule.tag+":")
}

// classifyCandidate updates one independent ID classification and emits its verbose diagnostic once.
func (rule *runtimeRule) classifyCandidate(triceID id.TriceID, triceFormat id.TriceFmt) bool {
	fingerprint := candidateFingerprint{typeName: triceFormat.Type, format: triceFormat.Strg}
	old, existed := rule.candidates[triceID]
	if existed && old.fingerprint == fingerprint {
		return old.compatible
	}
	if existed && old.compatible {
		rule.compatibleCount--
	}

	shape, err := fixedNumericShape(triceFormat)
	if err == nil {
		for i, expression := range rule.expressions {
			if expressionErr := validateExpressionVerb(expression, rule.verbs[i], &shape); expressionErr != nil {
				err = fmt.Errorf("expression %d (%s): %w", i+1, expression.source, expressionErr)
				break
			}
		}
	}
	state := candidateState{fingerprint: fingerprint, compatible: err == nil}
	if err != nil {
		state.reason = err.Error()
		rule.verbosef("excluded til.json ID %d: %s", triceID, state.reason)
	} else {
		rule.compatibleCount++
	}
	rule.candidates[triceID] = state
	return state.compatible
}

// ensureCandidate reclassifies a new or changed runtime entry before using its values.
func (rule *runtimeRule) ensureCandidate(record decoder.VisRecord) bool {
	compatible := rule.classifyCandidate(record.ID, id.TriceFmt{
		Type: record.Type,
		Strg: record.Format,
	})
	if compatible {
		return true
	}
	if rule.enabled && rule.compatibleCount == 0 {
		rule.disable(fmt.Sprintf(
			"runtime LUT change left no compatible candidate for tag %q; candidates: %s",
			rule.tag, formatCandidateDetails(rule.candidates),
		))
	}
	return false
}

// acceptsStamp enforces explicit widths and latches generic ts only after all earlier eligibility checks.
func (rule *runtimeRule) acceptsStamp(record decoder.VisRecord) bool {
	usesStamp := rule.references.genericTS || rule.references.ts16 || rule.references.ts32
	if !usesStamp {
		return true
	}
	if record.StampBits == 0 {
		rule.verbosef("ignored unstamped ID %d because the rule references a Target-Stamp", record.ID)
		return false
	}

	explicitBits := 0
	switch {
	case rule.references.ts16:
		explicitBits = 16
	case rule.references.ts32:
		explicitBits = 32
	}
	if explicitBits != 0 && record.StampBits != explicitBits {
		rule.verbosef(
			"ignored ID %d with a %d-bit Target-Stamp because the rule requires ts%d",
			record.ID, record.StampBits, explicitBits,
		)
		return false
	}
	if !rule.references.genericTS {
		return true
	}
	if rule.genericStampBits == 0 {
		rule.genericStampBits = record.StampBits
		rule.genericStampFirstID = record.ID
		return true
	}
	if rule.genericStampBits != record.StampBits {
		rule.disable(fmt.Sprintf(
			"generic ts changed from %d bits at ID %d to %d bits at ID %d",
			rule.genericStampBits, rule.genericStampFirstID, record.StampBits, record.ID,
		))
		return false
	}
	return true
}

// encode evaluates and converts all arguments before producing one complete temporary record.
func (rule *runtimeRule) encode(record decoder.VisRecord) ([]byte, error) {
	arguments := make([]interface{}, len(rule.expressions))
	for i, expression := range rule.expressions {
		value, err := expression.root.evaluate(record)
		if err != nil {
			return nil, fmt.Errorf("expression %d (%s): %w", i+1, expression.source, err)
		}
		argument, err := argumentForVerb(value, rule.verbs[i])
		if err != nil {
			return nil, fmt.Errorf("expression %d (%s): %w", i+1, expression.source, err)
		}
		arguments[i] = argument
	}
	encoded := fmt.Sprintf(rule.format, arguments...)
	if strings.Contains(encoded, "%!") {
		return nil, fmt.Errorf("formatter produced a Go diagnostic instead of a record")
	}
	return []byte(encoded), nil
}

// write performs exactly one synchronous sink write for one encoded record.
func (rule *runtimeRule) write(record []byte) error {
	if rule.sinkState == nil {
		return fmt.Errorf("sink %q is unavailable", rule.sink)
	}
	sink := rule.sinkState
	sink.mutex.Lock()
	defer sink.mutex.Unlock()
	if sink.writer == nil {
		return fmt.Errorf("sink %q is unavailable", sink.display)
	}
	if sink.failed {
		return fmt.Errorf("sink %q has already failed", sink.display)
	}
	written, err := sink.writer.Write(record)
	if err != nil {
		return err
	}
	if written != len(record) {
		return io.ErrShortWrite
	}
	return nil
}

// failSharedSink marks one destination failed and visibly disables every attached rule.
func (rule *runtimeRule) failSharedSink(triceID id.TriceID, writeError error) {
	sink := rule.sinkState
	if sink == nil {
		rule.disable(fmt.Sprintf("sink %q failed for ID %d: %v", rule.sink, triceID, writeError))
		return
	}
	sink.mutex.Lock()
	if sink.failed {
		sink.mutex.Unlock()
		return
	}
	sink.failed = true
	attachedRules := append([]*runtimeRule(nil), sink.rules...)
	sink.mutex.Unlock()
	for _, attached := range attachedRules {
		attached.disable(fmt.Sprintf(
			"shared sink %q failed while writing ID %d: %v",
			sink.display, triceID, writeError,
		))
	}
}

// disable permanently switches off one rule and always emits the required prominent warning.
func (rule *runtimeRule) disable(reason string) {
	if !rule.enabled {
		return
	}
	rule.enabled = false
	fmt.Fprintf(
		rule.router.diagnostic,
		"WARNING: -vis rule %d disabled: %s; rule=%q; normal logging continues\n",
		rule.number, reason, rule.raw,
	)
}

// verbosef reports one non-disabling classification or per-record ignore reason.
func (rule *runtimeRule) verbosef(format string, arguments ...interface{}) {
	if !rule.router.verbose {
		return
	}
	message := fmt.Sprintf(format, arguments...)
	fmt.Fprintf(rule.router.diagnostic, "vis: rule %d: %s\n", rule.number, message)
}

// fixedNumericShape reconstructs the same supported TREX scalar layout used by the decoder.
func fixedNumericShape(triceFormat id.TriceFmt) (candidateShape, error) {
	typeName := triceFormat.Type
	if before, _, found := strings.Cut(typeName, "Assert"); found {
		typeName = strings.TrimSpace(before)
	}
	if len(typeName) < 5 || !strings.EqualFold(typeName[:5], "TRICE") {
		return candidateShape{}, fmt.Errorf("type %q is not a Trice type", triceFormat.Type)
	}

	_, formatKinds := decoder.UReplaceN(triceFormat.Strg)
	fullType, err := id.ConstructFullTriceInfo(typeName, len(formatKinds))
	if err != nil {
		return candidateShape{}, fmt.Errorf("cannot reconstruct type %q: %w", triceFormat.Type, err)
	}
	bits, count, err := parseFixedNumericType(fullType)
	if err != nil {
		return candidateShape{}, err
	}
	if count != len(formatKinds) {
		return candidateShape{}, fmt.Errorf(
			"type %q expects %d values but its format contains %d conversions",
			triceFormat.Type, count, len(formatKinds),
		)
	}

	shape := candidateShape{count: min(count, decoder.VisValueCapacity)}
	for i, formatKind := range formatKinds {
		var kind scalarKind
		switch formatKind {
		case decoder.UnsignedFormatSpecifier, decoder.PointerFormatSpecifier:
			kind = scalarUnsigned
		case decoder.SignedFormatSpecifier:
			kind = scalarSigned
		case decoder.FloatFormatSpecifier:
			if bits != 32 && bits != 64 {
				return candidateShape{}, fmt.Errorf("%d-bit TREX values cannot carry floating-point data", bits)
			}
			kind = scalarFloat
		case decoder.BooleanFormatSpecifier:
			kind = scalarBool
		default:
			return candidateShape{}, fmt.Errorf("format conversion %d is not a supported numeric value", i+1)
		}
		if i < decoder.VisValueCapacity {
			shape.values[i] = kind
		}
	}
	return shape, nil
}

// parseFixedNumericType accepts only TRICE_0 and the fixed TRICE{8,16,32,64}_N forms.
func parseFixedNumericType(typeName string) (bits int, count int, err error) {
	upper := strings.ToUpper(typeName)
	if upper == "TRICE_0" {
		return 0, 0, nil
	}
	if !strings.HasPrefix(upper, "TRICE") {
		return 0, 0, fmt.Errorf("type %q is not supported by the TREX visualization MVP", typeName)
	}
	widthText, countText, found := strings.Cut(upper[5:], "_")
	if !found || strings.Contains(countText, "_") {
		return 0, 0, fmt.Errorf("type %q is not a fixed-width numeric TREX message", typeName)
	}
	bits, err = strconv.Atoi(widthText)
	if err != nil || (bits != 8 && bits != 16 && bits != 32 && bits != 64) {
		return 0, 0, fmt.Errorf("type %q has an unsupported numeric width", typeName)
	}
	count, err = strconv.Atoi(countText)
	if err != nil || count < 1 {
		return 0, 0, fmt.Errorf("type %q has an unsupported value count", typeName)
	}
	return bits, count, nil
}

// describeSink normalizes one file or UDP destination while preserving record-oriented writes.
func describeSink(fileSystem *afero.Afero, specification string) (sinkDescription, error) {
	if err := validateSinkSpecification(specification); err != nil {
		return sinkDescription{}, err
	}
	switch {
	case strings.HasPrefix(specification, "udp://"):
		addressText := strings.TrimPrefix(specification, "udp://")
		address, err := net.ResolveUDPAddr("udp", addressText)
		if err != nil {
			return sinkDescription{}, fmt.Errorf("cannot resolve UDP sink %q: %w", specification, err)
		}
		key := "udp://" + address.String()
		return sinkDescription{
			key:     key,
			display: specification,
			open: func() (io.WriteCloser, error) {
				return net.DialUDP("udp", nil, address)
			},
		}, nil
	}

	path := specification
	if strings.HasPrefix(path, "file:") {
		path = strings.TrimPrefix(path, "file:")
	}
	cleanPath := filepath.Clean(path)
	if info, statErr := fileSystem.Stat(cleanPath); statErr == nil && !info.Mode().IsRegular() {
		return sinkDescription{}, fmt.Errorf("file sink %q is not a regular file", specification)
	}
	absolutePath, err := filepath.Abs(cleanPath)
	if err != nil {
		return sinkDescription{}, fmt.Errorf("cannot normalize file sink %q: %w", specification, err)
	}
	return sinkDescription{
		key:     "file:" + filepath.Clean(absolutePath),
		display: cleanPath,
		open: func() (io.WriteCloser, error) {
			return fileSystem.OpenFile(cleanPath, os.O_CREATE|os.O_WRONLY|os.O_APPEND, 0o644)
		},
	}, nil
}

// formatCandidateDetails includes deterministic per-ID exclusion reasons in a disable warning.
func formatCandidateDetails(candidates map[id.TriceID]candidateState) string {
	ids := make([]int, 0, len(candidates))
	for triceID := range candidates {
		ids = append(ids, int(triceID))
	}
	sort.Ints(ids)
	parts := make([]string, 0, len(ids))
	for _, numericID := range ids {
		state := candidates[id.TriceID(numericID)]
		reason := state.reason
		if reason == "" {
			reason = "compatible"
		}
		parts = append(parts, fmt.Sprintf("%d (%s)", numericID, reason))
	}
	return strings.Join(parts, "; ")
}

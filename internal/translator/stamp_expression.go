// SPDX-License-Identifier: MIT

package translator

import (
	"fmt"
	"io"
	"strings"
	"sync"

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/printf"
)

// stampField distinguishes independent expression instances and stamp widths.
// One translator run represents one source; a reconnect constructs fresh state.
type stampField struct {
	bits  int
	delta bool
}

// stampConfigurations enumerates only the four switches with expression support.
func stampConfigurations() map[stampField]string {
	return map[stampField]string{
		{16, false}: decoder.TargetStamp16,
		{32, false}: decoder.TargetStamp32,
		{16, true}:  decoder.TargetStamp16Delta,
		{32, true}:  decoder.TargetStamp32Delta,
	}
}

// switchName identifies a field in startup and runtime diagnostics.
func (f stampField) switchName() string {
	name := fmt.Sprintf("-ts%d", f.bits)
	if f.delta {
		name += "delta"
	}
	return name
}

// compileStampExpressions reserves the exact printf( prefix and never falls back
// to legacy formatting after that prefix has selected expression mode.
func compileStampExpressions() (map[stampField]*printf.Program, error) {
	programs := make(map[stampField]*printf.Program)
	for field, format := range stampConfigurations() {
		if !strings.HasPrefix(format, "printf(") {
			continue
		}
		program, err := printf.ParseStamp(format, field.bits, field.delta)
		if err != nil {
			return nil, fmt.Errorf("%s: %w", field.switchName(), err)
		}
		programs[field] = &program
	}
	return programs, nil
}

// PrepareTargetStampFormats validates configuration before the input is opened.
// Runtime state is created separately for each translator session.
func PrepareTargetStampFormats() error { return prepareTargetStampFormats() }

// observeStamp snapshots all applicable expressions before event selection.
// Only decoded records supply samples; neither diagnostics nor missing stamps do.
func (state *targetStampState) observeStamp(record decoder.ApplicationRecord, available bool) {
	state.samples = nil
	if len(state.programs) == 0 {
		return
	}
	if !available || (record.StampBits != 16 && record.StampBits != 32) {
		return
	}
	state.samples = make(map[stampField]*printf.Sample)
	for field, program := range state.programs {
		if field.bits != record.StampBits {
			continue
		}
		sample := program.Observe(decoder.VisRecord{StampBits: record.StampBits, Stamp: record.Stamp})
		state.samples[field] = sample
		if err := sample.ObservationError(); err != nil {
			state.stampDiagnostic(field, err)
		}
	}
}

// stampDiagnostic keeps errors off structured application sinks.
func (state *targetStampState) stampDiagnostic(field stampField, err error) {
	if state.diagnostics != nil {
		fmt.Fprintf(state.diagnostics, "%s unavailable: %v\n", field.switchName(), err)
	}
}

// expressionStamp reads the current saved event. Repeated consumers reuse the
// sample's rendered result and therefore cannot commit a baseline twice.
func (state *targetStampState) expressionStamp(size int, delta bool) (text string, missing bool, active bool) {
	field := stampField{size * 8, delta}
	program := state.programs[field]
	if program == nil {
		return "", false, false
	}
	sample := state.samples[field]
	if sample == nil {
		return program.Placeholder("?"), true, true
	}
	text, missing, err := sample.Render()
	if err != nil && sample.ObservationError() == nil {
		state.stampDiagnostic(field, err)
	}
	return text, missing, true
}

// absoluteStamp and deltaStamp preserve legacy formatting for ordinary strings.
func (state *targetStampState) absoluteStamp(size int, stamp uint64) string {
	if text, _, active := state.expressionStamp(size, false); active {
		return text
	}
	return renderTargetStamp(size, stamp)
}

// deltaStamp commits expression history only when this field is selected.
func (state *targetStampState) deltaStamp(size int, stamp uint64) string {
	if text, _, active := state.expressionStamp(size, true); active {
		return text
	}
	return renderTargetDelta(size, stamp, state)
}

// placeholderKey keeps alignment caching separate from live counter histories.
type placeholderKey struct {
	size   int
	format string
}

// stampPlaceholders caches immutable placeholder strings across width queries.
// A query must neither recompile each message nor touch an observer's state.
var stampPlaceholders sync.Map

// expressionPlaceholder provides width/alignment without observing a fake zero.
func expressionPlaceholder(size int, format string) (string, bool) {
	if !strings.HasPrefix(format, "printf(") {
		return "", false
	}
	key := placeholderKey{size, format}
	if cached, ok := stampPlaceholders.Load(key); ok {
		return cached.(string), true
	}
	program, err := printf.ParseStamp(format, size*8, true)
	if err != nil {
		return "", true
	} // Startup validation reports the full error.
	text := program.Placeholder("-")
	stampPlaceholders.Store(key, text)
	return text, true
}

// newStampState owns the compiled functions for this source and tracking session.
func newStampState(diagnostics io.Writer) (targetStampState, error) {
	programs, err := compileStampExpressions()
	return targetStampState{programs: programs, diagnostics: diagnostics}, err
}

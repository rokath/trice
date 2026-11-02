// SPDX-License-Identifier: MIT

package id

import (
	"fmt"
	"strings"
	"sync"
)

// aliasTagRules is the frozen interpretation of persisted alias names. Order
// resolves equal-length matches; exclusions suppress contained occurrences only.
var aliasTagRules = []struct{ tag, positives, exclusions string }{
	{"FATAL", "fatal", "nonfatal non_fatal"},
	{"CRITICAL", "critical crit", "criterion criteria critic critique noncritical non_critical criticalsection critical_section criticalregion critical_region criticalpath critical_path"},
	{"ERROR", "error err", "interrupt override overridden overriding ferrite ferromagnet ferric ferrous ferrule terrain territory terrestrial berry cherry"},
	{"WARNING", "warning warn wrn", "nowarn no_warn nowarning no_warning"},
	{"INFO", "informal info inf", "infrared infrastructure infer inflect infix infimum infinite infinity inflate inflation inflating inflow influx influence infill infect"},
	{"DEBUG", "debug dbg deb db", "readback readbinary readbit readbyte readblock readbuffer feedback seedbed broadband deadband fieldbus fieldbuffer modbus redblack codebase codeblock codebook modebit debit debt debounce deblock debian loadbalance loadbalancing"},
	{"TRACE", "trace tr", "abstract attribute constrain contract contrast control electric electrode electron entries entry entropy extract instrument intrinsic matrix matrices metric retrieve retrieval stream strength stress stride string strip strobe strong struct track trap tray treat tree trend tremor transact transceiver transfer transform transient transistor transit translate translation translator transmission transmit transparent transparency transpose transposition transport transducer trial triangle triangular trigger trim trip tristate tri_state true truncate truncation try ultrasonic ultrasound ultraviolet"},
	{"VERBOSE", "verbose", "nonverbose non_verbose"},
}

// aliasNameResults caches only frozen name semantics, never command overrides
// or registry-dependent fallback. It therefore remains valid across TIL reloads.
var aliasNameResults sync.Map

// aliasNameTag visits overlapping literal occurrences so a separate surviving
// match is not lost when another part of the identifier contains an exclusion.
func aliasNameTag(name string) (tag, reason string) {
	if cached, ok := aliasNameResults.Load(name); ok {
		result := cached.([2]string)
		return result[0], result[1]
	}
	lower := strings.ToLower(name)
	type span struct {
		start, end int
		word       string
	}
	var excluded []span
	for _, rule := range aliasTagRules {
		for _, word := range strings.Fields(rule.exclusions) {
			for offset := 0; offset < len(lower); {
				index := strings.Index(lower[offset:], word)
				if index < 0 {
					break
				}
				start := offset + index
				excluded = append(excluded, span{start, start + len(word), word})
				offset = start + 1
			}
		}
	}
	best, selected := 0, ""
	var suppressed []string
	tie := false
	for _, rule := range aliasTagRules {
		for _, word := range strings.Fields(rule.positives) {
			for offset := 0; offset < len(lower); {
				index := strings.Index(lower[offset:], word)
				if index < 0 {
					break
				}
				start := offset + index
				offset = start + 1
				blocked := false
				for _, exclusion := range excluded {
					if exclusion.start <= start && start+len(word) <= exclusion.end {
						suppressed = append(suppressed, word+" inside "+exclusion.word)
						blocked = true
						break
					}
				}
				if blocked {
					continue
				}
				if len(word) > best {
					best, tag, selected, tie = len(word), rule.tag, word, false
				} else if len(word) == best && tag != rule.tag {
					tie = true
				}
			}
		}
	}
	reason = fmt.Sprintf("longest name match %q", selected)
	if tag == "" {
		reason = "no surviving name match"
	}
	if tie {
		reason += "; equal-length tie resolved by fixed tag priority"
	}
	if len(suppressed) > 0 {
		reason += "; excluded: " + strings.Join(suppressed, ", ")
	}
	aliasNameResults.Store(name, [2]string{tag, reason})
	return
}

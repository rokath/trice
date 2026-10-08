// SPDX-License-Identifier: MIT

#include <stdint.h>
#include <stdio.h>
#include "trice.h"
#include "trice_main_c_K1D52CCE8F1C3388A.h" // trice-bind: keep as last include before this file's Trice calls

// triceConfig.h uses these as independent 16-bit and 32-bit stamp sources.
// The build script also selects pc_sample_phase as the added CE cycle field.
uint16_t pc_sample_phase;
uint32_t pc_sample_milliseconds;
static FILE* binary_log; // Open while the synchronous output callback runs.
static int write_failed; // Remember errors because the callback returns void.

// The direct auxiliary writer keeps the demo self-contained: each Trice record
// is framed by the target library and saved exactly as the host decoder expects.
static void write_binary_log(const uint8_t* data, size_t length) {
	if (write_failed || fwrite(data, 1u, length, binary_log) != length) {
		write_failed = 1;
	}
}

// emit_sample demonstrates fields, ordinary conversions, and an opt-in CE
// selector. The CE rule reads pc_sample_phase at this exact call site.
static void emit_sample(unsigned int voltage_mv) {
	TRice32("info:ctx:Supply {voltage_mv:%u} mV\n", voltage_mv);
}

int main(void) {
	// Change these values and rerun build_and_run.sh, then a show_*.sh script.
	const char runtime_label[] = "pump A";
	const uint8_t reply[] = {0x41u, 0x00u, 0xffu};
	binary_log = fopen("capture.bin", "wb");
	if (binary_log == NULL) {
		perror("capture.bin");
		return 1;
	}
	// Attach our file writer to the library's direct-output callback.
	// Direct mode calls it inside each log call; no TriceTransfer is needed.
	UserNonBlockingDirectWrite8AuxiliaryFn = write_binary_log;
	TriceInit();
	// Named fields keep readable text and also expose values in JSON/KV output.
	trice("info:PC feature tour starts\n");
	TriceS("info:Device {device:%s}\n", runtime_label);
	pc_sample_phase = 7u;
	pc_sample_milliseconds = 100u;
	// The case of Trice/TRice selects the stamp width; 16 here is value width.
	// Thus TRice16 carries a 32-bit stamp and a 16-bit phase value.
	TRice16("info:Phase {phase:%u}\n", pc_sample_phase);
	emit_sample(3300u);
	trice8("wrn:Retry {attempt:%u}\n", 2u);
	// sensor is an application tag; the show scripts assign its severity weight.
	Trice8("sensor:Humidity {humidity_pct:%u} percent\n", 55u);
	pc_sample_phase = 11u;
	pc_sample_milliseconds = 125u;
	// A second sample makes stamp deltas and the changing CE cycle visible.
	emit_sample(3250u);
	trice32("dbg:Raw register=%08x\n", 0x2au);
	// Untagged text remains unchanged; JSON/KV marks its tag as untagged.
	trice("A message without a tag\n");
	// Buffer logs repeat one conversion per byte; named buffer fields are unsupported.
	TRICE8_B("rx:%02x ", reply, sizeof(reply));
	if (fclose(binary_log) != 0 || write_failed) {
		fprintf(stderr, "Could not finish capture.bin\n");
		return 1;
	}
	return 0;
}

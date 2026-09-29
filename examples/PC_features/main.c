// SPDX-License-Identifier: MIT

#include <stdint.h>
#include <stdio.h>
#include "trice.h"
#include "trice_main_c_K1D52CCE8F1C3388A.h" // trice-bind: keep as last include before this file's Trice calls

uint16_t pc_sample_phase;
uint32_t pc_sample_milliseconds;
static FILE* binary_log;
static int write_failed;

// The direct auxiliary writer keeps the demo self-contained: each Trice record
// is framed by the target library and saved exactly as the host decoder expects.
void TriceNonBlockingDirectWrite8Auxiliary(const uint8_t* data, size_t length) {
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
	const char runtime_label[] = "pump A";
	const uint8_t reply[] = {0x41u, 0x00u, 0xffu};
	binary_log = fopen("capture.bin", "wb");
	if (binary_log == NULL) {
		perror("capture.bin");
		return 1;
	}
	TriceInit();
	trice("info:PC feature tour starts\n");
	triceS("info:Device {device:%s}\n", runtime_label);
	pc_sample_phase = 7u;
	pc_sample_milliseconds = 100u;
	Trice16("info:Phase {phase:%u}\n", pc_sample_phase);
	emit_sample(3300u);
	trice8("wrn:Retry {attempt:%u}\n", 2u);
	trice8("sensor:Humidity {humidity_pct:%u} percent\n", 55u);
	pc_sample_phase = 11u;
	pc_sample_milliseconds = 125u;
	emit_sample(3250u);
	trice32("dbg:Raw register=%08x\n", 0x2au);
	trice("A message without a tag\n");
	TRICE8_B("rx:%02x ", reply, sizeof(reply));
	if (fclose(binary_log) != 0 || write_failed) {
		fprintf(stderr, "Could not finish capture.bin\n");
		return 1;
	}
	return 0;
}

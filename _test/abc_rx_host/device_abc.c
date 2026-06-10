//! \file device_abc.c
//! Trice generated ABC code - do not edit!

#include "device_abc.h"
#include "triceAbcReceive.h"

const triceAbc_t triceAbc[] = {
	/* Trice type */  /*   id, bitWidth, function pointer */
	/* TRICe_C    */ {  1001u,   0u, rx_no_payload },
	/* TRICE8_C   */ {  1006u,   8u, rx_i8_bulk },
	/* TRICE16_C  */ {  1002u,  16u, rx_i16 },
	/* TRIce32_C  */ {  1003u,  32u, rx_i32 },
	/* Trice64C   */ {  1004u,  64u, rx_i64 },
	/* TRiceC     */ {  1005u,   0u, rx_nested },
};

const unsigned triceAbcElements = sizeof(triceAbc) / sizeof(triceAbc[0]);

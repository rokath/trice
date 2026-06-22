//! \file nodeAbc.c
//! Trice generated ABC code - do not edit!

#include "nodeAbc.h"
#include "triceRx.h"

const triceAbc_t triceAbc[] = {
	/* Trice type */  /*   id, bitWidth, function pointer */
	/* triceC     */ {  1716u,   0u, logState },
	/* triceC     */ {  3578u,   0u, logState },
	/* trice8C    */ {  3792u,   8u, setKey },
	/* trice8C    */ {  3925u,   8u, setKey },
	/* triceC     */ {  4208u,   0u, logState },
	/* trice32C   */ {  4499u,  32u, divide },
	/* trice32C   */ {  4557u,  32u, DivideResult },
	/* triceC     */ {  4818u,   0u, getLeds },
	/* trice8C    */ {  5150u,   8u, setLeds },
	/* trice8C    */ {  6278u,   8u, setLeds },
	/* trice8C    */ {  6890u,   8u, setKey },
	/* trice8C    */ {  7145u,   8u, LedsState },
	/* triceC     */ {  7171u,   0u, getLeds },
	/* triceC     */ {  7313u,   0u, getLeds },
	/* trice8C    */ {  7431u,   8u, setLeds },
};

const unsigned triceAbcElements = sizeof(triceAbc) / sizeof(triceAbc[0]);

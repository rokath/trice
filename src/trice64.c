//! \file trice64.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! ///////////////////////////////////////////////////////////////////////////
#include "trice.h"

void trice64_1_fn( uint64_t tid, uint64_t v0 ){
    trice64_1_m( tid, v0 );
}

void trice64_2_fn( uint64_t tid,  uint64_t v0, uint64_t v1 ){
    trice64_2_m( tid, v0, v1 );
}

void trice64_3_fn( uint64_t tid,  uint64_t v0, uint64_t v1, uint64_t v2 ){
    trice64_3_m( tid, v0, v1, v2 );
}

void trice64_4_fn( uint64_t tid,  uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3 ){
    trice64_4_m( tid, v0, v1, v2, v3 );
}

void trice64_5_fn( uint64_t tid,  uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4 ){
    trice64_5_m( tid, v0, v1, v2, v3, v4 );
}

void trice64_6_fn( uint64_t tid,  uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5 ){
    trice64_6_m( tid, v0, v1, v2, v3, v4, v5 );
}

void trice64_7_fn( uint64_t tid,  uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6 ){
    trice64_7_m( tid, v0, v1, v2, v3, v4, v5, v6 );
}

void trice64_8_fn( uint64_t tid,  uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7 ){
    trice64_8_m( tid, v0, v1, v2, v3, v4, v5, v6, v7 );
}

void trice64_9_fn( uint64_t tid,  uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8 ){
    trice64_9_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8 );
}

void trice64_10_fn( uint64_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9 ){
    trice64_10_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 );
}

void trice64_11_fn( uint64_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9, uint64_t v10 ){
    trice64_11_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 );
}

void trice64_12_fn( uint64_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9, uint64_t v10, uint64_t v11 ){
    trice64_12_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 );
}

void Trice64_1_fn( uint64_t tid, uint64_t v0 ){
    Trice64_1_m( tid, v0 );
}

void Trice64_2_fn( uint64_t tid,  uint64_t v0, uint64_t v1 ){
    Trice64_2_m( tid, v0, v1 );
}

void Trice64_3_fn( uint64_t tid,  uint64_t v0, uint64_t v1, uint64_t v2 ){
    Trice64_3_m( tid, v0, v1, v2 );
}

void Trice64_4_fn( uint64_t tid,  uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3 ){
    Trice64_4_m( tid, v0, v1, v2, v3 );
}

void Trice64_5_fn( uint64_t tid,  uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4 ){
    Trice64_5_m( tid, v0, v1, v2, v3, v4 );
}

void Trice64_6_fn( uint64_t tid,  uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5 ){
    Trice64_6_m( tid, v0, v1, v2, v3, v4, v5 );
}

void Trice64_7_fn( uint64_t tid,  uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6 ){
    Trice64_7_m( tid, v0, v1, v2, v3, v4, v5, v6 );
}

void Trice64_8_fn( uint64_t tid,  uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7 ){
    Trice64_8_m( tid, v0, v1, v2, v3, v4, v5, v6, v7 );
}

void Trice64_9_fn( uint64_t tid,  uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8 ){
    Trice64_9_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8 );
}

void Trice64_10_fn( uint64_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9 ){
    Trice64_10_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 );
}

void Trice64_11_fn( uint64_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9, uint64_t v10 ){
    Trice64_11_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 );
}

void Trice64_12_fn( uint64_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9, uint64_t v10, uint64_t v11 ){
    Trice64_12_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 );
}


void TRice64_1_fn( uint64_t tid, uint64_t v0 ){
    TRice64_1_m( tid, v0 );
}

void TRice64_2_fn( uint64_t tid,  uint64_t v0, uint64_t v1 ){
    TRice64_2_m( tid, v0, v1 );
}

void TRice64_3_fn( uint64_t tid,  uint64_t v0, uint64_t v1, uint64_t v2 ){
    TRice64_3_m( tid, v0, v1, v2 );
}

void TRice64_4_fn( uint64_t tid,  uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3 ){
    TRice64_4_m( tid, v0, v1, v2, v3 );
}

void TRice64_5_fn( uint64_t tid,  uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4 ){
    TRice64_5_m( tid, v0, v1, v2, v3, v4 );
}

void TRice64_6_fn( uint64_t tid,  uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5 ){
    TRice64_6_m( tid, v0, v1, v2, v3, v4, v5 );
}

void TRice64_7_fn( uint64_t tid,  uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6 ){
    TRice64_7_m( tid, v0, v1, v2, v3, v4, v5, v6 );
}

void TRice64_8_fn( uint64_t tid,  uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7 ){
    TRice64_8_m( tid, v0, v1, v2, v3, v4, v5, v6, v7 );
}

void TRice64_9_fn( uint64_t tid,  uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8 ){
    TRice64_9_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8 );
}

void TRice64_10_fn( uint64_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9 ){
    TRice64_10_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 );
}

void TRice64_11_fn( uint64_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9, uint64_t v10 ){
    TRice64_11_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 );
}

void TRice64_12_fn( uint64_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9, uint64_t v10, uint64_t v11 ){
    TRice64_12_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 );
}

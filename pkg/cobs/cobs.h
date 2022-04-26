//! \file cobs.h
//! \author Thomas.Hoehenleitner [at] seerose.net


#ifndef COBS_H_
#define COBS_H_

#include <stddef.h>
#include <stdint.h>

size_t COBSEncode(void * restrict out, const void * restrict in, size_t length);
size_t COBSDecode(void * restrict out, const void * restrict in, size_t length);

#endif

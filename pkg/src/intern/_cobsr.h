/*****************************************************************************
 *
 * cobsr.h
 *
 * Consistent Overhead Byte Stuffing--Reduced (COBS/R)
 *
 ****************************************************************************/

#ifndef COBSR_H_
#define COBSR_H_


/*****************************************************************************
 * Includes
 ****************************************************************************/

#include <stdint.h>
#include <stdlib.h>


/*****************************************************************************
 * Defines
 ****************************************************************************/

#define COBSR_ENCODE_DST_BUF_LEN_MAX(SRC_LEN)           ((SRC_LEN) + (((SRC_LEN) + 253u)/254u))
#define COBSR_DECODE_DST_BUF_LEN_MAX(SRC_LEN)           (SRC_LEN)


/*****************************************************************************
 * Typedefs
 ****************************************************************************/

typedef enum
{
    COBSR_ENCODE_OK                     = 0x00,
    COBSR_ENCODE_NULL_POINTER           = 0x01,
    COBSR_ENCODE_OUT_BUFFER_OVERFLOW    = 0x02
} cobsr_encode_status;

typedef struct
{
    size_t              out_len;
    cobsr_encode_status status;
} cobsr_encode_result;


typedef enum
{
    COBSR_DECODE_OK                     = 0x00,
    COBSR_DECODE_NULL_POINTER           = 0x01,
    COBSR_DECODE_OUT_BUFFER_OVERFLOW    = 0x02,
    COBSR_DECODE_ZERO_BYTE_IN_INPUT     = 0x04,
} cobsr_decode_status;

typedef struct
{
    size_t              out_len;
    cobsr_decode_status status;
} cobsr_decode_result;


/*****************************************************************************
 * Function prototypes
 ****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

cobsr_encode_result cobsr_encode(uint8_t *dst_buf_ptr, size_t dst_buf_len, const uint8_t * src_ptr, size_t src_len);
cobsr_decode_result cobsr_decode(uint8_t *dst_buf_ptr, size_t dst_buf_len, const uint8_t * src_ptr, size_t src_len);


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* COBSR_H_ */

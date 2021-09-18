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

/* COBS/R-encode a string of input bytes, which may save one byte of output.
 *
 * dst_buf_ptr:    The buffer into which the result will be written
 * dst_buf_len:    Length of the buffer into which the result will be written
 * src_ptr:        The byte string to be encoded
 * src_len         Length of the byte string to be encoded
 *
 * returns:        A struct containing the success status of the encoding
 *                 operation and the length of the result (that was written to
 *                 dst_buf_ptr)
 */
cobsr_encode_result cobsr_encode(void * dst_buf_ptr, size_t dst_buf_len,
                                 const void * src_ptr, size_t src_len);

/* Decode a COBS/R byte string.
 *
 * dst_buf_ptr:    The buffer into which the result will be written
 * dst_buf_len:    Length of the buffer into which the result will be written
 * src_ptr:        The byte string to be decoded
 * src_len         Length of the byte string to be decoded
 *
 * returns:        A struct containing the success status of the decoding
 *                 operation and the length of the result (that was written to
 *                 dst_buf_ptr)
 */
cobsr_decode_result cobsr_decode(void * dst_buf_ptr, size_t dst_buf_len,
                                 const void * src_ptr, size_t src_len);


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* COBSR_H_ */

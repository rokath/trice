"""Consistent Overhead Byte Stuffing (COBS)

Python high-level wrapper for C functions, using ctypes.
This creates the same Python API as the `cobs` module
at:
    http://pypi.python.org/pypi/cobs

So, the unit tests that are included in that module
can be re-used nearly verbatim.

This is written for Python 2.x.
"""

import ctypes


# Load COBS DLL
try:
    # Windows
    cobs_dll = ctypes.cdll.libcobs
except OSError:
    # Linux
    cobs_dll = ctypes.cdll.LoadLibrary('./libcobs.so')


# Set up ctypes function for COBS encode
class _CobsEncodeResult(ctypes.Structure):
    _fields_ = [("out_len", ctypes.c_size_t),
                ("status", ctypes.c_int)]


class CobsEncodeStatus(object):
    OK                  = 0x00
    NULL_POINTER        = 0x01
    OUT_BUFFER_OVERFLOW = 0x02


encode_cfunc = cobs_dll.cobs_encode
encode_cfunc.restype = _CobsEncodeResult


# Set up ctypes function for COBS decode
class _CobsDecodeResult(ctypes.Structure):
    _fields_ = [("out_len", ctypes.c_size_t),
                ("status", ctypes.c_int)]


class CobsDecodeStatus(object):
    OK                  = 0x00
    NULL_POINTER        = 0x01
    OUT_BUFFER_OVERFLOW = 0x02
    ZERO_BYTE_IN_INPUT  = 0x04
    INPUT_TOO_SHORT     = 0x08


decode_cfunc = cobs_dll.cobs_decode
decode_cfunc.restype = _CobsDecodeResult


# Python helper functions for maximum encode/decode output length
def encode_size_max(in_bytes_len):
    return in_bytes_len + in_bytes_len // 254 + 1


def decode_size_max(in_bytes_len):
    return max(1, in_bytes_len - 1)


##############################################################################
#
# Python high-level interface that mimics the API of the Python `cobs` module
#
##############################################################################

class EncodeError(Exception):
    # This class doesn't exist in the Python module, since the only reason it
    # can be raised is output buffer overflow. Output buffer overflow is
    # possible in the C code but not in the Python code.
    pass


class DecodeError(Exception):
    pass


# Python wrapper function for COBS encode
def encode(in_bytes):
    out_buffer_len = encode_size_max(len(in_bytes))
    out_buffer = ctypes.create_string_buffer(out_buffer_len)

    ret_val = encode_cfunc(out_buffer, len(out_buffer), in_bytes, len(in_bytes))

    try:
        if ret_val.status & CobsEncodeStatus.OUT_BUFFER_OVERFLOW:
            raise EncodeError("output buffer overflow")
        elif ret_val.status != CobsEncodeStatus.OK:
            raise EncodeError("unknown error")
    except EncodeError as e:
#        e.output = out_buffer[:ret_val.out_len]
        raise e

    return out_buffer[:ret_val.out_len]


# Python wrapper function for COBS decode
def decode(in_bytes):
    out_buffer_len = decode_size_max(len(in_bytes))
    out_buffer = ctypes.create_string_buffer(out_buffer_len)

    ret_val = decode_cfunc(out_buffer, len(out_buffer), in_bytes, len(in_bytes))
    
    if ret_val.status & CobsDecodeStatus.OUT_BUFFER_OVERFLOW:
        raise DecodeError("output buffer overflow")
    elif ret_val.status & CobsDecodeStatus.ZERO_BYTE_IN_INPUT:
        raise DecodeError("zero byte found in input")
    elif ret_val.status & CobsDecodeStatus.INPUT_TOO_SHORT:
        raise DecodeError("not enough input bytes for length code")
    elif ret_val.status != CobsDecodeStatus.OK:
        raise DecodeError("unknown error")

    return out_buffer[:ret_val.out_len]


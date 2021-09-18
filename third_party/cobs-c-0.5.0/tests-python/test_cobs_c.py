"""
Consistent Overhead Byte Stuffing (COBS)

Unit Tests specific to the C implementation details.
In particular, test output buffer overflow detection.

This version is for Python 2.x.
"""

from array import array
import ctypes
import sys
import unittest

import cobs_wrapper


class CSpecificTests(unittest.TestCase):
    predefined_encodings = [
        [ "",                                       "\x01"                                                          ],
        [ "1",                                      "\x021"                                                         ],
        [ "12345",                                  "\x0612345"                                                     ],
        [ "12345\x006789",                          "\x0612345\x056789"                                             ],
        [ "\x0012345\x006789",                      "\x01\x0612345\x056789"                                         ],
        [ "12345\x006789\x00",                      "\x0612345\x056789\x01"                                         ],
        [ "\x00",                                   "\x01\x01"                                                      ],
        [ "\x00\x00",                               "\x01\x01\x01"                                                  ],
        [ "\x00\x00\x00",                           "\x01\x01\x01\x01"                                              ],
        [ array('B', range(1, 254)).tostring(),     "\xfe" + array('B', range(1, 254)).tostring()                   ],
        [ array('B', range(1, 255)).tostring(),     "\xff" + array('B', range(1, 255)).tostring()                   ],
        [ array('B', range(1, 256)).tostring(),     "\xff" + array('B', range(1, 255)).tostring() + "\x02\xff"      ],
        [ array('B', range(0, 256)).tostring(),     "\x01\xff" + array('B', range(1, 255)).tostring() + "\x02\xff"  ],
    ]

    def test_encode_null_pointer(self):
        for (test_string, _expected_encoded_string) in self.predefined_encodings:
            out_buffer_len = cobs_wrapper.encode_size_max(len(test_string))
            out_buffer = ctypes.create_string_buffer(out_buffer_len)

            # First, check that output buffer NULL pointer generates error.
            ret_val = cobs_wrapper.encode_cfunc(None, out_buffer_len, test_string, len(test_string))
            self.assertTrue(ret_val.status & cobs_wrapper.CobsEncodeStatus.NULL_POINTER)
            self.assertEqual(ret_val.out_len, 0)

            # Second, check that input buffer NULL pointer generates error.
            ret_val = cobs_wrapper.encode_cfunc(out_buffer, out_buffer_len, None, len(test_string))
            self.assertTrue(ret_val.status & cobs_wrapper.CobsEncodeStatus.NULL_POINTER)
            self.assertEqual(ret_val.out_len, 0)

    def test_decode_null_pointer(self):
        for (_expected_decoded_string, encoded_string) in self.predefined_encodings:
            out_buffer_len = cobs_wrapper.decode_size_max(len(encoded_string))
            out_buffer = ctypes.create_string_buffer(out_buffer_len)

            # First, check that output buffer NULL pointer generates error.
            ret_val = cobs_wrapper.decode_cfunc(None, out_buffer_len, encoded_string, len(encoded_string))
            self.assertTrue(ret_val.status & cobs_wrapper.CobsDecodeStatus.NULL_POINTER)
            self.assertEqual(ret_val.out_len, 0)

            # Second, check that input buffer NULL pointer generates error.
            ret_val = cobs_wrapper.decode_cfunc(out_buffer, out_buffer_len, None, len(encoded_string))
            self.assertTrue(ret_val.status & cobs_wrapper.CobsDecodeStatus.NULL_POINTER)
            self.assertEqual(ret_val.out_len, 0)

    def test_encode_output_overflow(self):
        for (test_string, expected_encoded_string) in self.predefined_encodings:
            try:
                # Sanity check that the expected encode string length is not
                # bigger than the computed maximum.
                self.assertTrue(len(expected_encoded_string) <= cobs_wrapper.encode_size_max(len(test_string)))

                real_out_buffer_len = cobs_wrapper.encode_size_max(len(test_string)) + 100

                for out_buffer_len in xrange(0, real_out_buffer_len + 1):

                    out_buffer = ctypes.create_string_buffer('\xAA' * real_out_buffer_len, real_out_buffer_len)

                    ret_val = cobs_wrapper.encode_cfunc(out_buffer, out_buffer_len, test_string, len(test_string))
                    actual_encoded = out_buffer[:ret_val.out_len]

                    # Check that the output length is never larger than the output buffer size
                    self.assertTrue(ret_val.out_len <= out_buffer_len)

                    # Check that the function never writes bytes past the end of the buffer
                    self.assertEqual(out_buffer[out_buffer_len:], '\xAA' * (real_out_buffer_len - out_buffer_len))

                    # Check that the function never writes byte past where is claims to have written
                    # (not strictly a requirement, but a nice-to-have if possible)
                    self.assertEqual(out_buffer[ret_val.out_len:], '\xAA' * (real_out_buffer_len - ret_val.out_len))

                    if out_buffer_len < len(expected_encoded_string):
                        # Check that the output buffer overflow error status is flagged
                        self.assertTrue((ret_val.status & cobs_wrapper.CobsEncodeStatus.OUT_BUFFER_OVERFLOW) != 0)

                        # Check that the output buffer is filled up to the limit
                        # (not strictly a requirement, but a nice-to-have)
                        self.assertEqual(ret_val.out_len, out_buffer_len)

                        # Check that the data that _is_ put in the output buffer is valid
                        # (not strictly a requirement, but a nice-to-have if possible)
                        actual_decoded = cobs_wrapper.decode(actual_encoded)
                        self.assertTrue(test_string.startswith(actual_decoded),
                                        "for %s, encode buffer length %d, got %s" % (repr(test_string), out_buffer_len, repr(actual_decoded)))

                    if out_buffer_len >= len(expected_encoded_string):
                        # Check that the output buffer overflow error status is NOT flagged
                        self.assertTrue((ret_val.status & cobs_wrapper.CobsEncodeStatus.OUT_BUFFER_OVERFLOW) == 0)

                    if (ret_val.status & cobs_wrapper.CobsEncodeStatus.OUT_BUFFER_OVERFLOW) == 0:
                        # Check that the correct encoded value is returned
                        self.assertEqual(actual_encoded, expected_encoded_string)

            except AssertionError:
                print >> sys.stderr, "For test string %s" % repr(test_string)
                raise

    def test_decode_output_overflow(self):
        for (expected_decoded_string, encoded_string) in self.predefined_encodings:
            try:
                # Sanity check that the expected decode string length is not
                # bigger than the computed maximum.
                self.assertTrue(len(expected_decoded_string) <= cobs_wrapper.decode_size_max(len(encoded_string)))

                real_out_buffer_len = cobs_wrapper.decode_size_max(len(encoded_string)) + 100

                for out_buffer_len in xrange(0, real_out_buffer_len + 1):

                    out_buffer = ctypes.create_string_buffer('\xAA' * real_out_buffer_len, real_out_buffer_len)

                    ret_val = cobs_wrapper.decode_cfunc(out_buffer, out_buffer_len, encoded_string, len(encoded_string))
                    actual_decoded = out_buffer[:ret_val.out_len]

                    # Check that the output length is never larger than the output buffer size
                    self.assertTrue(ret_val.out_len <= out_buffer_len)

                    # Check that the function never writes bytes past the end of the buffer
                    self.assertEqual(out_buffer[out_buffer_len:], '\xAA' * (real_out_buffer_len - out_buffer_len))

                    # Check that the function never writes byte past where is claims to have written
                    # (not strictly a requirement, but a nice-to-have if possible)
                    self.assertEqual(out_buffer[ret_val.out_len:], '\xAA' * (real_out_buffer_len - ret_val.out_len))

                    if out_buffer_len < len(expected_decoded_string):
                        # Check that the output buffer overflow error status is flagged
                        self.assertTrue((ret_val.status & cobs_wrapper.CobsDecodeStatus.OUT_BUFFER_OVERFLOW) != 0)

                        # Check that the output buffer is filled up to the limit
                        # (not strictly a requirement, but a nice-to-have if possible)
                        self.assertEqual(ret_val.out_len, out_buffer_len)

                        # Check that the data that _is_ put in the output buffer is valid
                        # (not strictly a requirement, but a nice-to-have if possible)
                        self.assertTrue(expected_decoded_string.startswith(actual_decoded),
                                        "for %s, decode buffer length %d, got %s" % (repr(expected_decoded_string), out_buffer_len, repr(actual_decoded)))

                    if out_buffer_len >= len(expected_decoded_string):
                        # Check that the output buffer overflow error status is NOT flagged
                        self.assertTrue((ret_val.status & cobs_wrapper.CobsDecodeStatus.OUT_BUFFER_OVERFLOW) == 0)

                    if (ret_val.status & cobs_wrapper.CobsDecodeStatus.OUT_BUFFER_OVERFLOW) == 0:
                        # Check that the correct encoded value is returned
                        self.assertEqual(out_buffer[:ret_val.out_len], expected_decoded_string)

            except AssertionError:
                print >> sys.stderr, "For test string %s" % repr(expected_decoded_string)
                raise

    def test_decode_single_buffer(self):
        for (expected_decoded_string, encoded_string) in self.predefined_encodings:
            try:
                real_single_buffer_len = cobs_wrapper.decode_size_max(len(encoded_string)) + 100

                single_buffer_len = len(encoded_string)

                single_buffer = ctypes.create_string_buffer('\xAA' * real_single_buffer_len, real_single_buffer_len)
                single_buffer.raw = encoded_string

                ret_val = cobs_wrapper.decode_cfunc(single_buffer, len(encoded_string), single_buffer, len(encoded_string))
                actual_decoded = single_buffer[:ret_val.out_len]

                # Check that the output length is never larger than the output buffer size
                self.assertTrue(ret_val.out_len <= single_buffer_len)

                # Check that the function never writes bytes past the end of the buffer
                self.assertEqual(single_buffer[single_buffer_len:], '\xAA' * (real_single_buffer_len - single_buffer_len))

                # Check that the function never writes byte past the original input length
                # (not strictly a requirement, but a nice-to-have if possible)
                self.assertEqual(single_buffer[len(encoded_string):], '\xAA' * (real_single_buffer_len - len(encoded_string)))

                # Check that the output buffer overflow error status is NOT flagged
                self.assertTrue((ret_val.status & cobs_wrapper.CobsDecodeStatus.OUT_BUFFER_OVERFLOW) == 0)

                # Check that the correct encoded value is returned
                self.assertEqual(single_buffer[:ret_val.out_len], expected_decoded_string)

            except AssertionError:
                print >> sys.stderr, "For test string %s" % repr(expected_decoded_string)
                raise


def runtests():
    unittest.main()


if __name__ == '__main__':
    runtests()

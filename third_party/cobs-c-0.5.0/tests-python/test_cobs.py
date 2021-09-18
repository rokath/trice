"""
Consistent Overhead Byte Stuffing (COBS)

Unit Tests

This version is for Python 2.x.
"""

from array import array
import random
import unittest

#from cobs import cobs
#from cobs.cobs import _cobs_py as cobs
import cobs_wrapper as cobs


def infinite_non_zero_generator():
    while True:
        for i in xrange(1,50):
            for j in xrange(1,256, i):
                yield j

def non_zero_generator(length):
    non_zeros = infinite_non_zero_generator()
    for i in xrange(length):
        yield non_zeros.next()

def non_zero_bytes(length):
    return ''.join(chr(i) for i in non_zero_generator(length))


class PredefinedEncodingsTests(unittest.TestCase):
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

    def test_predefined_encodings(self):
        for (test_string, expected_encoded_string) in self.predefined_encodings:
            encoded = cobs.encode(test_string)
            self.assertEqual(encoded, expected_encoded_string)

    def test_decode_predefined_encodings(self):
        for (test_string, expected_encoded_string) in self.predefined_encodings:
            decoded = cobs.decode(expected_encoded_string)
            self.assertEqual(test_string, decoded)


class PredefinedDecodeErrorTests(unittest.TestCase):
    decode_error_test_strings = [
        "\x00",
        "\x05123",
        "\x051234\x00",
        "\x0512\x004",
    ]

    def test_predefined_decode_error(self):
        for test_encoded in self.decode_error_test_strings:
            self.assertRaises(cobs.DecodeError, cobs.decode, test_encoded)


class ZerosTest(unittest.TestCase):
    def test_zeros(self):
        for length in xrange(520):
            test_string = '\x00' * length
            encoded = cobs.encode(test_string)
            expected_encoded = '\x01' * (length + 1)
            self.assertEqual(encoded, expected_encoded, "encoding zeros failed for length %d" % length)
            decoded = cobs.decode(encoded)
            self.assertEqual(decoded, test_string, "decoding zeros failed for length %d" % length)


class NonZerosTest(unittest.TestCase):
    def simple_encode_non_zeros_only(self, in_bytes):
        out_list = []
        for i in xrange(0, len(in_bytes), 254):
            data_block = in_bytes[i: i+254]
            out_list.append(chr(len(data_block) + 1))
            out_list.append(data_block)
        return ''.join(out_list)

    def test_non_zeros(self):
        for length in xrange(1, 1000):
            test_string = non_zero_bytes(length)
            encoded = cobs.encode(test_string)
            expected_encoded = self.simple_encode_non_zeros_only(test_string)
            self.assertEqual(encoded, expected_encoded,
                             "encoded != expected_encoded for length %d\nencoded: %s\nexpected_encoded: %s" %
                             (length, repr(encoded), repr(expected_encoded)))

    def test_non_zeros_and_trailing_zero(self):
        for length in xrange(1, 1000):
            non_zeros_string = non_zero_bytes(length)
            test_string = non_zeros_string + '\x00'
            encoded = cobs.encode(test_string)
            if (len(non_zeros_string) % 254) == 0:
                expected_encoded = self.simple_encode_non_zeros_only(non_zeros_string) + '\x01\x01'
            else:
                expected_encoded = self.simple_encode_non_zeros_only(non_zeros_string) + '\x01'
            self.assertEqual(encoded, expected_encoded,
                             "encoded != expected_encoded for length %d\nencoded: %s\nexpected_encoded: %s" %
                             (length, repr(encoded), repr(expected_encoded)))


class RandomDataTest(unittest.TestCase):
    NUM_TESTS = 5000
    MAX_LENGTH = 2000

    def test_random(self):
        try:
            for _test_num in xrange(self.NUM_TESTS):
                length = random.randint(0, self.MAX_LENGTH)
                test_string = ''.join(chr(random.randint(0,255)) for x in xrange(length))
                encoded = cobs.encode(test_string)
                self.assertTrue('\x00' not in encoded,
                                "encoding contains zero byte(s):\noriginal: %s\nencoded: %s" % (repr(test_string), repr(encoded)))
                self.assertTrue(len(encoded) <= len(test_string) + 1 + (len(test_string) // 254),
                                "encoding too big:\noriginal: %s\nencoded: %s" % (repr(test_string), repr(encoded)))
                decoded = cobs.decode(encoded)
                self.assertEqual(decoded, test_string,
                                 "encoding and decoding random data failed:\noriginal: %s\ndecoded: %s" % (repr(test_string), repr(decoded)))
        except KeyboardInterrupt:
            pass


def runtests():
    unittest.main()


if __name__ == '__main__':
    runtests()

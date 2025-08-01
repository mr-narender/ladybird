/*
 * Copyright (c) 2022, stelar7 <dudedbz@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/ByteBuffer.h>
#include <LibCrypto/Curves/EdwardsCurve.h>
#include <LibTest/TestCase.h>

// https://datatracker.ietf.org/doc/html/rfc8032#section-7.1
TEST_CASE(TEST_1)
{
    u8 private_bytes[32] = {
        0x9d, 0x61, 0xb1, 0x9d, 0xef, 0xfd, 0x5a, 0x60,
        0xba, 0x84, 0x4a, 0xf4, 0x92, 0xec, 0x2c, 0xc4,
        0x44, 0x49, 0xc5, 0x69, 0x7b, 0x32, 0x69, 0x19,
        0x70, 0x3b, 0xac, 0x03, 0x1c, 0xae, 0x7f, 0x60
    };

    u8 public_bytes[32] = {
        0xd7, 0x5a, 0x98, 0x01, 0x82, 0xb1, 0x0a, 0xb7,
        0xd5, 0x4b, 0xfe, 0xd3, 0xc9, 0x64, 0x07, 0x3a,
        0x0e, 0xe1, 0x72, 0xf3, 0xda, 0xa6, 0x23, 0x25,
        0xaf, 0x02, 0x1a, 0x68, 0xf7, 0x07, 0x51, 0x1a
    };

    u8 message_bytes[0] = {};

    u8 signature_bytes[64] = {
        0xe5, 0x56, 0x43, 0x00, 0xc3, 0x60, 0xac, 0x72,
        0x90, 0x86, 0xe2, 0xcc, 0x80, 0x6e, 0x82, 0x8a,
        0x84, 0x87, 0x7f, 0x1e, 0xb8, 0xe5, 0xd9, 0x74,
        0xd8, 0x73, 0xe0, 0x65, 0x22, 0x49, 0x01, 0x55,
        0x5f, 0xb8, 0x82, 0x15, 0x90, 0xa3, 0x3b, 0xac,
        0xc6, 0x1e, 0x39, 0x70, 0x1c, 0xf9, 0xb4, 0x6b,
        0xd2, 0x5b, 0xf5, 0xf0, 0x59, 0x5b, 0xbe, 0x24,
        0x65, 0x51, 0x41, 0x43, 0x8e, 0x7a, 0x10, 0x0b
    };

    ReadonlyBytes private_key { private_bytes, 32 };
    ReadonlyBytes public_key { public_bytes, 32 };
    ReadonlyBytes message { message_bytes, 0 };
    ReadonlyBytes expected_signature { signature_bytes, 64 };

    Crypto::Curves::Ed25519 curve;

    auto generated_signature = TRY_OR_FAIL(curve.sign(private_key, message));
    EXPECT_EQ(generated_signature, expected_signature);
    EXPECT(TRY_OR_FAIL(curve.verify(public_key, expected_signature, message)));
}

// https://datatracker.ietf.org/doc/html/rfc8032#section-7.1
TEST_CASE(TEST_2)
{
    u8 private_bytes[32] = {
        0x4c, 0xcd, 0x08, 0x9b, 0x28, 0xff, 0x96, 0xda,
        0x9d, 0xb6, 0xc3, 0x46, 0xec, 0x11, 0x4e, 0x0f,
        0x5b, 0x8a, 0x31, 0x9f, 0x35, 0xab, 0xa6, 0x24,
        0xda, 0x8c, 0xf6, 0xed, 0x4f, 0xb8, 0xa6, 0xfb
    };

    u8 public_bytes[32] = {
        0x3d, 0x40, 0x17, 0xc3, 0xe8, 0x43, 0x89, 0x5a,
        0x92, 0xb7, 0x0a, 0xa7, 0x4d, 0x1b, 0x7e, 0xbc,
        0x9c, 0x98, 0x2c, 0xcf, 0x2e, 0xc4, 0x96, 0x8c,
        0xc0, 0xcd, 0x55, 0xf1, 0x2a, 0xf4, 0x66, 0x0c
    };

    u8 message_bytes[1] = { 0x72 };

    u8 signature_bytes[64] = {
        0x92, 0xa0, 0x09, 0xa9, 0xf0, 0xd4, 0xca, 0xb8,
        0x72, 0x0e, 0x82, 0x0b, 0x5f, 0x64, 0x25, 0x40,
        0xa2, 0xb2, 0x7b, 0x54, 0x16, 0x50, 0x3f, 0x8f,
        0xb3, 0x76, 0x22, 0x23, 0xeb, 0xdb, 0x69, 0xda,
        0x08, 0x5a, 0xc1, 0xe4, 0x3e, 0x15, 0x99, 0x6e,
        0x45, 0x8f, 0x36, 0x13, 0xd0, 0xf1, 0x1d, 0x8c,
        0x38, 0x7b, 0x2e, 0xae, 0xb4, 0x30, 0x2a, 0xee,
        0xb0, 0x0d, 0x29, 0x16, 0x12, 0xbb, 0x0c, 0x00
    };

    ReadonlyBytes private_key { private_bytes, 32 };
    ReadonlyBytes public_key { public_bytes, 32 };
    ReadonlyBytes message { message_bytes, 1 };
    ReadonlyBytes expected_signature { signature_bytes, 64 };

    Crypto::Curves::Ed25519 curve;

    auto generated_signature = TRY_OR_FAIL(curve.sign(private_key, message));
    EXPECT_EQ(generated_signature, expected_signature);
    EXPECT(TRY_OR_FAIL(curve.verify(public_key, expected_signature, message)));
}

// https://datatracker.ietf.org/doc/html/rfc8032#section-7.1
TEST_CASE(TEST_3)
{
    u8 private_bytes[32] = {
        0xc5, 0xaa, 0x8d, 0xf4, 0x3f, 0x9f, 0x83, 0x7b,
        0xed, 0xb7, 0x44, 0x2f, 0x31, 0xdc, 0xb7, 0xb1,
        0x66, 0xd3, 0x85, 0x35, 0x07, 0x6f, 0x09, 0x4b,
        0x85, 0xce, 0x3a, 0x2e, 0x0b, 0x44, 0x58, 0xf7
    };

    u8 public_bytes[32] = {
        0xfc, 0x51, 0xcd, 0x8e, 0x62, 0x18, 0xa1, 0xa3,
        0x8d, 0xa4, 0x7e, 0xd0, 0x02, 0x30, 0xf0, 0x58,
        0x08, 0x16, 0xed, 0x13, 0xba, 0x33, 0x03, 0xac,
        0x5d, 0xeb, 0x91, 0x15, 0x48, 0x90, 0x80, 0x25
    };

    u8 message_bytes[2] = { 0xaf, 0x82 };

    u8 signature_bytes[64] = {
        0x62, 0x91, 0xd6, 0x57, 0xde, 0xec, 0x24, 0x02,
        0x48, 0x27, 0xe6, 0x9c, 0x3a, 0xbe, 0x01, 0xa3,
        0x0c, 0xe5, 0x48, 0xa2, 0x84, 0x74, 0x3a, 0x44,
        0x5e, 0x36, 0x80, 0xd7, 0xdb, 0x5a, 0xc3, 0xac,
        0x18, 0xff, 0x9b, 0x53, 0x8d, 0x16, 0xf2, 0x90,
        0xae, 0x67, 0xf7, 0x60, 0x98, 0x4d, 0xc6, 0x59,
        0x4a, 0x7c, 0x15, 0xe9, 0x71, 0x6e, 0xd2, 0x8d,
        0xc0, 0x27, 0xbe, 0xce, 0xea, 0x1e, 0xc4, 0x0a
    };

    ReadonlyBytes private_key { private_bytes, 32 };
    ReadonlyBytes public_key { public_bytes, 32 };
    ReadonlyBytes message { message_bytes, 2 };
    ReadonlyBytes expected_signature { signature_bytes, 64 };

    Crypto::Curves::Ed25519 curve;

    auto generated_signature = TRY_OR_FAIL(curve.sign(private_key, message));
    EXPECT_EQ(generated_signature, expected_signature);
    EXPECT(TRY_OR_FAIL(curve.verify(public_key, expected_signature, message)));
}

// https://datatracker.ietf.org/doc/html/rfc8032#section-7.1
TEST_CASE(TEST_1024)
{
    u8 private_bytes[32] = {
        0xf5, 0xe5, 0x76, 0x7c, 0xf1, 0x53, 0x31, 0x95,
        0x17, 0x63, 0x0f, 0x22, 0x68, 0x76, 0xb8, 0x6c,
        0x81, 0x60, 0xcc, 0x58, 0x3b, 0xc0, 0x13, 0x74,
        0x4c, 0x6b, 0xf2, 0x55, 0xf5, 0xcc, 0x0e, 0xe5
    };

    u8 public_bytes[32] = {
        0x27, 0x81, 0x17, 0xfc, 0x14, 0x4c, 0x72, 0x34,
        0x0f, 0x67, 0xd0, 0xf2, 0x31, 0x6e, 0x83, 0x86,
        0xce, 0xff, 0xbf, 0x2b, 0x24, 0x28, 0xc9, 0xc5,
        0x1f, 0xef, 0x7c, 0x59, 0x7f, 0x1d, 0x42, 0x6e
    };

    u8 message_bytes[1023] = {
        0x08, 0xb8, 0xb2, 0xb7, 0x33, 0x42, 0x42, 0x43,
        0x76, 0x0f, 0xe4, 0x26, 0xa4, 0xb5, 0x49, 0x08,
        0x63, 0x21, 0x10, 0xa6, 0x6c, 0x2f, 0x65, 0x91,
        0xea, 0xbd, 0x33, 0x45, 0xe3, 0xe4, 0xeb, 0x98,
        0xfa, 0x6e, 0x26, 0x4b, 0xf0, 0x9e, 0xfe, 0x12,
        0xee, 0x50, 0xf8, 0xf5, 0x4e, 0x9f, 0x77, 0xb1,
        0xe3, 0x55, 0xf6, 0xc5, 0x05, 0x44, 0xe2, 0x3f,
        0xb1, 0x43, 0x3d, 0xdf, 0x73, 0xbe, 0x84, 0xd8,
        0x79, 0xde, 0x7c, 0x00, 0x46, 0xdc, 0x49, 0x96,
        0xd9, 0xe7, 0x73, 0xf4, 0xbc, 0x9e, 0xfe, 0x57,
        0x38, 0x82, 0x9a, 0xdb, 0x26, 0xc8, 0x1b, 0x37,
        0xc9, 0x3a, 0x1b, 0x27, 0x0b, 0x20, 0x32, 0x9d,
        0x65, 0x86, 0x75, 0xfc, 0x6e, 0xa5, 0x34, 0xe0,
        0x81, 0x0a, 0x44, 0x32, 0x82, 0x6b, 0xf5, 0x8c,
        0x94, 0x1e, 0xfb, 0x65, 0xd5, 0x7a, 0x33, 0x8b,
        0xbd, 0x2e, 0x26, 0x64, 0x0f, 0x89, 0xff, 0xbc,
        0x1a, 0x85, 0x8e, 0xfc, 0xb8, 0x55, 0x0e, 0xe3,
        0xa5, 0xe1, 0x99, 0x8b, 0xd1, 0x77, 0xe9, 0x3a,
        0x73, 0x63, 0xc3, 0x44, 0xfe, 0x6b, 0x19, 0x9e,
        0xe5, 0xd0, 0x2e, 0x82, 0xd5, 0x22, 0xc4, 0xfe,
        0xba, 0x15, 0x45, 0x2f, 0x80, 0x28, 0x8a, 0x82,
        0x1a, 0x57, 0x91, 0x16, 0xec, 0x6d, 0xad, 0x2b,
        0x3b, 0x31, 0x0d, 0xa9, 0x03, 0x40, 0x1a, 0xa6,
        0x21, 0x00, 0xab, 0x5d, 0x1a, 0x36, 0x55, 0x3e,
        0x06, 0x20, 0x3b, 0x33, 0x89, 0x0c, 0xc9, 0xb8,
        0x32, 0xf7, 0x9e, 0xf8, 0x05, 0x60, 0xcc, 0xb9,
        0xa3, 0x9c, 0xe7, 0x67, 0x96, 0x7e, 0xd6, 0x28,
        0xc6, 0xad, 0x57, 0x3c, 0xb1, 0x16, 0xdb, 0xef,
        0xef, 0xd7, 0x54, 0x99, 0xda, 0x96, 0xbd, 0x68,
        0xa8, 0xa9, 0x7b, 0x92, 0x8a, 0x8b, 0xbc, 0x10,
        0x3b, 0x66, 0x21, 0xfc, 0xde, 0x2b, 0xec, 0xa1,
        0x23, 0x1d, 0x20, 0x6b, 0xe6, 0xcd, 0x9e, 0xc7,
        0xaf, 0xf6, 0xf6, 0xc9, 0x4f, 0xcd, 0x72, 0x04,
        0xed, 0x34, 0x55, 0xc6, 0x8c, 0x83, 0xf4, 0xa4,
        0x1d, 0xa4, 0xaf, 0x2b, 0x74, 0xef, 0x5c, 0x53,
        0xf1, 0xd8, 0xac, 0x70, 0xbd, 0xcb, 0x7e, 0xd1,
        0x85, 0xce, 0x81, 0xbd, 0x84, 0x35, 0x9d, 0x44,
        0x25, 0x4d, 0x95, 0x62, 0x9e, 0x98, 0x55, 0xa9,
        0x4a, 0x7c, 0x19, 0x58, 0xd1, 0xf8, 0xad, 0xa5,
        0xd0, 0x53, 0x2e, 0xd8, 0xa5, 0xaa, 0x3f, 0xb2,
        0xd1, 0x7b, 0xa7, 0x0e, 0xb6, 0x24, 0x8e, 0x59,
        0x4e, 0x1a, 0x22, 0x97, 0xac, 0xbb, 0xb3, 0x9d,
        0x50, 0x2f, 0x1a, 0x8c, 0x6e, 0xb6, 0xf1, 0xce,
        0x22, 0xb3, 0xde, 0x1a, 0x1f, 0x40, 0xcc, 0x24,
        0x55, 0x41, 0x19, 0xa8, 0x31, 0xa9, 0xaa, 0xd6,
        0x07, 0x9c, 0xad, 0x88, 0x42, 0x5d, 0xe6, 0xbd,
        0xe1, 0xa9, 0x18, 0x7e, 0xbb, 0x60, 0x92, 0xcf,
        0x67, 0xbf, 0x2b, 0x13, 0xfd, 0x65, 0xf2, 0x70,
        0x88, 0xd7, 0x8b, 0x7e, 0x88, 0x3c, 0x87, 0x59,
        0xd2, 0xc4, 0xf5, 0xc6, 0x5a, 0xdb, 0x75, 0x53,
        0x87, 0x8a, 0xd5, 0x75, 0xf9, 0xfa, 0xd8, 0x78,
        0xe8, 0x0a, 0x0c, 0x9b, 0xa6, 0x3b, 0xcb, 0xcc,
        0x27, 0x32, 0xe6, 0x94, 0x85, 0xbb, 0xc9, 0xc9,
        0x0b, 0xfb, 0xd6, 0x24, 0x81, 0xd9, 0x08, 0x9b,
        0xec, 0xcf, 0x80, 0xcf, 0xe2, 0xdf, 0x16, 0xa2,
        0xcf, 0x65, 0xbd, 0x92, 0xdd, 0x59, 0x7b, 0x07,
        0x07, 0xe0, 0x91, 0x7a, 0xf4, 0x8b, 0xbb, 0x75,
        0xfe, 0xd4, 0x13, 0xd2, 0x38, 0xf5, 0x55, 0x5a,
        0x7a, 0x56, 0x9d, 0x80, 0xc3, 0x41, 0x4a, 0x8d,
        0x08, 0x59, 0xdc, 0x65, 0xa4, 0x61, 0x28, 0xba,
        0xb2, 0x7a, 0xf8, 0x7a, 0x71, 0x31, 0x4f, 0x31,
        0x8c, 0x78, 0x2b, 0x23, 0xeb, 0xfe, 0x80, 0x8b,
        0x82, 0xb0, 0xce, 0x26, 0x40, 0x1d, 0x2e, 0x22,
        0xf0, 0x4d, 0x83, 0xd1, 0x25, 0x5d, 0xc5, 0x1a,
        0xdd, 0xd3, 0xb7, 0x5a, 0x2b, 0x1a, 0xe0, 0x78,
        0x45, 0x04, 0xdf, 0x54, 0x3a, 0xf8, 0x96, 0x9b,
        0xe3, 0xea, 0x70, 0x82, 0xff, 0x7f, 0xc9, 0x88,
        0x8c, 0x14, 0x4d, 0xa2, 0xaf, 0x58, 0x42, 0x9e,
        0xc9, 0x60, 0x31, 0xdb, 0xca, 0xd3, 0xda, 0xd9,
        0xaf, 0x0d, 0xcb, 0xaa, 0xaf, 0x26, 0x8c, 0xb8,
        0xfc, 0xff, 0xea, 0xd9, 0x4f, 0x3c, 0x7c, 0xa4,
        0x95, 0xe0, 0x56, 0xa9, 0xb4, 0x7a, 0xcd, 0xb7,
        0x51, 0xfb, 0x73, 0xe6, 0x66, 0xc6, 0xc6, 0x55,
        0xad, 0xe8, 0x29, 0x72, 0x97, 0xd0, 0x7a, 0xd1,
        0xba, 0x5e, 0x43, 0xf1, 0xbc, 0xa3, 0x23, 0x01,
        0x65, 0x13, 0x39, 0xe2, 0x29, 0x04, 0xcc, 0x8c,
        0x42, 0xf5, 0x8c, 0x30, 0xc0, 0x4a, 0xaf, 0xdb,
        0x03, 0x8d, 0xda, 0x08, 0x47, 0xdd, 0x98, 0x8d,
        0xcd, 0xa6, 0xf3, 0xbf, 0xd1, 0x5c, 0x4b, 0x4c,
        0x45, 0x25, 0x00, 0x4a, 0xa0, 0x6e, 0xef, 0xf8,
        0xca, 0x61, 0x78, 0x3a, 0xac, 0xec, 0x57, 0xfb,
        0x3d, 0x1f, 0x92, 0xb0, 0xfe, 0x2f, 0xd1, 0xa8,
        0x5f, 0x67, 0x24, 0x51, 0x7b, 0x65, 0xe6, 0x14,
        0xad, 0x68, 0x08, 0xd6, 0xf6, 0xee, 0x34, 0xdf,
        0xf7, 0x31, 0x0f, 0xdc, 0x82, 0xae, 0xbf, 0xd9,
        0x04, 0xb0, 0x1e, 0x1d, 0xc5, 0x4b, 0x29, 0x27,
        0x09, 0x4b, 0x2d, 0xb6, 0x8d, 0x6f, 0x90, 0x3b,
        0x68, 0x40, 0x1a, 0xde, 0xbf, 0x5a, 0x7e, 0x08,
        0xd7, 0x8f, 0xf4, 0xef, 0x5d, 0x63, 0x65, 0x3a,
        0x65, 0x04, 0x0c, 0xf9, 0xbf, 0xd4, 0xac, 0xa7,
        0x98, 0x4a, 0x74, 0xd3, 0x71, 0x45, 0x98, 0x67,
        0x80, 0xfc, 0x0b, 0x16, 0xac, 0x45, 0x16, 0x49,
        0xde, 0x61, 0x88, 0xa7, 0xdb, 0xdf, 0x19, 0x1f,
        0x64, 0xb5, 0xfc, 0x5e, 0x2a, 0xb4, 0x7b, 0x57,
        0xf7, 0xf7, 0x27, 0x6c, 0xd4, 0x19, 0xc1, 0x7a,
        0x3c, 0xa8, 0xe1, 0xb9, 0x39, 0xae, 0x49, 0xe4,
        0x88, 0xac, 0xba, 0x6b, 0x96, 0x56, 0x10, 0xb5,
        0x48, 0x01, 0x09, 0xc8, 0xb1, 0x7b, 0x80, 0xe1,
        0xb7, 0xb7, 0x50, 0xdf, 0xc7, 0x59, 0x8d, 0x5d,
        0x50, 0x11, 0xfd, 0x2d, 0xcc, 0x56, 0x00, 0xa3,
        0x2e, 0xf5, 0xb5, 0x2a, 0x1e, 0xcc, 0x82, 0x0e,
        0x30, 0x8a, 0xa3, 0x42, 0x72, 0x1a, 0xac, 0x09,
        0x43, 0xbf, 0x66, 0x86, 0xb6, 0x4b, 0x25, 0x79,
        0x37, 0x65, 0x04, 0xcc, 0xc4, 0x93, 0xd9, 0x7e,
        0x6a, 0xed, 0x3f, 0xb0, 0xf9, 0xcd, 0x71, 0xa4,
        0x3d, 0xd4, 0x97, 0xf0, 0x1f, 0x17, 0xc0, 0xe2,
        0xcb, 0x37, 0x97, 0xaa, 0x2a, 0x2f, 0x25, 0x66,
        0x56, 0x16, 0x8e, 0x6c, 0x49, 0x6a, 0xfc, 0x5f,
        0xb9, 0x32, 0x46, 0xf6, 0xb1, 0x11, 0x63, 0x98,
        0xa3, 0x46, 0xf1, 0xa6, 0x41, 0xf3, 0xb0, 0x41,
        0xe9, 0x89, 0xf7, 0x91, 0x4f, 0x90, 0xcc, 0x2c,
        0x7f, 0xff, 0x35, 0x78, 0x76, 0xe5, 0x06, 0xb5,
        0x0d, 0x33, 0x4b, 0xa7, 0x7c, 0x22, 0x5b, 0xc3,
        0x07, 0xba, 0x53, 0x71, 0x52, 0xf3, 0xf1, 0x61,
        0x0e, 0x4e, 0xaf, 0xe5, 0x95, 0xf6, 0xd9, 0xd9,
        0x0d, 0x11, 0xfa, 0xa9, 0x33, 0xa1, 0x5e, 0xf1,
        0x36, 0x95, 0x46, 0x86, 0x8a, 0x7f, 0x3a, 0x45,
        0xa9, 0x67, 0x68, 0xd4, 0x0f, 0xd9, 0xd0, 0x34,
        0x12, 0xc0, 0x91, 0xc6, 0x31, 0x5c, 0xf4, 0xfd,
        0xe7, 0xcb, 0x68, 0x60, 0x69, 0x37, 0x38, 0x0d,
        0xb2, 0xea, 0xaa, 0x70, 0x7b, 0x4c, 0x41, 0x85,
        0xc3, 0x2e, 0xdd, 0xcd, 0xd3, 0x06, 0x70, 0x5e,
        0x4d, 0xc1, 0xff, 0xc8, 0x72, 0xee, 0xee, 0x47,
        0x5a, 0x64, 0xdf, 0xac, 0x86, 0xab, 0xa4, 0x1c,
        0x06, 0x18, 0x98, 0x3f, 0x87, 0x41, 0xc5, 0xef,
        0x68, 0xd3, 0xa1, 0x01, 0xe8, 0xa3, 0xb8, 0xca,
        0xc6, 0x0c, 0x90, 0x5c, 0x15, 0xfc, 0x91, 0x08,
        0x40, 0xb9, 0x4c, 0x00, 0xa0, 0xb9, 0xd0
    };

    u8 signature_bytes[64] = {
        0x0a, 0xab, 0x4c, 0x90, 0x05, 0x01, 0xb3, 0xe2,
        0x4d, 0x7c, 0xdf, 0x46, 0x63, 0x32, 0x6a, 0x3a,
        0x87, 0xdf, 0x5e, 0x48, 0x43, 0xb2, 0xcb, 0xdb,
        0x67, 0xcb, 0xf6, 0xe4, 0x60, 0xfe, 0xc3, 0x50,
        0xaa, 0x53, 0x71, 0xb1, 0x50, 0x8f, 0x9f, 0x45,
        0x28, 0xec, 0xea, 0x23, 0xc4, 0x36, 0xd9, 0x4b,
        0x5e, 0x8f, 0xcd, 0x4f, 0x68, 0x1e, 0x30, 0xa6,
        0xac, 0x00, 0xa9, 0x70, 0x4a, 0x18, 0x8a, 0x03
    };

    ReadonlyBytes private_key { private_bytes, 32 };
    ReadonlyBytes public_key { public_bytes, 32 };
    ReadonlyBytes message { message_bytes, 1023 };
    ReadonlyBytes expected_signature { signature_bytes, 64 };

    Crypto::Curves::Ed25519 curve;

    auto generated_signature = TRY_OR_FAIL(curve.sign(private_key, message));
    EXPECT_EQ(generated_signature, expected_signature);
    EXPECT(TRY_OR_FAIL(curve.verify(public_key, expected_signature, message)));
}

// https://datatracker.ietf.org/doc/html/rfc8032#section-7.1
TEST_CASE(TEST_SHA_ABC)
{
    u8 private_bytes[32] = {
        0x83, 0x3f, 0xe6, 0x24, 0x09, 0x23, 0x7b, 0x9d,
        0x62, 0xec, 0x77, 0x58, 0x75, 0x20, 0x91, 0x1e,
        0x9a, 0x75, 0x9c, 0xec, 0x1d, 0x19, 0x75, 0x5b,
        0x7d, 0xa9, 0x01, 0xb9, 0x6d, 0xca, 0x3d, 0x42
    };

    u8 public_bytes[32] = {
        0xec, 0x17, 0x2b, 0x93, 0xad, 0x5e, 0x56, 0x3b,
        0xf4, 0x93, 0x2c, 0x70, 0xe1, 0x24, 0x50, 0x34,
        0xc3, 0x54, 0x67, 0xef, 0x2e, 0xfd, 0x4d, 0x64,
        0xeb, 0xf8, 0x19, 0x68, 0x34, 0x67, 0xe2, 0xbf
    };

    u8 message_bytes[64] = {
        0xdd, 0xaf, 0x35, 0xa1, 0x93, 0x61, 0x7a, 0xba,
        0xcc, 0x41, 0x73, 0x49, 0xae, 0x20, 0x41, 0x31,
        0x12, 0xe6, 0xfa, 0x4e, 0x89, 0xa9, 0x7e, 0xa2,
        0x0a, 0x9e, 0xee, 0xe6, 0x4b, 0x55, 0xd3, 0x9a,
        0x21, 0x92, 0x99, 0x2a, 0x27, 0x4f, 0xc1, 0xa8,
        0x36, 0xba, 0x3c, 0x23, 0xa3, 0xfe, 0xeb, 0xbd,
        0x45, 0x4d, 0x44, 0x23, 0x64, 0x3c, 0xe8, 0x0e,
        0x2a, 0x9a, 0xc9, 0x4f, 0xa5, 0x4c, 0xa4, 0x9f
    };

    u8 signature_bytes[64] = {
        0xdc, 0x2a, 0x44, 0x59, 0xe7, 0x36, 0x96, 0x33,
        0xa5, 0x2b, 0x1b, 0xf2, 0x77, 0x83, 0x9a, 0x00,
        0x20, 0x10, 0x09, 0xa3, 0xef, 0xbf, 0x3e, 0xcb,
        0x69, 0xbe, 0xa2, 0x18, 0x6c, 0x26, 0xb5, 0x89,
        0x09, 0x35, 0x1f, 0xc9, 0xac, 0x90, 0xb3, 0xec,
        0xfd, 0xfb, 0xc7, 0xc6, 0x64, 0x31, 0xe0, 0x30,
        0x3d, 0xca, 0x17, 0x9c, 0x13, 0x8a, 0xc1, 0x7a,
        0xd9, 0xbe, 0xf1, 0x17, 0x73, 0x31, 0xa7, 0x04
    };

    ReadonlyBytes private_key { private_bytes, 32 };
    ReadonlyBytes public_key { public_bytes, 32 };
    ReadonlyBytes message { message_bytes, 64 };
    ReadonlyBytes expected_signature { signature_bytes, 64 };

    Crypto::Curves::Ed25519 curve;

    auto generated_signature = TRY_OR_FAIL(curve.sign(private_key, message));
    EXPECT_EQ(generated_signature, expected_signature);
    EXPECT(TRY_OR_FAIL(curve.verify(public_key, expected_signature, message)));
}

// Test case for small-order point rejection
TEST_CASE(small_order_point_rejection)
{
    Crypto::Curves::Ed25519 curve;

    // Test message
    u8 message_bytes[5] = { 0x68, 0x65, 0x6c, 0x6c, 0x6f }; // "hello"
    ReadonlyBytes message { message_bytes, 5 };

    // Identity point (order 1) - should be rejected
    u8 identity_point[32] = {
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    // Point of order 2 - should be rejected
    u8 order_2_point[32] = {
        0xec, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f
    };

    // Point of order 4 - should be rejected
    u8 order_4_point[32] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80
    };

    // Point of order 8 - should be rejected
    u8 order_8_point[32] = {
        0xee, 0xd3, 0xf5, 0x5c, 0x1a, 0x63, 0x12, 0x58, 0xd6, 0x9c, 0xf7, 0xa2, 0xde, 0xf9, 0xde, 0x14,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10
    };

    // WPT test small-order point (pubKeys[0]) - should be rejected
    u8 wpt_small_order_point[32] = {
        0xc7, 0x17, 0x6a, 0x70, 0x3d, 0x4d, 0xd8, 0x4f, 0xba, 0x3c, 0x0b, 0x76, 0x0d, 0x10, 0x67, 0x0f,
        0x2a, 0x20, 0x53, 0xfa, 0x2c, 0x39, 0xcc, 0xc6, 0x4e, 0xc7, 0xfd, 0x77, 0x92, 0xac, 0x03, 0xfa
    };

    // Dummy signature (64 bytes) - content doesn't matter since verification should fail at key validation
    u8 dummy_signature[64] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    ReadonlyBytes signature { dummy_signature, 64 };

    // All of these should return false (verification failure due to small-order point)
    auto result1 = TRY_OR_FAIL(curve.verify(ReadonlyBytes { identity_point, 32 }, signature, message));
    EXPECT(!result1);

    auto result2 = TRY_OR_FAIL(curve.verify(ReadonlyBytes { order_2_point, 32 }, signature, message));
    EXPECT(!result2);

    auto result3 = TRY_OR_FAIL(curve.verify(ReadonlyBytes { order_4_point, 32 }, signature, message));
    EXPECT(!result3);

    auto result4 = TRY_OR_FAIL(curve.verify(ReadonlyBytes { order_8_point, 32 }, signature, message));
    EXPECT(!result4);

    auto result5 = TRY_OR_FAIL(curve.verify(ReadonlyBytes { wpt_small_order_point, 32 }, signature, message));
    EXPECT(!result5);
}

// Test case for small-order R point in signature rejection
TEST_CASE(small_order_signature_r_point_rejection)
{
    Crypto::Curves::Ed25519 curve;

    // Test message
    u8 message_bytes[5] = { 0x68, 0x65, 0x6c, 0x6c, 0x6f }; // "hello"
    ReadonlyBytes message { message_bytes, 5 };

    // Valid public key (from TEST_1)
    u8 valid_public_key[32] = {
        0xd7, 0x5a, 0x98, 0x01, 0x82, 0xb1, 0x0a, 0xb7,
        0xd5, 0x4b, 0xfe, 0xd3, 0xc9, 0x64, 0x07, 0x3a,
        0x0e, 0xe1, 0x72, 0xf3, 0xda, 0xa6, 0x23, 0x25,
        0xaf, 0x02, 0x1a, 0x68, 0xf7, 0x07, 0x51, 0x1a
    };

    // Signature with small-order R point (first 32 bytes) - should be rejected
    u8 signature_with_small_order_r[64] = {
        // R point: identity point (order 1)
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        // S value (dummy)
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    ReadonlyBytes public_key { valid_public_key, 32 };
    ReadonlyBytes signature { signature_with_small_order_r, 64 };

    // This should return false (verification failure due to small-order R point in signature)
    auto result = TRY_OR_FAIL(curve.verify(public_key, signature, message));
    EXPECT(!result);
}

/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include <gtest/gtest.h>
#include <string>
#include <array>
#include "utils.hpp"

extern "C" {
#include <sr25519/sr25519.h>
}

TEST(VrfTest, Verify) {
  auto keypair = randomKeypair();
  std::array<uint8_t, SR25519_VRF_OUTPUT_SIZE + SR25519_VRF_PROOF_SIZE>
      out_and_proof;

  auto message = "Hello, world!"_v;
  auto limit = std::vector<uint8_t>(32, 0xFF);

  auto res1 =
      sr25519_vrf_sign_if_less(out_and_proof.data(), keypair.data(),
                               message.data(), message.size(), limit.data());
  ASSERT_EQ(res1.result, Sr25519SignatureResult::Ok);
  ASSERT_TRUE(res1.is_less);

  auto res2 = sr25519_vrf_verify(
      keypair.data() + 64, message.data(), message.size(), out_and_proof.data(),
      out_and_proof.data() + SR25519_VRF_OUTPUT_SIZE);
  ASSERT_EQ(res2, Sr25519SignatureResult::Ok);

  out_and_proof[5] += 3;
  auto res3 = sr25519_vrf_verify(
      keypair.data() + 64, message.data(), message.size(), out_and_proof.data(),
      out_and_proof.data() + SR25519_VRF_OUTPUT_SIZE);
  ASSERT_NE(res3, Sr25519SignatureResult::Ok);


}

TEST(VrfTest, ResultNotLess) {
  auto keypair =
      "e07a5da9575743a1993a9f2b9e39991c8d0c2ee4137347e450a39561f419fa7e84912092ffd5746b98a628c57eeafcfa60eb7e5c07438e00b3d0d528f42666813223ce3b7e32de1f876d5cbf44619dd5d1fa91d8c87b63af4a068f4fee24ad3a"_unhex;
  std::array<uint8_t, SR25519_VRF_OUTPUT_SIZE + SR25519_VRF_PROOF_SIZE>
      out_and_proof;

  auto message = "Hello, world!"_v;
  auto limit = std::vector<uint8_t>(32, 0xAA);

  auto res1 =
      sr25519_vrf_sign_if_less(out_and_proof.data(), keypair.data(),
                               message.data(), message.size(), limit.data());
  ASSERT_EQ(res1.result, Sr25519SignatureResult::Ok);
  ASSERT_FALSE(res1.is_less);
}

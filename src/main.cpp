// SPDX-License-Identifier: MIT
// Copyright (c) 2025-2026, Amit Gefen

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <print>

#include "byte_size_literals.hpp"

int main() {
  // NOLINTNEXTLINE(google-build-using-namespace)
  using namespace amitgdev::literals::bytes;

  // 1. Basic usage with C++23 std::print
  const uint32_t page_size = 4_kb;
  const uint64_t database_limit = 1_tb;

  // Fmt may throw
  try {
    std::println("--- Byte Size Literals Demo ---");
    std::println("Standard Page Size: {:>10} bytes", page_size);
    std::println("Database Limit:     {:>10} bytes\n", database_limit);

    // 2. Compile-time Verification
    // NOLINTBEGIN(google-avoid-magic-numbers)
    static_assert(static_cast<uint64_t>(1_kb) == 1024ULL);
    static_assert(static_cast<uint64_t>(1_mb) == 1024ULL * 1024);
    static_assert(static_cast<uint64_t>(1_gb) == 1024ULL * 1024 * 1024);
    // NOLINTEND(google-avoid-magic-numbers)

    std::println("Verification: All static_asserts passed at compile-time.\n");

    // 3. Safety Demonstration
    // 16_kb (16384) fits in uint16_t (max 65535).
    const uint16_t small_buffer = 16_kb;
    std::println("Safety: 16_kb successfully stored in uint16_t: {}",
                 small_buffer);
  } catch (const std::exception& e) {
    // fputs instead of std::println: the error path must not throw again.
    std::fputs(e.what(), stderr);
    std::fputs("\n", stderr);
    return EXIT_FAILURE;
  }

  /* PROVING THE SAFETY:
     The following line would cause the C7595 error you saw because
     64_kb (65536) is 1 byte larger than uint16_t's maximum value.
  */
  // const uint16_t overflow = 64_kb;

  return EXIT_SUCCESS;
}
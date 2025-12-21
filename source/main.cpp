#include <cstdint>
#include <print>

#include "byte_size_literals.hpp"

int main() {  // NOLINT(bugprone-exception-escape)
  // NOLINTNEXTLINE(google-build-using-namespace)
  using namespace amitgdev::literals::bytes;

  // 1. Basic usage with C++23 std::print
  const uint32_t page_size = 4_kb;
  const uint64_t database_limit = 1_tb;

  std::print("--- Byte Size Literals Demo ---\n");
  std::print("Standard Page Size: {:>10} bytes\n", page_size);
  std::print("Database Limit:     {:>10} bytes\n\n", database_limit);

  // 2. Compile-time Verification
  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
  static_assert(static_cast<uint64_t>(1_kb) == 1024ULL);
  static_assert(static_cast<uint64_t>(1_mb) == 1024ULL * 1024);
  static_assert(static_cast<uint64_t>(1_gb) == 1024ULL * 1024 * 1024);
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

  std::print("Verification: All static_asserts passed at compile-time.\n\n");

  // 3. Safety Demonstration
  // 16_kb (16384) fits in uint16_t (max 65535).
  const uint16_t small_buffer = 16_kb;
  std::print("Safety: 16_kb successfully stored in uint16_t: {}\n", small_buffer);

  /* PROVING THE SAFETY:
     The following line would cause the C7595 error you saw because
     64_kb (65536) is 1 byte larger than uint16_t's maximum value.
  */
  // const uint16_t overflow = 64_kb;

  return 0;
}
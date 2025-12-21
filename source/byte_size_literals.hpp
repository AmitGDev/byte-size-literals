// NOLINTBEGIN(llvm-header-guard)
#ifndef AMITG_BYTESIZELITERALS_HPP_
#define AMITG_BYTESIZELITERALS_HPP_
// NOLINTEND(llvm-header-guard)

/*
    byte_size_literals.hpp
    Copyright (c) 2025, Amit Gefen

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/

#include <cstdint>
#include <limits>
#include <stdexcept>
#include <type_traits>

namespace amitgdev::literals::bytes {

class CheckedByteSize {
 public:
  consteval explicit CheckedByteSize(const uint64_t byte_size) : byte_size_(byte_size) {}

  // Implicit conversion to integral types with compile time bounds checking
  template <typename T>
    requires std::is_integral_v<T>
  // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
  consteval operator T() const {
    if (byte_size_ > static_cast<uint64_t>(std::numeric_limits<T>::max())) {
      // (Compiler may omit this compile-time throw message)
      throw std::overflow_error("byte size value overflows type max");
    }
    return static_cast<T>(byte_size_);
  }

 private:
  uint64_t byte_size_;
};

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
consteval CheckedByteSize operator""_kb(unsigned long long kilobytes) {
  return CheckedByteSize(kilobytes * 1024ULL);
}

consteval CheckedByteSize operator""_mb(unsigned long long megabytes) {
  return CheckedByteSize(megabytes * static_cast<uint64_t>(1024_kb));
}

consteval CheckedByteSize operator""_gb(unsigned long long gigabytes) {
  return CheckedByteSize(gigabytes * static_cast<uint64_t>(1024_mb));
}

consteval CheckedByteSize operator""_tb(unsigned long long terabytes) {
  return CheckedByteSize(terabytes * static_cast<uint64_t>(1024_gb));
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

}  // namespace amitgdev::literals::bytes

#endif  // AMITG_BYTESIZELITERALS_HPP_
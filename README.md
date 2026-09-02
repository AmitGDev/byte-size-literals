# Byte Size Literals

Provides compile-time checked user-defined literals for byte size values in C++.

## Overview

Byte Size Literals is a modern C++ header-only library that enables intuitive byte size notation with compile-time safety. It replaces error-prone magic numbers like `4096` or `1073741824` with readable literals like `4_kb` or `1_gb`, while checking at compile time that the value fits the target integral type.

The library itself needs C++20 (`consteval`, `requires`). The demo in `main.cpp` uses C++23 (`std::println`).

## Features

- Readability: Express byte sizes naturally (`4_kb`, `512_mb`, `2_gb`) instead of calculating raw values
- Safety: Compile-time bounds checking prevents a value from exceeding the target type's limits
- Zero Runtime Cost: All conversions and checks happen at compile time with no performance overhead
- Type Flexibility: Implicit conversion to any integral type with automatic bounds verification
- Cross-Platform: Tested and verified on Windows and Linux systems
- Header-Only: Single header file, no compilation or linking required

## Usage

1. Include `byte_size_literals.hpp` in your project:

```cpp
#include "byte_size_literals.hpp"
```

2. Use the literals namespace:

```cpp
using namespace amitgdev::literals::bytes;
```

3. Express byte sizes naturally:

```cpp
const uint32_t buffer_size = 64_kb;
const uint64_t max_file_size = 5_gb;
```

The literals automatically convert to the target integral type with compile-time bounds checking.

## Example Usage

See `main.cpp` for a comprehensive example. It demonstrates basic usage, `static_assert` verification of the unit calculations, and a commented-out overflow case (`const uint16_t overflow = 64_kb;`) that fails to compile.

## Technical Features

### Modern C++ Features

- C++20 `consteval`: Literal operators, the `CheckedByteSize` constructor, and the conversion operator are all immediate functions, so every operation executes at compile time
- C++20 `requires` clause: `std::is_integral_v<T>` restricts the conversion operator to integral types
- User-defined literals: Natural syntax with `operator""_kb`, `operator""_mb`, `operator""_gb`, `operator""_tb` suffixes
- Implicit conversions: Seamless integration with existing code using a templated conversion operator
- Compile-time exceptions: `throw std::overflow_error` inside a `consteval` function makes the expression non-constant, turning a bounds violation into a compile error

### Safety and Validation

- Compile-Time Bounds Detection:
  - Automatic bounds checking: Compares the byte count against `std::numeric_limits<T>::max()` for the target type
  - Compiler error generation: Invalid conversions are rejected at compile time (e.g., MSVC reports C7595)
  - Type-specific validation: Each target type is verified independently (e.g., `64_kb` fails for `uint16_t` but succeeds for `uint32_t`)
  - Zero runtime cost: All checks are eliminated in the compiled binary

- Known Limitation:
  - The literal's own arithmetic is done in `uint64_t` and is not overflow-checked, so a very large literal (e.g., an `_tb` value above 2^24 TB) wraps silently before the target-type check runs

- Static Assertions (in the demo, `main.cpp`):
  - Verify that `1_kb`, `1_mb`, and `1_gb` equal 1024, 1024^2, and 1024^3
  - Confirm binary (1024-based) rather than decimal (1000-based) conversions

### API Design

- Available Literals:
  - `_kb`: Kilobytes (x 1024 bytes)
  - `_mb`: Megabytes (x 1024 KB)
  - `_gb`: Gigabytes (x 1024 MB)
  - `_tb`: Terabytes (x 1024 GB)

- CheckedByteSize Class:
  - Internal representation using `uint64_t` for maximum range
  - Explicit `consteval` constructor prevents accidental construction
  - Templated conversion operator handles all integral types
  - `consteval` enforcement ensures compile-time-only usage

- Binary Units:
  - Uses binary prefixes (powers of 1024) not decimal SI units
  - Aligns with industry standard for memory and storage calculations
  - Consistent with OS and compiler expectations

### Code Organization

- Header Guard:
  - Standard `#ifndef`/`#define`/`#endif` pattern following the Google C++ Style Guide naming (`AMITGDEV_BYTE_SIZE_LITERALS_HPP_`)

- Namespace Structure:
  - Primary namespace: `amitgdev::literals::bytes`
  - Clear separation from other literal types
  - Follows standard library convention for literal namespaces

- Type Safety:
  - `requires` clause restricts conversions to integral types only
  - Prevents misuse with floating-point or other non-integral types
  - Constraint-based errors are clearer than a failed template body

### Robustness Features

- Intermediate Arithmetic:
  - Each unit is built from the previous one (`_mb` from `1024_kb`, and so on) using `static_cast<uint64_t>()`, so the chain stays in 64 bits

- Linting Compliance:
  - `NOLINTBEGIN`/`NOLINTEND` around the literal operators suppresses `cppcoreguidelines-avoid-magic-numbers` (the numbers are the feature)
  - `NOLINTNEXTLINE(google-explicit-constructor)` on the conversion operator, since implicit conversion is intended

- Error Messages:
  - `std::overflow_error` with the text "byte size value overflows type max"
  - The compiler may omit the custom text and report only a non-constant expression error

### Performance Characteristics

- Compile-Time Evaluation:
  - All operations eliminated: No runtime code generated for conversions
  - Literal substitution: Compiler replaces usage with the calculated constant
  - Zero abstraction penalty: Identical performance to raw numeric literals
  - Optimizer-friendly: Constants enable advanced optimization passes

- Memory Efficiency:
  - No runtime storage for `CheckedByteSize` objects
  - Direct value embedding in compiled code
  - Minimal binary size impact (same as manual calculation)

## Requirements

A C++23-compatible compiler and standard library.

## Build and Run

- [Build on Windows](https://github.com/AmitGDev/cpp-ci-template#build-on-windows)
- [Build on Linux](https://github.com/AmitGDev/cpp-ci-template#build-on-linux)

## Code Style

- [C++](https://github.com/AmitGDev/cpp-coding-standard)
- [Python](https://github.com/AmitGDev/cpp-ci-template#python)
- [CMake](https://github.com/AmitGDev/cpp-ci-template#cmake)

## License

MIT License. Copyright © 2025-2026 Amit Gefen.

---

**Last Updated:** 2026-10-22  
**Maintainer:** AmitGDev

### Byte Size Literals

<br>

Provides compile-time checked user-defined literals for byte size values in C++.

**Author:** Amit Gefen

**License:** MIT License

<br>

**Overview**

Byte Size Literals is a modern C++20 header-only library that enables intuitive byte size notation with compile-time safety. It replaces error-prone magic numbers like `4096` or `1073741824` with readable literals like `4_kb` or `1_gb`, while providing automatic overflow detection at compile time.

<br>

**Features**

- Readability: Express byte sizes naturally (`4_kb`, `512_mb`, `2_gb`) instead of calculating raw values
- Safety: Compile-time overflow checking prevents values from exceeding target type limits
- Zero Runtime Cost: All conversions and checks happen at compile time with no performance overhead
- Type Flexibility: Implicit conversion to any integral type with automatic bounds verification
- Cross-Platform: Tested and verified on Windows and Linux systems
- Header-Only: Single header file, no compilation or linking required

<br>

**Usage**

\- Include byte_size_literals.hpp in your project:
```cpp
#include "byte_size_literals.hpp"
```

\- Use the literals namespace:

```cpp
using namespace amitgdev::literals::bytes;
```

\- Express byte sizes naturally:

```cpp
const uint32_t buffer_size = 64_kb;
const uint64_t max_file_size = 5_gb;
```

The literals automatically convert to the target integral type with compile-time overflow checking.

<br>

**Example Usage**

See the **main.cpp** file for a comprehensive example.

<br>

**Dependencies**

No external dependencies. Requires C++20 compiler support.

<br>
--
<br>

### Technical Features:

**Modern C++ Features**

- **C++20 `consteval` functions** - Guarantees all literal operations execute at compile time
- **C++20 Concepts** - `std::is_integral_v` requirement ensures type safety
- **User-defined literals** - Natural syntax with `operator""_kb`, `operator""_mb`, `operator""_gb`, `operator""_tb` suffixes
- **Implicit conversions** - Seamless integration with existing code using conversion operators
- **Compile-time exceptions** - `throw std::overflow_error` for bounds violations caught during compilation

**Safety & Validation**

- Compile-Time Overflow Detection:
  - **Automatic bounds checking** - Compares literal value against `std::numeric_limits<T>::max()` for target type
  - **Compiler error generation** - Invalid conversions prevented with C7595-style compilation errors
  - **Type-specific validation** - Each target type verified independently (e.g., `64_kb` fails for `uint16_t` but succeeds for `uint32_t`)
  - **Zero runtime cost** - All checks eliminated in compiled binary

- Static Assertions:
  - Compile-time verification ensures correct calculations
  - Validates power-of-two relationships between units
  - Confirms binary (1024-based) rather than decimal (1000-based) conversions

**API Design**

- Available Literals:
  - **`_kb`** - Kilobytes (× 1024 bytes)
  - **`_mb`** - Megabytes (× 1024 KB)
  - **`_gb`** - Gigabytes (× 1024 MB)
  - **`_tb`** - Terabytes (× 1024 GB)

- CheckedByteSize Class:
  - Internal representation using `uint64_t` for maximum range
  - Explicit constructor prevents accidental construction
  - Templated conversion operator handles all integral types
  - `consteval` enforcement ensures compile-time-only usage

- Binary Units:
  - Uses binary prefixes (powers of 1024) not decimal SI units
  - Aligns with industry standard for memory and storage calculations
  - Consistent with OS and compiler expectations

**Code Organization**

- Header Guard:
  - Custom NOLINT markers for project-specific header guard style
  - Prevents multiple inclusion with standard `#ifndef`/`#define`/`#endif` pattern

- Namespace Structure:
  - Primary namespace: `amitgdev::literals::bytes`
  - Clear separation from other literal types
  - Follows standard library convention for literal namespaces

- Type Safety:
  - `requires` clause restricts conversions to integral types only
  - Prevents misuse with floating-point or other non-integral types
  - Concept-based constraints for clear compiler errors

**Robustness Features**

- Overflow Prevention:
  - Maximum value tracking across unit conversions
  - Cascading multiplication with `static_cast<uint64_t>()` for intermediate literals
  - Protection against silent wraparound or truncation

- Linting Compliance:
  - Strategic NOLINT comments for intentional design decisions
  - Suppression of magic number warnings (literals are the feature, not a bug)
  - Explicit conversion warnings disabled for intended implicit behavior

- Error Messages:
  - Descriptive `std::overflow_error` message for compilation failures
  - Clear indication of which type constraint was violated
  - Compiler may display custom error text during compilation

**Performance Characteristics**

- Compile-Time Evaluation:
  - **All operations eliminated** - No runtime code generated for conversions
  - **Literal substitution** - Compiler replaces usage with calculated constant
  - **Zero abstraction penalty** - Identical performance to raw numeric literals
  - **Optimizer-friendly** - Constants enable advanced optimization passes

- Memory Efficiency:
  - No runtime storage for `CheckedByteSize` objects
  - Direct value embedding in compiled code
  - Minimal binary size impact (same as manual calculation)
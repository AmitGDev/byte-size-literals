Byte Size Literals is a dependency-free, header-only C++ library that provides
compile-time checked user-defined literals for binary byte sizes. The public
header is `src\byte_size_literals.hpp`, and `src\main.cpp` is a small executable
demonstration of the API.

The library exposes the `amitgdev::literals::bytes` namespace and the `_kb`,
`_mb`, `_gb`, and `_tb` literals. `CheckedByteSize` stores values as
`uint64_t` and provides `consteval` checked conversions to integral types.
Conversions that exceed the destination type's maximum are rejected during
compilation.

The README describes compiler support beginning with C++23, while the current
CMake target and demonstration use C++23 (`std::print`). Preserve the current
CMake/CI standard unless the project requirements are intentionally changed.
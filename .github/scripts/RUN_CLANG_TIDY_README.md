# Why `run-clang-tidy.py` is here

## 1. Windows package compatibility

We found that run-clang-tidy.py was missing from the LLVM 20 Windows package used by our CI. We therefore vendor a known-compatible copy to provide reliable parallel execution with -j on Windows runners. This keeps CI independent of the script’s presence and installation path in a particular LLVM package, preventing breakage if upstream packaging changes or the script is moved or renamed.

## 2. Why the flags differ by operating system

We use Ninja to generate `compile_commands.json`. Ninja uses the compiler and options configured by CMake; therefore, the compilation database contains:

- **Windows:** MSVC (`cl.exe`) commands and MSVC-style options. Because `clang-tidy` is Clang-based, we pass `-extra-arg=--driver-mode=cl` so Clang parses the commands using MSVC driver conventions.
- **Linux:** Clang commands and Clang-style options, which `clang-tidy` understands natively. We do not pass the Windows driver-mode option on Linux because it can alter option parsing and cause header lookup failures, such as `cstdint` not being found.

## 3. Maintenance

When changing the project’s C++ standard, update the `$extra` flags in `static-code-analysis.yml` so they remain consistent with the CMake configuration.
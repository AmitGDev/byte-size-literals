`src\main.cpp` should demonstrate:

1. `_kb`, `_mb`, `_gb`, and `_tb` binary (1024-based) conversions.
2. Compile-time `static_assert` validation.
3. Successful conversion to an integral destination type.
4. A compile-time overflow example, kept commented out because it must fail.

Keep the demonstration deterministic and avoid timing-dependent or environment-dependent behavior.
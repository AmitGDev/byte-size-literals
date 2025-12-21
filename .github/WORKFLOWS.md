# GitHub CI/CD Infrastructure

This directory contains GitHub Actions workflows and reusable composite actions for automated CI/CD pipelines.

## Architecture

The CI/CD infrastructure follows a **"Recipe Pattern"** design:
- **Workflows** (recipes) define high-level, platform-agnostic orchestration
- **Composite Actions** (ingredients) encapsulate platform-specific implementation details

This separation keeps workflows clean, maintainable, and cross-platform without conditional clutter.

## Directory Structure

```
.github/
├── workflows/
│   └── static-code-analysis.yml          # Main static analysis recipe
├── actions/
│   ├── check-formatting/
│   │   └── action.yml                    # Validates code formatting with clang-format
│   ├── setup-cpp-tools/
│   │   └── action.yml                    # Installs build tools and configures environment
│   ├── configure-cmake/
│   │   └── action.yml                    # Configures CMake build system
│   ├── build-project/
│   │   └── action.yml                    # Compiles the C++ project
│   └── run-clang-tidy/
│       └── action.yml                    # Runs clang-tidy static analysis
└── scripts/
    └── run-clang-tidy.py                 # Python script for parallel clang-tidy execution
```

## Workflows

### Static Code Analysis Recipe (`static-code-analysis.yml`)

**Purpose:** Cross-platform static analysis for C++23 code

**Triggers:**
- Push to any branch
- Pull requests
- Weekly schedule (Sundays at 06:00 UTC)
- Manual dispatch

**Matrix Strategy:**
- **Platforms:** Windows (latest), Ubuntu (latest)
- **Configurations:** Debug, Release
- **Architecture:** x64 on all platforms

**Pipeline Steps:**
1. Checkout repository
2. Validate code formatting (clang-format)
3. Setup build environment (compilers, tools)
4. Configure CMake with Ninja generator
5. Build project with C++23 standard
6. Verify compilation database
7. Run clang-tidy with warnings-as-errors
8. Optional: CodeQL security analysis
9. Upload artifacts on failure

**Features:**
- ✅ clang-format enforcement
- ✅ clang-tidy static analysis
- ✅ Zero platform-specific logic in main workflow
- ✅ Parallel builds with Ninja
- ✅ Compilation database validation
- ✅ Build artifact retention (7 days)
- ⚙️ Optional CodeQL integration (controlled via `CODEQL_ENABLED` env var)

## Composite Actions

### `check-formatting`
Validates C++ source files against clang-format rules. Fails the build if formatting issues are detected.

**Platforms:** Cross-platform (uses bash)  
**Input:** None  
**Output:** Exit code 0 (success) or 1 (formatting issues)

---

### `setup-cpp-tools`
Installs and configures the C++ build environment with pinned LLVM 20.1.8:
- **Windows:** LLVM 20.1.8 (clang-tidy, clang-format), Ninja, MSVC 2022
- **Linux:** LLVM 20.1.8 (clang-tidy-20, clang-20) via official LLVM apt repository, ninja-build

**Important:** Both platforms use the **exact same LLVM version (20.1.8)** to ensure consistent static analysis results.

**Platforms:** Windows, Linux  
**Input:** None  
**Output:** Tools available in PATH

---

### `configure-cmake`
Configures the CMake build system with platform-specific compilers:
- **Windows:** MSVC (cl.exe) with x64 architecture
- **Linux:** Clang (clang++)

**Platforms:** Windows, Linux  
**Input:** `config` (Debug/Release)  
**Output:** Build directory with CMake configuration

---

### `build-project`
Compiles the C++ project using CMake and Ninja:
- **Windows:** Uses MSVC x64 toolchain
- **Linux:** Uses Clang

**Platforms:** Windows, Linux  
**Input:** `config` (Debug/Release)  
**Output:** Compiled binaries and compilation database

---

### `run-clang-tidy`
Executes clang-tidy static analysis with project-specific configuration:
- Parallel execution (uses all available CPU cores)
- Warnings treated as errors
- Header filter: `source/.*`
- Source filter: `source/.*`

**Platforms:** Windows, Linux  
**Input:** `build-dir` (default: `build`)  
**Output:** Static analysis results (fails on warnings)

## Configuration

### Enable CodeQL Analysis
Set the environment variable in the workflow:
```yaml
env:
  CODEQL_ENABLED: 1  # Set to 0 to disable
```

### Customize clang-tidy Rules
Edit `.clang-tidy` in the repository root to configure checks.

### Modify Build Configurations
Update the matrix strategy in `static-code-analysis.yml`:
```yaml
matrix:
  os: [windows-latest, ubuntu-latest, macos-latest]  # Add macOS
  config: ['Debug', 'Release', 'RelWithDebInfo']     # Add configs
```

## Usage

### Running Locally

**Check formatting:**
```bash
clang-format --dry-run --Werror source/**/*.{cpp,hpp,h}
```

**Fix formatting:**
```bash
clang-format -i source/**/*.{cpp,hpp,h}
```

**Run clang-tidy manually:**
```bash
cmake -B build -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
python .github/scripts/run-clang-tidy.py -p build "source/.*"
```

### Extending the Pipeline

To add a new workflow step:

1. **Simple step:** Add directly to the workflow
2. **Platform-specific step:** Create a new composite action

Example composite action structure:
```yaml
name: 'Action Name'
description: 'What it does'

inputs:
  param-name:
    description: 'Parameter description'
    required: true

runs:
  using: 'composite'
  steps:
    - name: Step (Windows)
      if: runner.os == 'Windows'
      shell: pwsh
      run: |
        # Windows implementation

    - name: Step (Linux)
      if: runner.os == 'Linux'
      shell: bash
      run: |
        # Linux implementation
```

## Design Principles

1. **Separation of Concerns:** Workflows orchestrate; actions implement
2. **Cross-Platform by Default:** Write once, run everywhere
3. **Fail Fast:** Catch issues early in the pipeline
4. **Artifact Preservation:** Save logs and databases for debugging
5. **Explicit over Implicit:** Specify architectures, compilers, and flags clearly
6. **Idempotent Actions:** Actions can be run multiple times safely

## Maintenance

### Updating Dependencies

**LLVM version (both platforms):**
Both Windows and Linux are pinned to **LLVM 20.1.8** for consistency.

To update to a newer version, edit `.github/actions/setup-cpp-tools/action.yml`:

**Windows:**
```yaml
choco install llvm --version=20.1.8 -y  # Update version here
```

**Linux:**
```yaml
# Update repository URL to match new version
sudo add-apt-repository "deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-20 main" -y

# Update package names
sudo apt-get install -y clang-tidy-20 clang-20 ninja-build  # Change -20 to new version
```

**Important:** Keep both platforms synchronized to ensure consistent static analysis results across all CI/CD runs.

### Testing Changes

1. Create a feature branch
2. Push changes to trigger the workflow
3. Review Actions tab for results
4. Check artifacts if build fails

### Common Issues

**Issue:** "compile_commands.json not found"  
**Solution:** Ensure CMake is configured with `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON` (should be automatic with Ninja)

**Issue:** clang-tidy warnings fail the build  
**Solution:** Fix the warnings or adjust `.clang-tidy` configuration

**Issue:** Different clang-tidy versions on Windows vs Linux  
**Solution:** Both platforms are pinned to LLVM 20.1.8 in the `setup-cpp-tools` action

## Benefits of This Architecture

- **~60% reduction** in main workflow size (220 → 95 lines)
- **Reusable actions** across multiple workflows
- **Clear separation** between "what" (workflow) and "how" (actions)
- **Easier testing** of individual components
- **Consistent behavior** across platforms
- **Maintainable** platform-specific code isolation

## Contributing

When adding new workflows or actions:
1. Follow the recipe pattern (platform-agnostic workflows)
2. Use composite actions for platform-specific logic
3. Document inputs, outputs, and platform requirements
4. Test on all target platforms
5. Update this README with new functionality

---

**Last Updated:** December 2025  
**Maintainer:** Project Team
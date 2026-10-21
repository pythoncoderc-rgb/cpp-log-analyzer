# C++ Log Analyzer

[![build-and-test](https://github.com/pythoncoderc-rgb/cpp-log-analyzer/actions/workflows/build.yml/badge.svg)](https://github.com/pythoncoderc-rgb/cpp-log-analyzer/actions/workflows/build.yml)

A small C++17 command-line utility that reads text logs, detects common severity markers, prints aggregate counts, and optionally filters one severity level.

## What it demonstrates

- file input with `std::ifstream`
- string parsing without third-party libraries
- `enum class` and strongly typed severity values
- aggregation with a fixed-size array
- filtering with reusable library functions
- CMake library/app/test targets
- CTest
- GitHub Actions on Windows and Linux

## Supported severity tags

- `DEBUG`
- `INFO`
- `WARN` / `WARNING`
- `ERROR`
- `CRITICAL` / `FATAL`
- lines without a recognized tag are counted as `UNKNOWN`

The parser scans bracketed fields, so a timestamp can appear before the severity:

```text
[2026-10-21 12:00:00] [INFO] Application started
```

## Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

## Test

```bash
ctest --test-dir build -C Release --output-on-failure
```

## Run

```bash
./build/log_analyzer examples/sample.log
./build/log_analyzer examples/sample.log --level ERROR
```

On a Visual Studio multi-config build, the executable is normally under `build\\Release\\`.

## Structure

```text
include/log_analyzer.hpp   public API
src/log_analyzer.cpp       parser and statistics
src/main.cpp               CLI
examples/sample.log        example input
tests/                     unit-style assertions
.github/workflows/         CI
```

## Scope

This is a focused command-line parser. It does not implement every log format, timestamp parser, multiline event format, or streaming observability feature.

## License

MIT

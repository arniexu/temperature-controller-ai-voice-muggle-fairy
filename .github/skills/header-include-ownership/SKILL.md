# header-include-ownership

Scope: project-constraint

## Purpose
Keep header dependencies one-way and module-owned.

## Rules
1. GPIO pin macros belong in `Core/Inc/gpio.h`.
2. App-level orchestration belongs in `main.c` and related app files.
3. Prefer module headers not to include `main.h` unless strictly required.
4. If macros move, update direct consumers to include the owning module header.
5. Avoid broad refactors when a narrow include fix solves the issue.

## Validation
- Search for missing macro consumers after moves.
- Build and confirm no new include-not-found regressions.

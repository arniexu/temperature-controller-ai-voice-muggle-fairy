# dual-main-entry-guard

Scope: project-constraint

## Purpose
Prevent accidental edits to the wrong `main.c` in this repository.

## Repository Facts
- Production entry: `Core/Src/main.c`
- UI test entry: `UI/Core/Src/main.c`

## Rules
1. Default to editing only `Core/Src/main.c`.
2. Do not edit `UI/Core/Src/main.c` unless user explicitly asks.
3. When proposing entry changes, state which `main.c` is targeted.
4. Before commit or validation, check both files are unchanged unless intended.

## Validation
- `git status -- Core/Src/main.c UI/Core/Src/main.c`

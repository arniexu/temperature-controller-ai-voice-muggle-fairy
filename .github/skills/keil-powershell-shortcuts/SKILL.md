# keil-powershell-shortcuts

Scope: project-constraint

## Purpose
Provide short PowerShell entrypoints for Keil build workflows in this repository.

## Trigger Phrases
- "build"
- "rebuild"
- "clean"
- "flash"
- "tc"
- "btempc"
- "run the short command"
- "open workspace and build"

## Canonical Files
- Workspace command source: `.vscode/tempc-commands.ps1`
- Workspace loader: `.vscode/tempc-bootstrap.ps1`
- VS Code task wrapper: `.vscode/keil-build.ps1`
- VS Code task definitions: `.vscode/tasks.json`
- PowerShell profile loader: `Microsoft.PowerShell_profile.ps1`

## Available Commands
- `build_ai_tempc`
- `rebuild_ai_tempc`
- `clean_ai_tempc`
- `flash_ai_tempc`
- `tempc` / `tc`
- `btempc`, `rtempc`, `ctempc`, `ftempc`

## Behavior
1. Prefer the workspace-local command pack over ad-hoc terminal commands.
2. Treat the PowerShell profile as a bootstrap only; keep the actual command logic in `.vscode/tempc-commands.ps1`.
3. Use `tempc` or `tc` when the user wants one compact entrypoint with subcommands.
4. Use `tc log` and `tc err` to inspect build output and the first blocking errors.
5. Keep build output visible by printing the tail of the Keil log after UV4 completes.

## Examples
- `tc b` -> build the main target
- `tc r` -> rebuild the main target
- `tc c` -> clean generated build outputs
- `tc f` -> flash/download with UV4
- `tc err` -> show current build errors
- `tc log 50` -> show the last 50 log lines

## Guardrails
- Do not duplicate the command implementation in the PowerShell profile.
- Do not assume UV4 output is sufficient without reading the log file.
- Keep the workspace script as the single source of truth for these commands.

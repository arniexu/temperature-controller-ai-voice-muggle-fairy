# keilbridge-issue-workflow

Scope: generic

## Purpose
Run a structured KeilBridge diagnosis and track issues with status transitions.

## Steps
1. Run `inspect` for quick target summary.
2. Run `model` for normalized findings.
3. Run `doctor backend` when toolchain setup is suspected.
4. Run `doctor elf` and `doctor flash` for runtime/packaging phases.
5. Update issue tracker status per issue:
   - `open` -> `in_progress` -> `resolved` or `blocked`

## Required Artifacts
- `inspect.txt`
- `model.json`
- `issue-tracker.json`
- `activity-log.md`
- `status-summary.json`

## Guardrails
- One abnormal finding equals one tracked issue.
- Do not batch unrelated fixes into one issue update.

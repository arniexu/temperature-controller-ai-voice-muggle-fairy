# keil-build-evidence-diagnosis

Scope: generic

## Purpose
Diagnose Keil build failures using evidence from logs, not exit codes.

## Triggers
- "Keil build failed"
- "Can this project compile now"
- "Check build status"

## Inputs
- `.uvprojx` path
- target name
- latest UV4/HTML build log path

## Steps
1. Run target build.
2. Read HTML build log and extract:
   - first blocking error
   - total errors/warnings
   - target output status (`axf` created or not)
3. Group failures by category: include/path, source list, toolchain, link/flash.
4. Return a short blocker list with file and line references.

## Output Format
- Build status: pass/fail
- Blocking errors (ordered)
- Evidence paths
- Suggested smallest next fix

## Guardrails
- Never conclude success from UV4 exit code alone.
- Always cite log evidence.

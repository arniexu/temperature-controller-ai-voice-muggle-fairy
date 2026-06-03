# continuity-sync

Scope: generic

## Purpose
Keep decisions and issue status changes synced to Continuity CLI.

## When
- After each meaningful repair or policy change.
- When issue status changes in tracker.

## Command
`continuity log "<question>" "<answer>" --tags <tag1,tag2,...>`

## Logging Checklist
- issue id and target
- symptom and root cause
- exact repair
- validation command/evidence
- current status (`open|in_progress|resolved|blocked`)

## Guardrails
- Do not manually edit `.continuity/*` backend files for issue logging.
- Search for duplicate decisions before logging a new one.

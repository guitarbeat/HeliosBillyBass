# Firmware Workspace

This workspace hosts multiple firmware projects under a unified structure.

## Directory layout

| Path | Description | Entry point |
| ---- | ----------- | ----------- |
| `projects/billybass_v2/` | fresh attempt at controlling the fish | `billybass_v2.ino` |
| `projects/archive/btbillybass/` | original Bluetooth Billy Bass firmware (read-only) | `BTBillyBass.ino` |
| `projects/helios/` | documentation site, examples, and supporting materials | n/a |

## Quick start

1. For the new fish controller, open `projects/billybass_v2/billybass_v2.ino` in the Arduino IDE.
2. For the archived prototype, open `projects/archive/btbillybass/BTBillyBass/BTBillyBass.ino` for reference.
3. For docs and examples, see `projects/helios/README.md` and `projects/helios/docs/`.

## Conventions

- Add new firmware projects under `projects/<project-name>/`.
- Keep each project self-contained with its own source, docs, examples, and libraries.
- Shared utilities may later live under top-level `tools/` or `shared/`.

## Notes

This consolidation replaces legacy `btb/` and `helios/` content with a clearer `projects/` layout.

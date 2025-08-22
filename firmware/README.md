# Firmware Workspace

This workspace hosts multiple firmware-related projects under a unified structure.

## Layout

- `projects/`
  - `billybass_v2/` – fresh attempt at controlling the fish
  - `archive/btbillybass/` – original Bluetooth Billy Bass firmware (read-only)
  - `helios/` – documentation site, examples, and supporting materials

## Quick start

- New attempt: open `projects/billybass_v2/billybass_v2.ino` in the Arduino IDE and begin implementing.
- Archived firmware: open `projects/archive/btbillybass/BTBillyBass/BTBillyBass.ino` for reference.
- Docs and examples: see `projects/helios/README.md` and `projects/helios/docs/`.

## Conventions

- Add new firmware projects under `projects/<project-name>/`.
- Keep each project self-contained: source, docs, examples, and libraries stay within the project folder.
- Cross-project utilities can later live under top-level `tools/` or `shared/` if needed.

## Notes

This consolidates legacy `btb/` and `helios/` content into `projects/` for a cleaner monorepo layout.

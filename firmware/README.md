# Firmware Workspace

This workspace hosts multiple firmware-related projects under a unified structure.

## Layout

- `projects/`
  - `btbillybass/` – Arduino Big Mouth Billy Bass firmware (primary fish controller)
  - `helios/` – Documentation site, examples, and supporting materials

## Quick start

- BTBillyBass firmware: open `projects/btbillybass/BTBillyBass/BTBillyBass.ino` in the Arduino IDE and build/upload per the project README.
- Docs and examples: see `projects/helios/README.md` and `projects/helios/docs/`.

## Conventions

- Add new firmware projects under `projects/<project-name>/`.
- Keep each project self-contained: source, docs, examples, and libraries stay within the project folder.
- Cross-project utilities can later live under top-level `tools/` or `shared/` if needed.

## Notes

This consolidates legacy `btb/` and `helios/` content into `projects/` for a cleaner monorepo layout.

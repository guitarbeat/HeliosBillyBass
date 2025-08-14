# Contributing

## Guidelines
- Follow code style in `src/` (clear naming, guard clauses, minimal nesting)
- Use Doxygen-style comments for public APIs (`@param`, `@return`, `@see`)
- Update documentation in `docs/` when behavior or interfaces change
- Add an entry to `CHANGELOG.md` (Keep a Changelog format)

## Where to add things
- Hardware abstraction: `src/drivers/`
- Core logic/features: `src/core/`
- Config/constants: `src/core/Config.h`

## Testing
- Verify with hardware when possible
- Exercise serial commands and safety limits
- Use debug mode (`d`) to inspect state and settings

## Docs
- Edit or add focused pages in `docs/`
- Keep the root `README.md` concise, link out to `docs/`
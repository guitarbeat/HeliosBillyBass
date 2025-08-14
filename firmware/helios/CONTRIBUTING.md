# Contributing

Thank you for your interest in contributing!

## Workflow
- Create a feature branch from `main`
- Use concise commits; prefer Conventional Commits (e.g., feat:, fix:, docs:)
- Open a Pull Request with a clear description and checklist

## Code style
- Arduino/C++: keep code readable, avoid deep nesting, handle edge cases early
- Documentation: update relevant docs in `docs/` when changing behavior

## Monorepo preparation
This repository is preparing to host multiple projects under `projects/`. When adding new code:
- Place project-specific code under `projects/<ProjectName>/`
- Keep cross-project assets under shared directories (`docs/`, `vendor/`, `.github/`)
- Scope CI workflows using path filters when possible

## Docs
- MkDocs site is configured via `mkdocs.yml`
- Add or update pages in `docs/`; link from `docs/index.md`
- Keep links relative and verified with `mkdocs build` locally if possible

## Pull Requests
- Include tests or instructions for manual verification
- Update `CHANGELOG.md` for user-visible changes
- Tag reviewers in CODEOWNERS if applicable
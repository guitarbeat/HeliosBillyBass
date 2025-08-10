# Merge Preparation Guide

This repository is structured as a monorepo to host multiple projects. To merge another repository into this one without losing history, follow these steps.

## Option A: Preserve full Git history (recommended)

1. Add the other repo as a remote:
   - `git remote add other <url>`
   - `git fetch other --tags`
2. Create a subtree directory for it:
   - Choose `projects/<name>/<subdir>` (e.g., `projects/another/ProjectName`)
3. Rewrite the other repo history to live under that path:
   - `git checkout --orphan temp-merge`
   - `git read-tree --prefix=projects/<name>/<subdir>/ -u other/<default-branch>`
   - `git commit -m "chore: import <repo> into projects/<name>/<subdir> (preserve history)"`
4. Merge into main branch:
   - `git checkout <your-main-branch>`
   - `git merge --allow-unrelated-histories temp-merge`
   - `git branch -D temp-merge`

Notes:
- If large or binary files exist, consider using Git LFS first.
- Validate build paths after merge; update relative links as needed.

## Option B: Subtree merge (alternative)

- `git subtree add --prefix=projects/<name>/<subdir> <url> <branch> --squash`
- Later updates: `git subtree pull --prefix=projects/<name>/<subdir> <url> <branch> --squash`

## Option C: Squash import

- Create the directory, copy files, then commit with a message noting the source commit/tag. This loses history but is simplest.

## Post-merge checklist

- Update top-level `README.md` project index
- Ensure unique toolchain files do not clash across projects
- Deduplicate shared libraries (e.g., Arduino libraries) into `projects/<name>/<subdir>/libraries` or a shared `shared/` folder
- Fix CI and format/lint configurations to target project subpaths
- Run builds/tests for each project
- Update any docs links to the new paths
# Monorepo Merge Guide

This guide outlines how to merge this repository with another into a single monorepo while preserving history and minimizing disruption.

## Goals
- Preserve full git history of both repos
- Avoid path collisions and clarify ownership
- Keep docs/CI working during and after the merge
- Prepare for multiple projects under one roof

## Target structure
```
/                     # Monorepo root
├─ projects/
│  ├─ BTBillyBass/   # This project (current location, to be moved later)
│  └─ <OtherProject>/
├─ docs/
│  ├─ index.md
│  ├─ MERGE_GUIDE.md
│  └─ projects/
│     ├─ BTBillyBass/        # (optional) project-scoped docs
│     └─ <OtherProject>/
├─ .github/
│  └─ workflows/
├─ vendor/
│  └─ arduino/
│     └─ libraries/          # (optional future) shared Arduino libs
└─ mkdocs.yml
```

Notes:
- We will not move `BTBillyBass/` immediately; plan is to relocate into `projects/BTBillyBass/` after merge to reduce churn.
- Arduino libraries currently under `BTBillyBass/libraries/` will remain in place until both projects agree on a shared `vendor/arduino/libraries/` convention.

## Merge strategy (preserve history)

### Option A: git subtree (recommended)
```
# Replace the placeholders below accordingly
REMOTE_NAME=<other_remote>
REMOTE_URL=<https://github.com/org/other-repo.git>
REMOTE_BRANCH=main
TARGET_DIR=projects/<OtherProject>

# Add and fetch remote
git remote add "$REMOTE_NAME" "$REMOTE_URL"
git fetch "$REMOTE_NAME" "$REMOTE_BRANCH"

# Create target directory (if not exists)
mkdir -p "$TARGET_DIR"

# Import as subtree, preserving history (no squash)
git subtree add --prefix="$TARGET_DIR" "$REMOTE_NAME" "$REMOTE_BRANCH"

# Alternatively, to squash history into one commit:
# git subtree add --prefix="$TARGET_DIR" "$REMOTE_NAME" "$REMOTE_BRANCH" --squash
```

### Option B: read-tree (advanced)
```
TARGET_DIR=projects/<OtherProject>
REMOTE_NAME=<other_remote>
REMOTE_BRANCH=main

git remote add "$REMOTE_NAME" <url>
git fetch "$REMOTE_NAME" "$REMOTE_BRANCH"

git read-tree --prefix="$TARGET_DIR" -u "$REMOTE_NAME/$REMOTE_BRANCH"

git commit -m "Import <OtherProject> into $TARGET_DIR via read-tree"
```

## Post-merge tasks
- Resolve path collisions (e.g., duplicate `README.md`, `LICENSE`, `.github` files)
  - Keep monorepo root `README.md`, move project-specific readmes to `projects/<Project>/README.md`
  - Keep a single root `LICENSE`; ensure project LICENSE specifics are referenced from project README
  - Consolidate `.github/workflows` and templates; scope by paths if needed
- Update docs links
  - Prefer relative links within project docs
  - Add a Docs nav section per project in `mkdocs.yml`
- CI adjustments
  - Use path filters in workflows to run jobs only for changed projects
  - Keep MkDocs deploy as-is; it uses `docs/` root
- Arduino specifics
  - If consolidating libraries, move to `vendor/arduino/libraries/` and install via Arduino Library Manager or adjust include paths
  - Keep `#include "MX1508.h"` intact; ensure library resolution strategy is documented

## Moving this project after merge (optional)
```
# Move code into projects/BTBillyBass and update internal links
mkdir -p projects/BTBillyBass
git mv BTBillyBass projects/BTBillyBass

# Update docs references if they refer to old paths
# (Search for BTBillyBass.ino in docs and adjust if necessary)

git commit -m "Move BTBillyBass into projects/BTBillyBass"
```

## Link hygiene checklist
- [ ] Update any absolute paths in docs to new `projects/<Project>/` locations
- [ ] Verify GitHub Actions path filters
- [ ] Verify MkDocs nav works and links resolve
- [ ] Add CODEOWNERS entries for new project

## Rollback plan
If the merge introduces regressions:
- Revert the subtree/read-tree commit that imported the other project
- Re-run the process with `--squash` to minimize conflicts, or create an intermediate branch for conflict resolution

## References
- Git Subtree docs: https://git-scm.com/book/en/v2/Git-Tools-Subtree-Merging
- Read-tree method: https://git.kernel.org/pub/scm/git/git.git/tree/Documentation/howto/using-merge-subtree.txt
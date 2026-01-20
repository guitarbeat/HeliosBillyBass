# Palette's Journal

## 2025-10-25 - [Accessibility Improvements]
**Learning:** Icon-only buttons using `title` attribute for tooltips are insufficient for screen readers; explicit `aria-label` is required. Material Icons should be hidden from screen readers using `aria-hidden="true"` to prevent reading ligature text.
**Action:** Always pair icon-only buttons with `aria-label` and hide decorative icons. Ensure custom macros enforce these patterns.

## 2025-10-26 - [Form Tooltip Accessibility]
**Learning:** Tooltips implemented as `<span>` elements inside `<label>` elements are inaccessible to keyboard users and violate HTML nesting rules.
**Action:** Use the `label_with_help` macro which places the help button as a sibling to the label in a flex container, ensuring semantic validity and keyboard accessibility with explicit `aria-label`.

# Palette's Journal

## 2025-10-25 - [Accessibility Improvements]
**Learning:** Icon-only buttons using `title` attribute for tooltips are insufficient for screen readers; explicit `aria-label` is required. Material Icons should be hidden from screen readers using `aria-hidden="true"` to prevent reading ligature text.
**Action:** Always pair icon-only buttons with `aria-label` and hide decorative icons. Ensure custom macros enforce these patterns.

## 2025-10-26 - [Form Accessibility]
**Learning:** Interactive elements (like help buttons) nested inside `<label>` elements are invalid HTML and confusing for screen readers. Using `<span>` with `onclick` for tooltips is inaccessible to keyboard users.
**Action:** Use the `label_with_help` macro to render accessible structures where the help button is a sibling to the label, ensuring full keyboard navigability and valid HTML.

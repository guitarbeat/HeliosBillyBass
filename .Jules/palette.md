# Palette's Journal

## 2025-10-25 - [Accessibility Improvements]
**Learning:** Icon-only buttons using `title` attribute for tooltips are insufficient for screen readers; explicit `aria-label` is required. Material Icons should be hidden from screen readers using `aria-hidden="true"` to prevent reading ligature text.
**Action:** Always pair icon-only buttons with `aria-label` and hide decorative icons. Ensure custom macros enforce these patterns.

## 2025-10-27 - [Custom Sliders Accessibility]
**Learning:** Custom visual sliders that hide the native `<input type="range">` with `display: none` (e.g., `hidden` class) completely remove the control from the accessibility tree, making it unusable for keyboard and screen reader users.
**Action:** Use `sr-only` (screen-reader only) utility instead of `hidden` to keep the input focusable but invisible. Use CSS peer selectors (e.g., `peer-focus:ring`) to style the adjacent custom visual element when the invisible input has focus, ensuring a visible focus indicator.

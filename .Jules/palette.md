# Palette's Journal

## 2025-10-25 - [Accessibility Improvements]
**Learning:** Icon-only buttons using `title` attribute for tooltips are insufficient for screen readers; explicit `aria-label` is required. Material Icons should be hidden from screen readers using `aria-hidden="true"` to prevent reading ligature text.
**Action:** Always pair icon-only buttons with `aria-label` and hide decorative icons. Ensure custom macros enforce these patterns.

## 2025-10-26 - [Accordion Accessibility]
**Learning:** Collapsible sections implemented with only `<div>` or headings and click handlers are not accessible to keyboard or screen reader users.
**Action:** Use the "Button in Heading" pattern: wrap the trigger in a `<button>` with `aria-expanded` and `aria-controls`. Wrap content in a region with `role="region"` and `aria-labelledby`.

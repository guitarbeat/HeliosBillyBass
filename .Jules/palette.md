# Palette's Journal

## 2025-10-25 - [Accessibility Improvements]
**Learning:** Icon-only buttons using `title` attribute for tooltips are insufficient for screen readers; explicit `aria-label` is required. Material Icons should be hidden from screen readers using `aria-hidden="true"` to prevent reading ligature text.
**Action:** Always pair icon-only buttons with `aria-label` and hide decorative icons. Ensure custom macros enforce these patterns.

## 2026-01-15 - [Dynamic Content Accessibility]
**Learning:** Dynamic buttons generated in JavaScript (e.g., via template literals) often miss accessibility attributes found in static templates or macros.
**Action:** Review JavaScript files (like `config.js`) for HTML string generation and ensure `aria-label` and `aria-hidden` are included in the markup strings.

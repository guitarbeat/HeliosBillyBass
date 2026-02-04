# Palette's Journal

## 2025-10-25 - [Accessibility Improvements]
**Learning:** Icon-only buttons using `title` attribute for tooltips are insufficient for screen readers; explicit `aria-label` is required. Material Icons should be hidden from screen readers using `aria-hidden="true"` to prevent reading ligature text.
**Action:** Always pair icon-only buttons with `aria-label` and hide decorative icons. Ensure custom macros enforce these patterns.

## 2025-10-26 - [Form Tooltip Pattern]
**Learning:** Legacy help buttons were nested inside `<label>` elements, creating invalid HTML and accessibility issues.
**Action:** Refactored to use a side-by-side flex pattern where the help button is a sibling of the label. Updated `toggleTooltip` JS to support finding the tooltip container from this new structure. Use `label_with_help` macro to enforce this valid pattern.

## 2025-10-27 - [Accessible Collapsible Sections]
**Learning:** Collapsible sections used non-semantic `<h3>` elements as click targets, making them inaccessible to keyboard users and screen readers.
**Action:** Refactored `section_card` macro to use a semantic `<button>` inside the header with `aria-expanded` and `aria-controls` attributes. Updated JavaScript to toggle ARIA states and handle the new structure while maintaining backward compatibility.

## 2025-11-20 - [Dynamic UI Accessibility]
**Learning:** UI components generated dynamically via JavaScript (e.g., personality sliders, service buttons) often lacked the semantic structure (like `<input type="range">` or `aria-label`) present in their static HTML counterparts, leading to hidden accessibility failures.
**Action:** When porting or generating UI in JS, rigorously replicate the accessible HTML structure used in static templates, especially for custom controls like sliders.

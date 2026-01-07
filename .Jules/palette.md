# Palette's Journal

## 2025-02-18 - [Billy B-Assistant] Accessibility Improvements
**Learning:** Icon-only buttons often lack accessible names, relying solely on `title` attributes which are insufficient for screen reader users. Also, custom Javascript-based sliders can be completely inaccessible if they don't proxy a native input.
**Action:** Always verify that icon-only buttons have an `aria-label` or accessible text content. For custom controls, ensure they are backed by native semantic HTML elements or proper ARIA roles and keyboard event handlers.

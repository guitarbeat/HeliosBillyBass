## 2024-05-23 - Accessibility for Icon-only Buttons
**Learning:** Icon-only buttons (like those using Material Icons) are often implemented without accessible names. Adding `aria-label="{{ title }}"` to the button and `aria-hidden="true"` to the icon element significantly improves screen reader experience without affecting visual design.
**Action:** Always check `macros.html` or component libraries for base button implementations and enforce ARIA label patterns there to fix multiple instances at once.

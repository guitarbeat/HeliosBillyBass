## 2024-05-22 - Missing ARIA labels in macros
**Learning:** Reusable macros like `icon_button` often miss accessibility attributes, propagating issues across the app.
**Action:** Always check macros for `aria-label` and `aria-hidden` attributes when they render interactive elements.

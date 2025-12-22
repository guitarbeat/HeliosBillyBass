## 2025-05-23 - Accessibility Improvements
**Learning:**
Icon-only buttons and interactive spans are a common accessibility anti-pattern in this codebase.
- Visibility toggle buttons were completely silent to screen readers (missing `aria-label`).
- Tooltip triggers were implemented as `<span>` elements with `onclick`, making them inaccessible to keyboard users.

**Action:**
- Always add `aria-label` to icon-only buttons.
- Use `<button type="button">` for interactive elements instead of `<span>` or `<div>`, ensuring keyboard focus and activation.
- When replacing spans with buttons, reset default button styles (using Tailwind classes like `bg-transparent border-none p-0`) to maintain visual consistency.

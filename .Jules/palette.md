## 2024-03-24 - Collapsible Sections Accessibility
**Learning:** Collapsible sections implemented with clickable `<h3>` tags are inaccessible to keyboard users and screen readers.
**Action:** Always wrap the toggleable header content in a semantic `<button>` element with `aria-expanded` and `aria-controls` attributes. Ensure the JavaScript click handler is attached to the button, not the heading container.

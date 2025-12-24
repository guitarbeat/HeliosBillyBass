## 2024-05-24 - Accessible Icon Buttons
**Learning:** Icon-only buttons (like toggles and action buttons) were relying solely on `title` or surrounding context, which is insufficient for screen reader users. The `icon_button` macro was updated to automatically apply `aria-label` from the title, ensuring consistent accessibility across the app without manual intervention for every instance.
**Action:** When creating new UI components, always ensure icon-only interactive elements have an explicit accessible name (via `aria-label` or `aria-labelledby`).

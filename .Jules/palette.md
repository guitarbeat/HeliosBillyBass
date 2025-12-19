# Palette's Journal

## 2025-05-18 - Missing Loading Feedback
**Learning:** Users lack immediate visual confirmation when saving configuration or personas, leading to potential uncertainty or double-submission, especially since the "success" notification is asynchronous and disconnected from the button.
**Action:** Implement button loading states (spinner + disabled) for all async form submissions to provide immediate tactile feedback.

## 2025-05-18 - Accessible Icon Buttons
**Learning:** Icon-only buttons relying solely on `title` attributes are not fully accessible and may not provide a clear accessible name to all screen readers.
**Action:** Always include `aria-label` on icon-only buttons, defaulting to the `title` text if no specific label is provided.

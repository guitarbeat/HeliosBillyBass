## 2024-05-23 - [Accessibility Improvement for Icon Buttons]
**Learning:** Material Icons often use ligatures (readable text like "visibility") as the icon content. If not hidden with `aria-hidden="true"`, screen readers may read this text in addition to or instead of the label, which can be confusing.
**Action:** Always add `aria-hidden="true"` to `<span class="material-icons">` elements when they are decorative or accompanying an `aria-label` on the parent button.

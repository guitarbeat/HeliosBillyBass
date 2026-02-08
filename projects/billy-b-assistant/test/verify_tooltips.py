from playwright.sync_api import sync_playwright, expect
import time
import sys

def verify_tooltips():
    with sync_playwright() as p:
        browser = p.chromium.launch(headless=True)
        page = browser.new_page()

        print("Navigating to app...")
        try:
            page.goto("http://localhost:5000")
        except Exception as e:
            print(f"Failed to load page: {e}")
            sys.exit(1)

        # Locate the "OpenAI API Key" help button
        # The button has aria-label="Show help for OpenAI API Key"
        print("Locating help button...")
        help_button = page.locator('button[aria-label="Show help for OpenAI API Key"]')

        # Verify initial state
        print("Checking initial state...")
        expect(help_button).to_be_visible()
        expect(help_button).to_have_attribute("aria-expanded", "false")

        # Get the tooltip ID from aria-controls
        tooltip_id = help_button.get_attribute("aria-controls")
        print(f"Tooltip ID: {tooltip_id}")

        if not tooltip_id:
            print("ERROR: aria-controls attribute missing on help button")
            browser.close()
            sys.exit(1)

        tooltip = page.locator(f"#{tooltip_id}")
        expect(tooltip).to_have_attribute("role", "region")
        expect(tooltip).to_have_attribute("aria-hidden", "true")

        # Check that it is hidden via CSS
        expect(tooltip).not_to_be_visible()

        # Click the button
        print("Clicking help button...")
        help_button.click()

        # Verify expanded state
        print("Checking expanded state...")
        expect(help_button).to_have_attribute("aria-expanded", "true")
        expect(tooltip).to_have_attribute("data-visible", "true")
        expect(tooltip).to_have_attribute("aria-hidden", "false")
        expect(tooltip).to_be_visible()

        page.screenshot(path="verification_tooltip_expanded.png")
        print("Screenshot saved to verification_tooltip_expanded.png")

        # Click again to close
        print("Clicking help button again...")
        help_button.click()

        # Verify collapsed state
        print("Checking collapsed state...")
        expect(help_button).to_have_attribute("aria-expanded", "false")
        expect(tooltip).to_have_attribute("data-visible", "false")
        expect(tooltip).to_have_attribute("aria-hidden", "true")
        expect(tooltip).not_to_be_visible()

        print("✅ Tooltip verification passed!")
        browser.close()

if __name__ == "__main__":
    verify_tooltips()

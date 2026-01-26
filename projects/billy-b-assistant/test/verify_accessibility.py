from playwright.sync_api import sync_playwright, expect

def verify_accessibility():
    with sync_playwright() as p:
        browser = p.chromium.launch(headless=True)
        page = browser.new_page()

        print("Navigating to app...")
        page.goto("http://localhost:5000")

        # Select the Software Settings section header button
        # The title is "Software Settings"
        # Since I changed the macro, the button should be findable by text inside the button
        section_button = page.get_by_role("button", name="Software Settings")

        print("Checking initial state...")
        expect(section_button).to_be_visible()
        expect(section_button).to_have_attribute("aria-expanded", "true")

        # Check if content is visible
        content_div = page.locator("#section-software-content")
        expect(content_div).to_be_visible()

        print("Testing collapse...")
        section_button.click()
        expect(section_button).to_have_attribute("aria-expanded", "false")
        expect(content_div).not_to_be_visible() # .hidden class adds display:none

        print("Testing expand...")
        section_button.click()
        expect(section_button).to_have_attribute("aria-expanded", "true")
        expect(content_div).to_be_visible()

        print("Testing keyboard focus...")
        section_button.focus()

        # Take screenshot of focused state
        page.screenshot(path="verification_accessibility.png")
        print("Screenshot saved to verification_accessibility.png")

        browser.close()

if __name__ == "__main__":
    verify_accessibility()

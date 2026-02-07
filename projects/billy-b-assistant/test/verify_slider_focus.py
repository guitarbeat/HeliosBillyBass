from playwright.sync_api import sync_playwright, expect
import time

def verify_slider_focus():
    with sync_playwright() as p:
        # Use headless=True for the sandbox environment
        browser = p.chromium.launch(headless=True)
        page = browser.new_page()

        print("Navigating to app...")
        try:
            page.goto("http://localhost:5000")
        except Exception as e:
            print(f"Failed to load page: {e}")
            browser.close()
            return

        print("Waiting for #mic-gain-bar...")
        try:
            mic_bar = page.locator("#mic-gain-bar")
            mic_bar.wait_for(state="visible", timeout=5000)

            # Ensure the input is NOT focused initially
            mic_input = page.locator("#mic-gain")
            expect(mic_input).not_to_be_focused()

            print("Clicking #mic-gain-bar...")
            mic_bar.click()

            print("Checking if #mic-gain is focused...")
            expect(mic_input).to_be_focused(timeout=2000)
            print("SUCCESS: Input focused after click!")

            # Take screenshot
            page.screenshot(path="projects/billy-b-assistant/test/verification_slider.png")
            print("Screenshot saved to projects/billy-b-assistant/test/verification_slider.png")

        except Exception as e:
            print(f"Test failed with error: {e}")
            raise
        finally:
            browser.close()

if __name__ == "__main__":
    verify_slider_focus()

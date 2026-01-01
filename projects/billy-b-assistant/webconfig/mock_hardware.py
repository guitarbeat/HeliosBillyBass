import sys
from unittest.mock import MagicMock

# Mock lgpio
sys.modules['lgpio'] = MagicMock()
sys.modules['gpiozero'] = MagicMock()

# Now run the server
import runpy
runpy.run_module('projects.billy-b-assistant.webconfig.server', run_name='__main__')

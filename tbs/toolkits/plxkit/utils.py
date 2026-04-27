import sounddevice as sd
import sys
import time

GREEN = "\033[92m"
RESET = "\033[0m"

FLAG = "USB AUDIO  CODEC"

def list_devices():
    print("Fetching audio devices...",end="")
    sys.stdout.flush()
    devices = sd.query_devices()
    sys.stdout.write('\x1b[1A')
    sys.stdout.write('\x1b[2K')
    sys.stdout.write('\r')

    print("Suggested PLX device will show in "+GREEN+"green"+RESET+" color.")
    for i, dev in enumerate(devices):
        if FLAG in dev['name']:
            print(f"{GREEN}{i}: {dev['name']} ({dev['max_input_channels']} in, {dev['max_output_channels']} out){RESET}")
        else:
            print(f"{i}: {dev['name']} ({dev['max_input_channels']} in, {dev['max_output_channels']} out)")
    return devices

def get_device_index(name_substring, is_input=True):
    """Find device index by name substring."""
    devices = sd.query_devices()
    name_substring = name_substring.lower()
    for i, dev in enumerate(devices):
        if name_substring in dev['name'].lower():
            if is_input and dev['max_input_channels'] > 0:
                return i
            if not is_input and dev['max_output_channels'] > 0:
                return i
    return None

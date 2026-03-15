#!/bin/bash

SERIAL="/dev/ttyACM0"
PIPE="/dev/buzzer"

# Give permissions
chmod a+rw $SERIAL
stty -F $SERIAL 115200 raw -echo

# Create named pipe if it doesn't exist
if [ ! -p "$PIPE" ]; then
    mkfifo "$PIPE"
    chmod 666 "$PIPE"
fi

# Start Python daemon if not already running
if ! pgrep -f buzzerd.py >/dev/null; then
    python3 /root/felix/buzzerd.py &
fi

echo "Felix init done"
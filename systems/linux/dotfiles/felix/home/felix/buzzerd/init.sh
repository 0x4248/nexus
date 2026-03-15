# Setup arduino buzzer

if [ ! -e /dev/ttyACM0 ]; then
    echo "Error: /dev/ttyACM0 not found. Please connect the Arduino device and try again."
    exit 1
fi
chmod a+rw /dev/ttyACM0
stty -F /dev/ttyACM0 115200 raw -echo

mkfifo /tmp/buzzer
chmod 666 /tmp/buzzer

python3 buzzerd.py &

echo 500 > /tmp/buzzer
sleep 1
echo 0 > /tmp/buzzer
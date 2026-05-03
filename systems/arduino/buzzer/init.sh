# Setup arduino buzzer

# Dont sleep, we need to set permissions on the device
chmod a+rw /dev/ttyACM0
stty -F /dev/ttyACM0 115200 raw -echo

mkfifo /tmp/buzzer
chmod 666 /tmp/buzzer

python3 buzzerd.py &

echo 500 > /tmp/buzzer
sleep 1
echo 0 > /tmp/buzzer
# install the felix systemd service

cp /root/felix/systemd/felix.service /etc/systemd/system/
systemctl enable felix.service
echo "Felix service installed. Please reboot to start the service."
consent=""

while [[ ! "$consent" =~ ^[Yy]$ ]]; do
    read -p "Do you want to reboot now? (y/n) " consent
    if [[ "$consent" =~ ^[Yy]$ ]]; then
        clear
        echo "Felix service installer is rebooting the system to start the service. Please wait..."
        systemctl reboot
    else
        exit 0
    fi
done
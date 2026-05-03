# install the felix systemd service

cp /root/felix/systemd/felix.service /etc/systemd/system/
systemctl enable felix.service
chmod -x /root/felix/felix_init.sh
echo "Felix service installed. Please reboot to start the service."

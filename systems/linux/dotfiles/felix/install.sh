# Install/update the felix service
rm -rf /root/felix
cp -rv home/* /root/
cp -rv home/.* /root/
if [ -f /etc/systemd/system/felix.service ]; then
    systemctl disable felix.service
    rm /etc/systemd/system/felix.service
fi

bash /root/felix/systemd/install.sh
systemctl daemon-reload
chmod +x /root/felix/felix_init.sh


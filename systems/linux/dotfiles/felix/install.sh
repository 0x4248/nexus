# Install/update the felix service
rm -rf /root/felix
mkdir -p /root/felix
cp -r home/felix/. /root/felix/.
if [ -f /etc/systemd/system/felix.service ]; then
    systemctl disable felix.service
    rm /etc/systemd/system/felix.service
fi

bash /root/felix/systemd/install.sh
systemctl daemon-reload
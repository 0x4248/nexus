# Enables NTP on Debian-based systems and can fix some synchronization issues.
sudo apt update
sudo apt install systemd-timesyncd
sudo timedatectl set-ntp true
sudo systemctl enable systemd-timesyncd
sudo systemctl start systemd-timesyncd
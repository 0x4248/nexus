#!/bin/sh
set -eu

INSTALL_DIR="/opt/nexus/usr/net/PulseWatch"
DATA_DIR="/var/lib/pulsewatch"
SERVICE="/etc/systemd/system/pulsewatch-server.service"

echo "[+] Installing PulseWatch server"

# user
if ! id pulsewatch >/dev/null 2>&1; then
    useradd -r -s /usr/sbin/nologin pulsewatch
fi

# data dir
mkdir -p "$DATA_DIR"
chown pulsewatch:pulsewatch "$DATA_DIR"
chmod 750 "$DATA_DIR"

# permissions on code
chown -R root:root "$INSTALL_DIR"
chmod -R 755 "$INSTALL_DIR"

# systemd service
cat > "$SERVICE" <<EOF
[Unit]
Description=PulseWatch Server
After=network.target

[Service]
ExecStart=/usr/bin/python3 ${INSTALL_DIR}/server.py
WorkingDirectory=${INSTALL_DIR}
User=pulsewatch
Group=pulsewatch
Restart=always
RestartSec=3

NoNewPrivileges=true
PrivateTmp=true
ProtectSystem=strict
ProtectHome=true
ReadWritePaths=${DATA_DIR}

[Install]
WantedBy=multi-user.target
EOF

systemctl daemon-reload
systemctl enable --now pulsewatch-server

echo "[+] PulseWatch server installed and running"

#!/bin/sh
set -eu

INSTALL_DIR="/opt/nexus/usr/net/PulseWatch"
SERVICE="/etc/systemd/system/pulsewatch-client.service"

SERVER_URL="${1:-http://localhost:9000}"

echo "[+] Installing PulseWatch client"
echo "    Server: ${SERVER_URL}"

# permissions on code
chown -R root:root "$INSTALL_DIR"
chmod -R 755 "$INSTALL_DIR"

# systemd service
cat > "$SERVICE" <<EOF
[Unit]
Description=PulseWatch Client
After=network-online.target
Wants=network-online.target

[Service]
ExecStart=/usr/bin/python3 ${INSTALL_DIR}/client.py ${SERVER_URL}
WorkingDirectory=${INSTALL_DIR}
Restart=always
RestartSec=5

NoNewPrivileges=true
PrivateTmp=true
ProtectSystem=strict
ProtectHome=true

[Install]
WantedBy=multi-user.target
EOF

systemctl daemon-reload
systemctl enable --now pulsewatch-client

echo "[+] PulseWatch client installed and running"

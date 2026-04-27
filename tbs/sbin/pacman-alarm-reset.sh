sudo tee /etc/pacman.d/mirrorlist <<'EOF'
Server = http://mirror.archlinuxarm.org/$arch/$repo
Server = http://de3.mirror.archlinuxarm.org/$arch/$repo
Server = http://sg.mirror.archlinuxarm.org/$arch/$repo
Server = http://us.mirror.archlinuxarm.org/$arch/$repo
EOF

sudo pacman -Syyu

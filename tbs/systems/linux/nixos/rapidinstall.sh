parted /dev/vda -- mklabel gpt
parted /dev/vda -- mkpart root ext4 512MB -8GB
parted /dev/vda -- mkpart swap linux-swap -8GB 100%
parted /dev/vda -- mkpart ESP fat32 1MB 512MB
parted /dev/vda -- set 3 esp on
mkfs.ext4 -L nixos /dev/vda1
mkswap -L swap /dev/vda2
mkfs.fat -F 32 -n boot /dev/vda3
mount /dev/disk/by-label/nixos /mnt
mkdir -p /mnt/boot
mount -o umask=077 /dev/disk/by-label/boot /mnt/boot
swapon /dev/vda2
nixos-generate-config --root /mnt
nano /mnt/etc/nixos/configuration.nix
echo now run when ready: nixos-install
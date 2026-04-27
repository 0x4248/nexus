# floppy-qformat.sh - Quick format a floppy disk with FAT12 filesystem
# COMMAND [DEVICE] [LABEL (optional)]

if [ -z "$1" ]; then
    echo "Usage: $0 [DEVICE] [LABEL (optional)]"
    exit 1
fi

DEVICE="$1"
LABEL="${2:-FLOPPY}"
if [ ! -b "$DEVICE" ]; then
    echo "Error: $DEVICE is not a block device."
    exit 1
fi
umount "$DEVICE" 2>/dev/null
mkfs.vfat -F 12 -n "$LABEL" "$DEVICE"
sync
 
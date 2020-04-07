#!/bin/sh

function create-image() {
    img="efi-image.img"
    [ -f "$img" ] && echo Saving old img... &&  mv -f ${img}{,.old}
    size=$((32*(1<<20))) # desired size in bytes, 32MB in this case
    offset=1048576 # filesystem starts at 1MB offset
    fs="$img"@@"$offset"

    # image size is gpt + filesystem size + gpt backup
    echo Creating new iso...
    truncate -s $((size + 3*offset)) "$img"

    echo Creating gpt partition table with ESP...
    parted --script --align min "$img" \
        mklabel gpt                    \
        mkpart EFI "2048s" '100%'      \
        set 1 boot on

    echo Writing FAT16 filesystem at offset $offset...
    mformat -i "$fs" -t 31 -h 32 -n 64 -c 1 -v "KOS"
    echo Copying boot files...
    mmd     -i "$fs" ::/EFI{,/BOOT}
    mcopy   -i "$fs" "$1" ::/EFI/BOOT/BOOTX64.EFI
}

function run-qemu() {
    qemu-system-x86_64 -cpu host -enable-kvm \
        -bios /usr/share/ovmf/x64/OVMF.fd -net none \
        -drive format=raw,file="$1" -nographic
}

function find-esp() {
    device="$(lsblk -nlp -o NAME,PARTTYPE | \
                awk 'toupper($2) ~ /C12A7328-F81F-11D2-BA4B-00A0C93EC93B/{print $1}')"
    findmnt "$device" -no target
}

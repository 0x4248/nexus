#!/bin/bash


run_update() {
    PASSWORD=$(dialog --insecure --passwordbox "Enter your sudo password:" 10 50 3>&1 1>&2 2>&3 3>&-)

    if [ $? -ne 0 ]; then
        dialog --msgbox "Update cancelled." 6 30
        return
    fi

    echo "$PASSWORD" | sudo -S true 2>/dev/null
    if [ $? -ne 0 ]; then
        dialog --msgbox "Authentication failed. Please try again." 6 40
        return
    fi

    dialog --infobox "Running system update... please wait." 5 40
    TEMP_OUTPUT=$(mktemp)
    echo "$PASSWORD" | sudo apt upgrade >"$TEMP_OUTPUT" 2>&1

    dialog --textbox "$TEMP_OUTPUT" 20 70
    rm -f "$TEMP_OUTPUT"
}

ping(){
    TEMP_OUTPUT=$(mktemp)
    ping 1.1.1.1 >"$TEMP_OUTPUT" 2>&1

    dialog --textbox "$TEMP_OUTPUT" 20 70
    rm -f "$TEMP_OUTPUT"
}

while true; do
    CHOICE=$(dialog --clear --backtitle "System Update Tool" \
        --title "Main Menu" \
        --menu "Choose an option:" 12 40 2 \
        1 "Run Update" \
        2 ""
        2 "Exit" \
        3>&1 1>&2 2>&3)

    case $CHOICE in
        1)
            run_update
            ;;
        2)
            clear
            exit 0
            ;;
        *)
            clear
            exit 0
            ;;
    esac
done

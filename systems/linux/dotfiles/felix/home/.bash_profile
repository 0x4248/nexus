# Display centered message with blue background on TTY
clear
message="TITAN DATA SYSTEM | FELIX CONSOLE"
cols=$(tput cols)
padding=$(( (cols - ${#message}) / 2 ))
echo -e "\e[44m\e[97m$(printf "%*s%s%*s\n" $padding "" "$message" $padding "")\e[0m"

echo ""
echo "System has been $(uptime -p)"
date
echo ""


free -h
echo ""
lsblk
echo""
printf "\e[44m%${cols}s\e[0m\n" ""

echo -e "500\nd50\n600\nd50\n700\nd50\n0" > /dev/buzzer

alias logout="echo -e \"600\nd50\n500\nd100\n0\" > /dev/buzzer && logout"
alias exit="echo -e \"600\nd50\n500\nd100\n0\" > /dev/buzzer && exit"

source .bashrc

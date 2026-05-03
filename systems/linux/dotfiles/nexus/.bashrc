#
# ~/.bashrc
#

# If not running interactively, don't do anything
[[ $- != *i* ]] && return


echo -e " _   _ _______  ___   _ ____  "
echo -e "| \ | | ____\ \/ | | | / ___| "
echo -e "|  \| |  _|  \  /| | | \___ \ "
echo -e "| |\  | |___ /  \| |_| |___) |"
echo -e "|_| \_|_____/_/\_\\___/|____/ "
echo ""
echo "Welcome to nexus intranet"
uptime -p
uname -a

alias ls='ls --color=auto'
alias ll='ls -alF'
alias la='ls -A'
alias l='ls -CF'
alias h='cd ~'
alias e='exit'
alias q='exit'
alias grep='grep --color=auto'

[[ $- != *i* ]] && return

if [[ -z "${BASHRCSOURCED}" ]] ; then
  BASHRCSOURCED="Y"
  [[ "$PS1" = '\s-\v\$ ' ]] && PS1='[\u@\h \W]\$ '
  case ${TERM} in
    Eterm*|alacritty*|aterm*|foot*|gnome*|konsole*|kterm*|putty*|rxvt*|tmux*|xterm*)
      PROMPT_COMMAND+=('printf "\033]0;%s@%s:%s\007" "${USER}" "${HOSTNAME%%.*}" "${PWD/#$HOME/\~}"')
      ;;
    screen*)
      PROMPT_COMMAND+=('printf "\033_%s@%s:%s\033\\" "${USER}" "${HOSTNAME%%.*}" "${PWD/#$HOME/\~}"')
      ;;
  esac
fi

if [[ -r /usr/share/bash-completion/bash_completion ]]; then
  . /usr/share/bash-completion/bash_completion
fi

export PATH="$PATH:/home/blix/.local/bin"

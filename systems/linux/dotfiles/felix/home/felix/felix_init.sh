bash /root/felix/buzzerd/init.sh
# do a tune EOF
# API is d# for delay and # for buzzer value but it needs to be in a single echo new line separated string
echo -e "d1000\n500\nd1000\n0" > /tmp
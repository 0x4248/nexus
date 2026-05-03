for i in {0..5}; do
	for j in {0..3}; do
		echo $((j*300)) > /dev/buzzer
		sleep 0.02
	done
done
echo 0 > /dev/buzzer

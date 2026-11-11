# tests/test_udp.sh
./receiver &          # start receiver in background
RECEIVER_PID=$!
sleep 0.5             # give it time to bind the socket
python3 tests/test_send.py
kill $RECEIVER_PID    # clean up
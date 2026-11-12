# Check if receiver.c successfully processes a UDP packet from sender.py.
./sonopsis & 
RECEIVER_PID=$! # hold (p)rocess (ID) of newly launched SONOPSIS exe
sleep 0.5 # wait for socket binding...
python3 ../scripts/test_send.py # build/ -> scripts/

# Clean up gracefully if receiver.c finishes before reaching this
kill $RECEIVER_PID 2>/dev/null || true 
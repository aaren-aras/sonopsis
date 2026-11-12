# Launch compiled process in background, hold its process ID,
# wait for socket binding, then run test_send.py.
./sonopsis & 
RECEIVER_PID=$!
sleep 0.5 
python3 ../scripts/test_send.py # from build/ -> scripts/

# Clean up gracefully if receiver.c finishes before reaching this
kill $RECEIVER_PID 2>/dev/null || true 
import socket
import struct
import time

import numpy as np

HOST = '127.0.0.1'
PORT = 5005

NUM_CHANNELS = 1
SAMPLES_PER_LINE = 1000  # match your pipeline_config.h

# build fake frame
timestamp_ns = int(time.time_ns())
channel_count = NUM_CHANNELS
samples_per_line = SAMPLES_PER_LINE

# header: uint64 + int32 + int32 = 16 bytes
header = struct.pack('<QII', timestamp_ns, channel_count, samples_per_line)

# fake RF data: sine wave at 5MHz sampled at 40MSPS
t = np.linspace(0, samples_per_line / 40e6, samples_per_line, dtype=np.float32)
rf_line = np.sin(2 * np.pi * 5e6 * t).astype(np.float32)
sample_data = np.tile(rf_line, NUM_CHANNELS)  # same line for all channels

payload = header + sample_data.tobytes()

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(payload, (HOST, PORT))
print(f'Sent {len(payload)} bytes')
print(f'Timestamp: {timestamp_ns} ns')
print(f'Expected samples: {NUM_CHANNELS * SAMPLES_PER_LINE}')
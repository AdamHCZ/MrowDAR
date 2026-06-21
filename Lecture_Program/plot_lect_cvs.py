import serial
import csv
import time
import matplotlib.pyplot as plt
from collections import deque

# ==========================
# User configuration
# ==========================

SERIAL_PORT = "COM5"      # Change this: COM3, COM4, COM5, etc.
BAUD_RATE = 115200
CSV_FILE = "Accelerometer_data.csv"

MAX_POINTS = 200          # Number of points shown on the live graph

# ==========================
# Data buffers
# ==========================

x_data = deque(maxlen=MAX_POINTS)
y_data = deque(maxlen=MAX_POINTS)
z_data = deque(maxlen=MAX_POINTS)
sample_data = deque(maxlen=MAX_POINTS)

sample_counter = 0

# ==========================
# Serial connection
# ==========================

ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
time.sleep(2)

print("Serial connection opened.")
print("Waiting for STM32 data...")

# ==========================
# CSV file
# ==========================

csv_file = open(CSV_FILE, "w", newline="")
csv_writer = csv.writer(csv_file)
csv_writer.writerow(["x_axis", "y_axis", "z_axis"])

# ==========================
# Plot configuration
# ==========================

plt.ion()
fig, ax = plt.subplots(figsize=(10, 6))

line_x, = ax.plot([], [], label="X axis")
line_y, = ax.plot([], [], label="Y axis")
line_z, = ax.plot([], [], label="Z axis")

ax.set_title("Live MPU6050 Accelerometer Data")
ax.set_xlabel("Sample number")
ax.set_ylabel("Raw acceleration value")
ax.grid(True)
ax.legend()

try:
    while True:
        raw_line = ser.readline().decode(errors="ignore").strip()

        if not raw_line:
            continue

        # Expected format: x,y,z
        parts = raw_line.replace(" ", "").split(",")

        if len(parts) != 3:
            print("Invalid data:", raw_line)
            continue

        try:
            x = float(parts[0])
            y = float(parts[1])
            z = float(parts[2])
        except ValueError:
            print("Conversion error:", raw_line)
            continue

        # Save to CSV
        csv_writer.writerow([x, y, z])
        csv_file.flush()

        # Store for plotting
        sample_counter += 1
        sample_data.append(sample_counter)
        x_data.append(x)
        y_data.append(y)
        z_data.append(z)

        # Update plot
        line_x.set_data(sample_data, x_data)
        line_y.set_data(sample_data, y_data)
        line_z.set_data(sample_data, z_data)

        ax.relim()
        ax.autoscale_view()

        plt.pause(0.01)

        #print(f"X: {x}, Y: {y}, Z: {z}")

except KeyboardInterrupt:                                                              
    print("\nProgram stopped by user.")

finally:
    ser.close()
    csv_file.close()
    print("Serial port closed.")
    print("CSV file saved.")
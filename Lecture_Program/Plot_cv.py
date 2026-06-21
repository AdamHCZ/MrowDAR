import csv
import matplotlib.pyplot as plt

CSV_FILE = "Accelerometer_data.csv"

x_values = []
y_values = []
z_values = []

with open(CSV_FILE, "r", newline="") as file:
    reader = csv.reader(file)

    header = next(reader, None)

    for row in reader:
        if len(row) < 3:
            continue

        try:
            x_values.append(float(row[0]))
            y_values.append(float(row[1]))
            z_values.append(float(row[2]))
        except ValueError:
            continue

samples = list(range(len(x_values)))

plt.figure(figsize=(10, 6))

plt.plot(samples, x_values, label="X axis")
plt.plot(samples, y_values, label="Y axis")
plt.plot(samples, z_values, label="Z axis")

plt.title("MPU6050 Accelerometer Data")
plt.xlabel("Sample number")
plt.ylabel("Raw acceleration value")
plt.legend()
plt.grid(True)

plt.show()
import serial
import time
import csv

# Configure the serial port
# Replace 'COM5'
SERIAL_PORT = 'COM5'  
BAUD_RATE = 115600      # Match the baud rate of your transmitting device

temp_list = [["x_axis","y_axis","z_axis"]]

try:
    # Open the port with a 1-second read timeout
    with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1) as ser:
        print(f"Connected to {SERIAL_PORT} at {BAUD_RATE} baud.")
        
        # Flush the buffer to clear any old unread data
        ser.reset_input_buffer()

        with open("Accelerometer_data.csv", "w", newline="", encoding="utf-8") as file:
            writer = csv.writer(file)
            writer.writerows(temp_list)
        
            while True:
                # Check if there is data waiting in the buffer
                if ser.in_waiting > 0:
                    # Read a full line (terminated by '\n')
                    raw_data = ser.readline()
                    
                    # Decode bytes to string and strip whitespace/newlines (\r\n)
                    decoded_data = raw_data.decode('utf-8', errors='ignore').rstrip()
                    temp_list = [decoded_data.split(",")]
                    writer.writerows(temp_list)
                    
                    print(f"{temp_list}")
                    
                time.sleep(0.01) # Small delay to prevent high CPU usage

except serial.SerialException as e:
    print(f"Error connecting to serial port: {e}")
except KeyboardInterrupt:
    print("\nProgram stopped by user.")

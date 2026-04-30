import socket
import matplotlib.pyplot as plt

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("0.0.0.0", 5000))
sock.settimeout(1.0)

print("Receiver Edge Mode... (CTRL+C to stop)")

raw_data = []
filtered_data = []
time_log = []

try:
    while True:
        try:
            data, addr = sock.recvfrom(1024)

            msg = data.decode().strip().split(",")

            if len(msg) != 3:
                continue

            raw = float(msg[0])
            filtered = float(msg[1])
            proc_time = float(msg[2]) / 1000  # us → ms

            raw_data.append(raw)
            filtered_data.append(filtered)
            time_log.append(proc_time)

            print(f"RAW: {raw:.2f} | EDGE: {filtered:.2f} | Time: {proc_time:.2f} ms")

        except socket.timeout:
            continue

except KeyboardInterrupt:
    print("\nStopping... plotting")

    sock.close()

    # ===== PLOT RAW vs FILTER =====
    plt.figure()
    plt.plot(raw_data, label="Raw")
    plt.plot(filtered_data, label="Filtered (Edge)")
    plt.legend()
    plt.title("Skenario B - Edge Filtering")
    plt.xlabel("Sample")
    plt.ylabel("Angle")

    # ===== PLOT TIME =====
    plt.figure()
    plt.plot(time_log)
    plt.title("Processing Time ESP32 (ms)")
    plt.xlabel("Sample")
    plt.ylabel("ms")

    plt.show()
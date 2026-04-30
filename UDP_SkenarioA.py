import socket
import numpy as np
import time
import matplotlib.pyplot as plt

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("0.0.0.0", 5000))
sock.settimeout(1.0)

print("Receiver Ready... (CTRL+C to stop)")

# ===== PARTICLE FILTER =====
NUM_PARTICLES = 1000

particles = None
weights = np.ones(NUM_PARTICLES) / NUM_PARTICLES

def init_particles(measurement):
    global particles
    particles = measurement + np.random.normal(0, 5, NUM_PARTICLES)

def particle_filter(measurement):
    global particles, weights

    # Prediction (lebih adaptif)
    particles += np.random.normal(0, 4.0, NUM_PARTICLES)

    # Update (lebih toleran)
    weights = np.exp(-0.05 * (particles - measurement)**2)
    weights += 1e-300
    weights /= np.sum(weights)

    # Resampling (SYSTEMATIC ? lebih cepat & stabil)
    cumulative_sum = np.cumsum(weights)
    step = 1.0 / NUM_PARTICLES
    start = np.random.uniform(0, step)
    points = start + step * np.arange(NUM_PARTICLES)

    indices = np.searchsorted(cumulative_sum, points)
    particles[:] = particles[indices]
    weights[:] = 1.0 / NUM_PARTICLES

    return np.mean(particles)

# ===== DATA =====
raw_data = []
filtered_data = []
time_log = []

initialized = False

try:
    while True:
        try:
            data, addr = sock.recvfrom(1024)
            measurement = float(data.decode())

            if not initialized:
                init_particles(measurement)
                initialized = True

            start = time.time()
            filtered = particle_filter(measurement)
            end = time.time()

            raw_data.append(measurement)
            filtered_data.append(filtered)
            time_log.append((end - start) * 1000)

            print(f"RAW: {measurement:.2f} | FILTERED: {filtered:.2f} | Time: {time_log[-1]:.2f} ms")

        except socket.timeout:
            continue

except KeyboardInterrupt:
    print("\nStopping...")

    sock.close()

    plt.figure()
    plt.plot(raw_data, label="Raw")
    plt.plot(filtered_data, label="Filtered (Server - Optimized)")
    plt.legend()
    plt.title("Skenario A - Optimized")

    plt.figure()
    plt.plot(time_log)
    plt.title("Processing Time (ms)")

    plt.show()
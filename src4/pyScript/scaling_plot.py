import matplotlib.pyplot as plt

# Sample data for Strong and Weak scaling
# Strong scaling times
strong_scaling_times = {
    64:   {1: 0.0641015, 2: 0.0434506, 4: 0.0625524, 8: 0.0853676, 16: 0.137435},
    128:  {1: 0.344177, 2: 0.206263, 4: 0.145251, 8: 0.144993, 16: 0.208141},
    256:  {1: 2.31123, 2: 1.22539, 4: 0.684252, 8: 0.454977, 16: 0.447215},
    512:  {1: 16.9584, 2: 8.66368, 4: 4.41961, 8: 2.467496, 16: 1.60822},
    1024: {1: 163.92, 2: 77.736, 4: 40.614, 8: 26.5026, 16: 21.4442}
}

# Weak scaling times
weak_scaling_times = {
    64:   {1: 0.0641015, 2: 0.102673, 4: 0.14174, 8: 0.239626, 16: 0.442806},
    128:  {1: 0.344177, 2: 0.463714, 4: 0.679039, 8: 1.02596, 16: 1.59415},
    256:  {1: 2.31123, 2: 3.18267, 4: 4.43313, 8: 10.0012, 16: 19.9354},
}

# Number of threads
threads = [1, 2, 4, 8, 16]

# Plotting Strong Scaling
plt.figure(figsize=(10, 6), dpi=100)

for resolution, time_data in sorted(strong_scaling_times.items()):
    time_list = [time_data[t] for t in threads]
    plt.plot(threads, time_list, marker='o', label=f'{resolution} x {resolution}')

plt.xlabel('Number of Threads')
plt.ylabel('Time to Solution (seconds)')
plt.title('Strong Scaling: Time vs Number of Threads for Different Resolutions')
plt.legend(title='Resolutions', loc='upper right')
plt.grid(True, linestyle='--', linewidth=0.5)
plt.xticks(threads)
plt.tight_layout()
plt.show()

# Plotting Weak Scaling
plt.figure(figsize=(10, 6), dpi=100)

for resolution, time_data in sorted(weak_scaling_times.items()):
    time_list = [time_data[t] for t in threads]
    plt.plot(threads, time_list, marker='o', label=f'{resolution} x {resolution}')

plt.xlabel('Number of Threads')
plt.ylabel('Time to Solution (seconds)')
plt.title('Weak Scaling: Time vs Number of Threads for Proportional Increment of Problem Size')
plt.legend(title='Base Resolutions', loc='upper left', bbox_to_anchor=(1, 1))
plt.grid(True, linestyle='--', linewidth=0.5)
plt.xticks(threads)
plt.tight_layout()
plt.show()

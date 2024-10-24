import matplotlib.pyplot as plt
import numpy as np

# Data provided
threads = [1, 2, 4, 8, 16, 20]
vector_lengths = [100000, 1000000, 10000000, 100000000, 1000000000]

reduction_times = {
    100000: [0.0117552, 0.00614834, 0.00339909, 0.00245894, 0.00281437, 0.0040203],
    1000000: [0.117108, 0.0588869, 0.0299629, 0.0159175, 0.0105182, 0.00896654],
    10000000: [1.36257, 0.699316, 0.644237, 0.483181, 0.538742, 0.525357],
    100000000: [13.616, 6.91513, 6.29939, 5.61593, 4.45651, 5.20869],
    1000000000: [133.917, 69.3902, 63.7676, 56.4091, 45.0795, 49.6325]
}

critical_times = {
    100000: [0.0117822, 0.00612854, 0.00342749, 0.00237419, 0.00269554, 0.00303355],
    1000000: [0.11747, 0.0586937, 0.0297113, 0.0155507, 0.00926112, 0.00837001],
    10000000: [1.36296, 0.698466, 0.643983, 0.568081, 0.540695, 0.644733],
    100000000: [13.5554, 6.91435, 6.29815, 5.61594, 4.46429, 5.24865],
    1000000000: [133.921, 69.4007, 63.7917, 56.4636, 44.9397, 49.7888]
}
plt.figure(figsize=(12, 8))

for vector_length in vector_lengths:
    plt.plot(threads, reduction_times[vector_length], label=f'Reduction (N=10^{int(np.log10(vector_length))})', marker='o')

for vector_length in vector_lengths:
    plt.plot(threads, critical_times[vector_length], label=f'Critical (N=10^{int(np.log10(vector_length))})', linestyle='--', marker='x')

plt.xscale('log', base=2)
plt.yscale('log')

plt.xlabel('Number of Threads (log scale)')
plt.ylabel('Execution Time (seconds, log scale)')
plt.title('Execution Time vs. Number of Threads for Different Vector Lengths')
plt.legend(fontsize='small', loc='upper right', bbox_to_anchor=(1.25, 1))
plt.grid(True, which="both", linestyle='--', linewidth=0.5)

plt.savefig("execution_time_vs_threads.pdf", bbox_inches='tight')

plt.show()

import matplotlib.pyplot as plt
import numpy as np
threads = [1, 2, 4, 8]
serial_times = [53.9293, 53.9291, 53.9331, 53.9288]
parallel_times = [53.9290, 27.0123, 13.5054, 6.7539]

speedup = [serial_times[i] / parallel_times[i] for i in range(len(threads))]

plt.figure(figsize=(10, 6))
plt.plot(threads, speedup, marker='o', linestyle='-', color='b', label='Speedup')

plt.xscale('log', base=2)
plt.yscale('log')
plt.xlabel('Number of Threads (log scale)')
plt.ylabel('Speedup (log scale)')
plt.title('Speedup vs Number of Threads for Approximating $\pi$')
plt.grid(True, which="both", linestyle='--', linewidth=0.5)
plt.legend()

plt.savefig('pi_speedup_plot.pdf', bbox_inches='tight')
plt.show()

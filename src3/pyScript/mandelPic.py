import matplotlib.pyplot as plt
import numpy as np

threads = [1, 2, 4, 8, 16, 20]
serial_times = [156.967, 156.970, 156.962, 156.968, 156.963, 156.979]
parallel_times = [314.236, 235.837, 233.083, 208.487, 186.563, 181.296]
speedup = [serial_times[0] / pt for pt in parallel_times]

parallel_efficiency = [s / t for s, t in zip(speedup, threads)]

plt.figure(figsize=(10, 5))
plt.plot(threads, speedup, marker='o', linestyle='-', color='b', label='Speedup')
plt.xscale('log', base=2)
plt.yscale('linear')
plt.xlabel('Number of Threads (log scale)')
plt.ylabel('Speedup')
plt.title('Speedup vs. Number of Threads for Mandelbrot Set Computation')
plt.grid(True, which="both", linestyle='--', linewidth=0.5)
plt.legend()
plt.savefig('speedup_plot.pdf', bbox_inches='tight')
plt.show()

plt.figure(figsize=(10, 5))
plt.plot(threads, parallel_efficiency, marker='o', linestyle='-', color='r', label='Parallel Efficiency')
plt.xscale('log', base=2)
plt.yscale('linear')
plt.xlabel('Number of Threads (log scale)')
plt.ylabel('Parallel Efficiency')
plt.title('Parallel Efficiency vs. Number of Threads for Mandelbrot Set Computation')
plt.grid(True, which="both", linestyle='--', linewidth=0.5)
plt.legend()
plt.savefig('parallel_efficiency_plot.pdf', bbox_inches='tight')
plt.show()
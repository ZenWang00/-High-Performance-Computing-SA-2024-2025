import matplotlib.pyplot as plt
import numpy as np

threads = [1, 2, 4, 8, 16, 32, 64, 128]
speedup = [1.00, 1.17, 1.83, 2.21, 2.23, 2.29, 2.31, 2.32]

plt.figure(figsize=(10, 6))

plt.plot(threads, speedup, marker='o', linestyle='-', color='b', label='Speedup')

plt.xlabel('Number of Threads', fontsize=14)
plt.ylabel('Speedup', fontsize=14)
plt.xscale('log', base=2) 
plt.grid(True, which="both", ls="--", linewidth=0.5)
plt.title('Speedup vs Number of Threads', fontsize=16)

plt.legend()
plt.tight_layout()
plt.savefig('histogram_speedup_plot.pdf')

plt.show()

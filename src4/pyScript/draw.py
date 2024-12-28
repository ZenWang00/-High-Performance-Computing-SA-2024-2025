import matplotlib.pyplot as plt

# 并行进程数和对应求解时间数据
procs = [2, 4, 8, 16]
times = [6.869531, 3.690769, 2.270762, 1.174711]

# 创建一个绘图窗口
plt.figure(figsize=(6,4))

# 在对数坐标下绘制 procs vs times 的曲线
plt.loglog(procs, times, marker='o', linestyle='-')

# 设置坐标轴和标题
plt.xlabel('Number of Processes (log scale)')
plt.ylabel('Total Runtime (seconds, log scale)')
plt.title('PETSc Scalability for 1024x1024 Grid (CG Solver)')

# 添加网格线
plt.grid(True, which="both", ls="--")

# 将图像保存为文件，也可以调整文件名和格式
plt.savefig("scalability_plot.png", dpi=300, bbox_inches='tight')

# 显示图像（可选）
plt.show()

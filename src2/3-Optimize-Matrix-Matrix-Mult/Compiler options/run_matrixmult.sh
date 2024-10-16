#!/bin/bash
#SBATCH --job-name=matrix_mult_test      # 作业名称
#SBATCH --output=matrix_mult_test-%j.out # 输出文件 (默认：slurm-%j.out)
#SBATCH --error=matrix_mult_test-%j.err  # 错误文件
#SBATCH --ntasks=1                       # 任务数
#SBATCH --cpus-per-task=1                # 每个任务的 CPU 数
#SBATCH --time=00:30:00                  # 最大运行时间
#SBATCH --partition=slim                 # 使用的分区

# 加载必要的模块
module load gcc
export OMP_NUM_THREADS=1

# 编译所有版本
make all

# set some environment variables

echo "==== benchmark-naive -O0 ===================="
srun ./benchmark-naive-O0 | tee timing_O0.data
echo

echo "==== benchmark-naive -O1 ===================="
srun ./benchmark-naive-O1 | tee timing_O1.data
echo

echo "==== benchmark-naive -O2 ===================="
srun ./benchmark-naive-O2 | tee timing_O2.data
echo

echo "==== benchmark-naive -O3 ===================="
srun ./benchmark-naive-O3 | tee timing_O3.data
echo

echo "==== benchmark-blocked -O3 =================="
srun ./benchmark-blocked-O3 | tee timing_blocked_dgemm.data
echo

echo "==== benchmark-blas ========================="
srun ./benchmark-blas | tee timing_blas_dgemm.data
echo

echo
echo "==== plot results ========================="
gnuplot timing.gp

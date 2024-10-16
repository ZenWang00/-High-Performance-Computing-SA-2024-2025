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


echo "==== benchmark-naive-unrolled ======================="
srun ./benchmark-naive-unrolled | tee timing_naive_unrolled.data
echo

echo "==== benchmark-blocked-unrolled ====================="
srun ./benchmark-blocked-unrolled | tee timing_blocked_unrolled.data
echo

echo "==== benchmark-blas-unrolled ========================"
srun ./benchmark-blas-unrolled | tee timing_blas_unrolled.data
echo

echo "==== benchmark-blas (original) ======================"
srun ./benchmark-blas | tee timing_blas_dgemm.data
echo

echo
echo "==== plot results ========================="
gnuplot timing.gp

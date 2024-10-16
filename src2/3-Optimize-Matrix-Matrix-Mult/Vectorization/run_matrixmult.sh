#!/bin/bash
#SBATCH --job-name=matrixmult
#SBATCH --output=matrixmult-%j.out
#SBATCH --error=matrixmult-%j.err
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --partition=slim
#SBATCH --time=00:20:00

# load some modules & list loaded modules
module list

# set some environment variables
export OMP_NUM_THREADS=1

echo "==== benchmark-naive-vectorized ====================="
srun ./benchmark-naive-vectorized | tee timing_naive_vectorized.data
echo

echo "==== benchmark-blocked-vectorized ==================="
srun ./benchmark-blocked-vectorized | tee timing_blocked_vectorized.data
echo

echo "==== benchmark-blas-vectorized ======================"
srun ./benchmark-blas-vectorized | tee timing_blas_vectorized.data
echo

echo "==== benchmark-blas (original) ======================"
srun ./benchmark-blas | tee timing_blas_dgemm.data
echo

echo
echo "==== plot results ========================="
gnuplot timing.gp

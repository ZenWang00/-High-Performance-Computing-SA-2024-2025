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

echo "==== benchmark-naive-pragma =================="
srun ./benchmark-naive-pragma | tee timing_pragma_dgemm.data
echo

echo "==== benchmark-blocked-pragma ================"
srun ./benchmark-blocked-pragma | tee timing_blocked_pragma.data
echo

echo "==== benchmark-blas ========================"
srun ./benchmark-blas | tee timing_blas_dgemm.data
echo

echo
echo "==== plot results ========================="
gnuplot timing.gp

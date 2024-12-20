#!/bin/bash

# This script runs strong and weak scaling tests for the 2D diffusion problem.
# It runs the program with different numbers of processors and different problem sizes.

# Load necessary modules (if required for your environment)
# module load mpi

# Compile the code using Makefile
make

# Strong Scaling and Weak Scaling Test Parameters
strong_problem_sizes=(64 128 256 512 1024)
weak_base_size=64
processors=(1 2 4 8 16)
total_time=0.05
num_time_steps=100

# Directory for output files
mkdir -p scaling_results

# CSV file for combined scaling results
scaling_csv="scaling_results/scaling_results.csv"
echo "Test Type,Problem Size,Processors,Time (ms)" > $scaling_csv

# Strong Scaling Test
for n in "${strong_problem_sizes[@]}"; do
  for p in "${processors[@]}"; do
    echo "Running strong scaling test with problem size ${n}x${n} on ${p} processors..."
    mpirun -np $p ./main $n $num_time_steps $total_time > scaling_results/strong_${n}x${n}_p${p}.txt 2>&1
    elapsed_time=$(grep "Total time spent" scaling_results/strong_${n}x${n}_p${p}.txt | awk '{print $4}')
    echo "Strong,${n}x${n},${p},${elapsed_time}" >> $scaling_csv
  done
done

# Weak Scaling Test
for p in "${processors[@]}"; do
  # Scale problem size proportionally to the number of processors
  n=$((weak_base_size * p))
  echo "Running weak scaling test with problem size ${n}x${n} on ${p} processors..."
  mpirun -np $p ./main $n $num_time_steps $total_time > scaling_results/weak_${n}x${n}_p${p}.txt 2>&1
  elapsed_time=$(grep "Total time spent" scaling_results/weak_${n}x${n}_p${p}.txt | awk '{print $4}')
  echo "Weak,${n}x${n},${p},${elapsed_time}" >> $scaling_csv
done

# Summary
echo "Strong and weak scaling tests completed. Results are saved in scaling_results directory and the combined CSV file."

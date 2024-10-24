#!/bin/bash

VECTOR_LENGTHS=(100000 1000000 10000000 100000000 1000000000)
THREAD_COUNTS=(1 2 4 8 16 20)

g++ -O3 -fopenmp dotProduct.cpp -o dotProduct
if [ $? -ne 0 ]; then
  echo "Compilation failed!"
  exit 1
fi

for N in "${VECTOR_LENGTHS[@]}"; do
    
    FILENAME="experiment_results_${N}.csv"
    echo "Thread_Count,Method,Execution_Time" > $FILENAME
    for T in "${THREAD_COUNTS[@]}"; do
        export OMP_NUM_THREADS=$T
        echo "Running with Vector Length = $N and Threads = $T"

        ./dotProduct $N $T

        if [ $? -ne 0 ]; then
          echo "Execution failed for Vector Length = $N and Threads = $T!"
          exit 1
        fi
    done
done

echo "All experiments completed successfully."

#!/bin/bash

make CC=gcc CFLAGS="-O3 -fopenmp" LDFLAGS="-O3 -fopenmp"
if [ $? -ne 0 ]; then
  echo "Compilation failed!"
  exit 1
fi

THREAD_COUNTS=(1 2 4 8 16 20)

for T in "${THREAD_COUNTS[@]}"; do
  export OMP_NUM_THREADS=$T
  echo "Running with $T thread(s):"
  ./mandel_seq
  if [ $? -ne 0 ]; then
    echo "Execution failed for $T thread(s)!"
    exit 1
  fi
  echo ""
done

echo "All experiments completed successfully."

set title "nxn matrix multiplication on single-core Intel Xeon E5-2650"
set xlabel "Matrix size (n)"
set ylabel "Performance (GFlops/s)"
set grid
set logscale y 10
set yrange [1:*]

set terminal pdf
set output "timing.pdf"

plot "timing_basic_dgemm.data" using 2:4 title "Naive dgemm" with linespoints pointtype 7 pointsize 1 linewidth 2 linetype 1 linecolor rgb "red", \
     "timing_naive_tune.data" using 2:4 title "Naive dgemm Tuned" with linespoints pointtype 5 pointsize 1 linewidth 2 linetype 2 linecolor rgb "blue", \
     "timing_blocked_dgemm.data" using 2:4 title "Blocked dgemm" with linespoints pointtype 9 pointsize 1 linewidth 2 linetype 3 linecolor rgb "green", \
     "timing_blas_dgemm.data" using 2:4 title "Ref. BLAS dgemm" with linespoints pointtype 3 pointsize 1 linewidth 2 linetype 4 linecolor rgb "black"
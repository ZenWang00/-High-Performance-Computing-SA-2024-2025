set title "nxn matrix multiplication on single-core Intel Xeon E5-2650"
set xlabel "Matrix size (n)"
set ylabel "Performance (GFlops/s)"
set grid
set logscale y 10
set yrange [1:*]

set terminal pdf
set output "timing.pdf"

plot "timing_naive_vectorized.data" using 2:4 title "Naive dgemm Vectorized" with linespoints pointtype 6 pointsize 1 linewidth 2 linetype 1 linecolor rgb "red", \
     "timing_blocked_vectorized.data" using 2:4 title "Blocked dgemm Vectorized" with linespoints pointtype 5 pointsize 1 linewidth 2 linetype 2 linecolor rgb "blue", \
     "timing_blas_vectorized.data" using 2:4 title "OpenBLAS Vectorized" with linespoints pointtype 7 pointsize 1 linewidth 2 linetype 3 linecolor rgb "purple", \
     "timing_blas_dgemm.data" using 2:4 title "Ref. BLAS dgemm (original)" with linespoints pointtype 3 pointsize 1 linewidth 2 linetype 4 linecolor rgb "black"

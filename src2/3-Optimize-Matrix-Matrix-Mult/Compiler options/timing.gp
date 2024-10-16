set title "nxn matrix multiplication on single-core Intel Xeon E5-2650"
set xlabel "Matrix size (n)"
set ylabel "Performance (GFlops/s)"
set grid
set logscale y 10
set yrange [1:*]

set terminal pdf
set output "timing.pdf"

plot "timing_basic_dgemm.data" using 2:4 title "Naive dgemm -O0" with linespoints pointtype 7 pointsize 2 linewidth 3 linetype 1 linecolor rgb "red", \
     "timing_blocked_dgemm.data" using 2:4 title "Blocked dgemm -O3" with linespoints pointtype 5 pointsize 2 linewidth 3 linetype 2 linecolor rgb "blue", \
     "timing_blas_dgemm.data" using 2:4 title "Ref. BLAS dgemm" with linespoints pointtype 9 pointsize 2 linewidth 3 linetype 3 linecolor rgb "green", \
     "timing_O0.data" using 2:4 title "Naive dgemm -O0" with linespoints pointtype 4 pointsize 2 linewidth 3 linetype 4 linecolor rgb "magenta", \
     "timing_O1.data" using 2:4 title "Naive dgemm -O1" with linespoints pointtype 6 pointsize 2 linewidth 3 linetype 5 linecolor rgb "cyan", \
     "timing_O2.data" using 2:4 title "Naive dgemm -O2" with linespoints pointtype 8 pointsize 2 linewidth 3 linetype 6 linecolor rgb "orange", \
     "timing_O3.data" using 2:4 title "Naive dgemm -O3" with linespoints pointtype 3 pointsize 2 linewidth 3 linetype 7 linecolor rgb "black"
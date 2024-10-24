#include <stdio.h>
#include <stdlib.h>

#include "consts.h"
#include "pngwriter.h"
#include "walltime.h"
void print_benchmark(const char* version, double time, long nTotalIterationsCount) {
    printf("\n%s version:\n", version);
    printf("Total time:                 %g seconds\n", time);
    printf("Image size:                 %ld x %ld = %ld Pixels\n",
           (long)IMAGE_WIDTH, (long)IMAGE_HEIGHT,
           (long)(IMAGE_WIDTH * IMAGE_HEIGHT));
    printf("Total number of iterations: %ld\n", nTotalIterationsCount);
    printf("Avg. time per pixel:        %g seconds\n",
           time / (double)(IMAGE_WIDTH * IMAGE_HEIGHT));
    printf("Avg. time per iteration:    %g seconds\n",
           time / (double)nTotalIterationsCount);
    printf("Iterations/second:          %g\n",
           nTotalIterationsCount / time);
    printf("MFlop/s:                    %g\n",
           nTotalIterationsCount * 8.0 / time * 1.e-6);
}

int main(int argc, char **argv) {
  png_data *pPng = png_create(IMAGE_WIDTH, IMAGE_HEIGHT);

  double x, y, x2, y2, cx, cy;

  double fDeltaX = (MAX_X - MIN_X) / (double)IMAGE_WIDTH;
  double fDeltaY = (MAX_Y - MIN_Y) / (double)IMAGE_HEIGHT;

  long nTotalIterationsCount = 0;

  long i, j;

  double time_start = walltime();
  // do the calculation
  cy = MIN_Y;
  for (j = 0; j < IMAGE_HEIGHT; j++) {
    cx = MIN_X;
    for (i = 0; i < IMAGE_WIDTH; i++) {
      x = cx;
      y = cy;
      x2 = x * x;
      y2 = y * y;
      // compute the orbit z, f(z), f^2(z), f^3(z), ...
      // count the iterations until the orbit leaves the circle |z|=2.
      // stop if the number of iterations exceeds the bound MAX_ITERS.
      int n = 0;
      // TODO
      // >>>>>>>> CODE IS MISSING
      while ((x2 + y2 <= 4.0) && (n < MAX_ITERS)) {
       y = 2 * x * y + cy;
       x = x2 - y2 + cx;
       x2 = x * x;
       y2 = y * y;
       n++;
       nTotalIterationsCount++;
       }
      // <<<<<<<< CODE IS MISSING
      // n indicates if the point belongs to the mandelbrot set
      // plot the number of iterations at point (i, j)
      int c = ((long)n * 255) / MAX_ITERS;
      png_plot(pPng, i, j, c, c, c);
      cx += fDeltaX;
    }
    cy += fDeltaY;
  }

  double time_end = walltime();
  double serial_time = time_end - time_start;

  print_benchmark("Serial", serial_time, nTotalIterationsCount);

  #pragma omp parallel for private(x, y, x2, y2, cx, cy) reduction(+:nTotalIterationsCount)
    for (long j = 0; j < IMAGE_HEIGHT; j++) {
        cy = MIN_Y + j * fDeltaY;
        for (long i = 0; i < IMAGE_WIDTH; i++) {
            cx = MIN_X + i * fDeltaX;
            x = cx;
            y = cy;
            x2 = x * x;
            y2 = y * y;
            int n = 0;
            while ((x2 + y2 <= 4.0) && (n < MAX_ITERS)) {
                y = 2 * x * y + cy;
                x = x2 - y2 + cx;
                x2 = x * x;
                y2 = y * y;
                n++;
                nTotalIterationsCount++;
            }

            int c = ((long)n * 255) / MAX_ITERS;
            #pragma omp critical
            png_plot(pPng, i, j, c, c, c);
        }
    }

    time_end = walltime();
    double parallel_time = time_end - time_start;

    print_benchmark("Parallel", parallel_time, nTotalIterationsCount);

    double speedup = serial_time / parallel_time;
    printf("\nSpeedup: %g\n", speedup);


  png_write(pPng, "mandel.png");
  return 0;
}

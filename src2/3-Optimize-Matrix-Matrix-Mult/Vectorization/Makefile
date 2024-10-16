# Some possible configs ########################################################

# Rosa GCC config (module load gcc)
CC = gcc                         # compiler (& linker)
OPT = -O3 -march=native \
      -funroll-loops    \
      -ftree-vectorize           # original optimization flags

# Vectorization specific optimization flags
OPT_VECTOR = -O3 -march=native \
             -ftree-vectorize  \
             -funroll-loops    # optimization flags for vectorization

CFLAGS = -Wall -std=gnu99 $(OPT) # standard compiler flags
CFLAGS_VECTOR = -Wall -std=gnu99 $(OPT_VECTOR) # standard compiler flags with vectorization optimizations

#Rosa INTEL config (module load intel)
# CC = icx                           # compiler (& linker)
# OPT = -O3 -march=core-avx2         # optimization flags (you may add more)
# CFLAGS = -Wall -std=gnu99 $(OPT)   # standard compiler flags
# See the Intel oneAPI Math Kernel Library Link Line Advisor
# https://www.intel.com/content/www/us/en/developer/tools/oneapi/onemkl-link-line-advisor.html#gs.5jekhw
CFLAGS += -I"${MKLROOT}/include"  # more header files
LDLIBS = -L${MKLROOT}/lib/intel64 \
         -lmkl_intel_lp64         \
         -lmkl_sequential         \
         -lmkl_core               \
         -lpthread -lm -ldl       # linker options

################################################################################

targets = benchmark-naive-vectorized benchmark-blocked-vectorized benchmark-blas-vectorized benchmark-blas
objects = benchmark.o dgemm-naive.o dgemm-blocked.o dgemm-blas.o
results = timing_naive_vectorized.data \
          timing_blocked_vectorized.data \
          timing_blas_vectorized.data \
          timing_blas_dgemm.data \
          timing.pdf

.PHONY: default
default: all

.PHONY: all
all: $(targets)

# Compile different versions with vectorization optimization
benchmark-naive-vectorized: benchmark.o dgemm-naive.o
	$(CC) -o $@ $^ $(CFLAGS_VECTOR) $(LDLIBS)

benchmark-blocked-vectorized: benchmark.o dgemm-blocked.o
	$(CC) -o $@ $^ $(CFLAGS_VECTOR) $(LDLIBS)

benchmark-blas-vectorized: benchmark.o dgemm-blas.o
	$(CC) -o $@ $^ $(CFLAGS_VECTOR) $(LDLIBS)

benchmark-blas: benchmark.o dgemm-blas.o
	$(CC) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

.PHONY: clean
clean:
	$(RM) $(targets) $(objects)

.PHONY: cleanall
cleanall: clean
	$(RM) $(results)

# Some possible configs ########################################################

# Rosa GCC config (module load gcc)
CC = gcc                         # compiler (& linker)
OPT_O0 = -O0 -march=native       # No optimization, useful as a baseline
OPT_O1 = -O1 -march=native       # Moderate optimization level 1
OPT_O2 = -O2 -march=native       # Higher optimization level 2
OPT_O3 = -O3 -march=native       \
         -funroll-loops          \
         -ftree-vectorize        # Highest level optimization, plus extra flags for performance

CFLAGS_O0 = -Wall -std=gnu99 $(OPT_O0) # standard compiler flags with no optimization
CFLAGS_O1 = -Wall -std=gnu99 $(OPT_O1) # standard compiler flags with level 1 optimization
CFLAGS_O2 = -Wall -std=gnu99 $(OPT_O2) # standard compiler flags with level 2 optimization
CFLAGS_O3 = -Wall -std=gnu99 $(OPT_O3) # standard compiler flags with level 3 optimization and additional flags

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

# Targets and object files
targets = benchmark-naive-O0 benchmark-naive-O1 benchmark-naive-O2 benchmark-naive-O3 \
          benchmark-blocked-O3 benchmark-blas
objects = benchmark.o dgemm-naive.o dgemm-blocked.o dgemm-blas.o
results = timing_basic_dgemm.data   \
          timing_blas_dgemm.data    \
          timing_blocked_dgemm.data \
          timing.pdf

.PHONY: default
default: all

.PHONY: all
all: $(targets)

# Compile different versions with varying optimization levels
benchmark-naive-O0: benchmark.o dgemm-naive.o
	$(CC) -o $@ $^ $(CFLAGS_O0) $(LDLIBS)

benchmark-naive-O1: benchmark.o dgemm-naive.o
	$(CC) -o $@ $^ $(CFLAGS_O1) $(LDLIBS)

benchmark-naive-O2: benchmark.o dgemm-naive.o
	$(CC) -o $@ $^ $(CFLAGS_O2) $(LDLIBS)

benchmark-naive-O3: benchmark.o dgemm-naive.o
	$(CC) -o $@ $^ $(CFLAGS_O3) $(LDLIBS)

benchmark-blocked-O3: benchmark.o dgemm-blocked.o
	$(CC) -o $@ $^ $(CFLAGS_O3) $(LDLIBS)

benchmark-blas: benchmark.o dgemm-blas.o
	$(CC) -o $@ $^ $(LDLIBS)

# Pattern rule to compile .c files to .o files
%.o: %.c
	$(CC) -c $(CFLAGS_O3) $<

.PHONY: clean
clean:
	$(RM) $(targets) $(objects)

.PHONY: cleanall
cleanall: clean
	$(RM) $(results)
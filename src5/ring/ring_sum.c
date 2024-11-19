#include <mpi.h> // MPI
#include <stdio.h>

int main(int argc, char *argv[]) {

  // Initialize MPI, get size and rank
  int size, rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // IMPLEMENT: Ring sum algorithm
  int sum = rank; // initialize sum
  int rank_send = rank;
  int rank_recive;
  int send, receive;

  for (int i = 0; i < size - 1; i++){
    send = (rank + 1) % size;
    receive = (rank - 1 + size) % size;

    MPI_Sendrecv(
    &rank_send, 1, MPI_INT, send, 0, 
    &rank_recive, 1, MPI_INT, receive, 0, 
    MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  sum += rank_recive;
  rank_send = rank_recive;
  }
  printf("Process %i: Sum = %i\n", rank, sum);

  // Finalize MPI
  MPI_Finalize();

  return 0;
}

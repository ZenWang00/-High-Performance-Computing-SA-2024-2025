/****************************************************************
 *                                                              *
 * This file has been written as a sample solution to an        *
 * exercise in a course given at the CSCS-USI Summer School     *
 * It is made freely available with the understanding that      *
 * every copy of this file must include this header and that    *
 * CSCS/USI take no responsibility for the use of the enclosed  *
 * teaching material.                                           *
 *                                                              *
 * Purpose: Exchange ghost cell in 2 directions using a topology*
 *                                                              *
 * Contents: C-Source                                           *
 *                                                              *
 ****************************************************************/

/* Use only 16 processes for this exercise
 * Send the ghost cell in two directions: left<->right and top<->bottom
 * ranks are connected in a cyclic manner, for instance, rank 0 and 12 are connected
 *
 * process decomposition on 4*4 grid
 *
 * |-----------|
 * | 0| 1| 2| 3|
 * |-----------|
 * | 4| 5| 6| 7|
 * |-----------|
 * | 8| 9|10|11|
 * |-----------|
 * |12|13|14|15|
 * |-----------|
 *
 * Each process works on a 6*6 (SUBDOMAIN) block of data
 * the D corresponds to data, g corresponds to "ghost cells"
 * xggggggggggx
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * xggggggggggx
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SUBDOMAIN 6
#define DOMAINSIZE (SUBDOMAIN+2)

int main(int argc, char *argv[])
{
    int rank, size, i, j, dims[2], periods[2], rank_top, rank_bottom, rank_left, rank_right;
    double data[DOMAINSIZE*DOMAINSIZE];
    MPI_Request request;
    MPI_Status status;
    MPI_Comm comm_cart;
    MPI_Datatype data_ghost;

    // Initialize MPI
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size!=16) {
        printf("please run this with 16 processors\n");
        MPI_Finalize();
        exit(1);
    }

    // initialize the domain
    for (i=0; i<DOMAINSIZE*DOMAINSIZE; i++) {
        data[i]=rank;
    }
    // TODO: set the dimensions of the processor grid and periodic boundaries in both dimensions
    dims[0]= 4;
    dims[1]= 4;
    periods[0]= 1;
    periods[1]= 1;

    // TODO: Create a Cartesian communicator (4*4) with periodic boundaries (we do not allow
    // the reordering of ranks) and use it to find your neighboring
    // ranks in all dimensions in a cyclic manner.
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &comm_cart);
    
    
    // TODO: find your top/bottom/left/right neighbor using the new communicator, see MPI_Cart_shift()
    // rank_top, rank_bottom
    // rank_left, rank_right
    MPI_Cart_shift(comm_cart, 0, 1, &rank_top, &rank_bottom);
    MPI_Cart_shift(comm_cart, 1, 1, &rank_left, &rank_right);
    //part:1 start
    //  TODO: create derived datatype data_ghost, create a datatype for sending the column, see MPI_Type_vector() and MPI_Type_commit()
    // data_ghost
    MPI_Type_vector(SUBDOMAIN, 1, DOMAINSIZE, MPI_DOUBLE, &data_ghost);
    MPI_Type_commit(&data_ghost);

    //  TODO: ghost cell exchange with the neighbouring cells in all directions
    // part2
    MPI_Irecv(&data[1], DOMAINSIZE - 2, MPI_DOUBLE, rank_top, 0, comm_cart, &request);
    MPI_Send(&data[DOMAINSIZE + 1], DOMAINSIZE - 2, MPI_DOUBLE, rank_bottom, 0, comm_cart);
    MPI_Wait(&request, &status);

    // // to the bottom
    MPI_Irecv(&data[DOMAINSIZE * (DOMAINSIZE - 1) + 1], DOMAINSIZE - 2, MPI_DOUBLE, rank_bottom, 1, comm_cart, &request);
    MPI_Send(&data[DOMAINSIZE * (DOMAINSIZE - 2) + 1], DOMAINSIZE - 2, MPI_DOUBLE, rank_top, 1, comm_cart);
    MPI_Wait(&request, &status);

    MPI_Datatype data_ghost_column;
    MPI_Type_vector(SUBDOMAIN, 1, DOMAINSIZE, MPI_DOUBLE, &data_ghost_column);
    MPI_Type_commit(&data_ghost_column);

    // // to the left
    MPI_Irecv(&data[DOMAINSIZE], 1, data_ghost_column, rank_left, 2, comm_cart, &request);
    MPI_Send(&data[DOMAINSIZE + 1], 1, data_ghost_column, rank_right, 2, comm_cart);
    MPI_Wait(&request, &status);

    // // to the right
    MPI_Irecv(&data[2 * DOMAINSIZE - 1], 1, data_ghost_column, rank_right, 3, comm_cart, &request);
    MPI_Send(&data[2 * DOMAINSIZE - 2], 1, data_ghost_column, rank_left, 3, comm_cart);
    MPI_Wait(&request, &status);    


    //Bonus [10 Points]: Also exchange ghost values with the neighbors in ordinal directions (northeast, southeast, southwest and northwest).
    int coords[2]; 
    int coords_nw[2], coords_ne[2], coords_sw[2], coords_se[2];
    int rank_northwest, rank_northeast, rank_southwest, rank_southeast; 

    MPI_Cart_coords(comm_cart, rank, 2, coords);

    coords_nw[0] = (coords[0] - 1 + dims[0]) % dims[0];
    coords_nw[1] = (coords[1] - 1 + dims[1]) % dims[1];
    MPI_Cart_rank(comm_cart, coords_nw, &rank_northwest);

    coords_ne[0] = (coords[0] - 1 + dims[0]) % dims[0];
    coords_ne[1] = (coords[1] + 1) % dims[1];
    MPI_Cart_rank(comm_cart, coords_ne, &rank_northeast);

    coords_sw[0] = (coords[0] + 1) % dims[0];
    coords_sw[1] = (coords[1] - 1 + dims[1]) % dims[1];
    MPI_Cart_rank(comm_cart, coords_sw, &rank_southwest);

    coords_se[0] = (coords[0] + 1) % dims[0];
    coords_se[1] = (coords[1] + 1) % dims[1];
    MPI_Cart_rank(comm_cart, coords_se, &rank_southeast);


    MPI_Sendrecv(&data[DOMAINSIZE + 1], 1, MPI_DOUBLE, rank_southeast, 4,
                &data[0], 1, MPI_DOUBLE, rank_northwest, 4,
                comm_cart, &status);

    MPI_Sendrecv(&data[DOMAINSIZE + DOMAINSIZE -2], 1, MPI_DOUBLE, rank_southwest, 5,
                &data[DOMAINSIZE -1], 1, MPI_DOUBLE, rank_northeast, 5,
                comm_cart, &status);

    MPI_Sendrecv(&data[(DOMAINSIZE -2) * DOMAINSIZE +1], 1, MPI_DOUBLE, rank_northeast, 6,
                &data[DOMAINSIZE * (DOMAINSIZE -1)], 1, MPI_DOUBLE, rank_southwest, 6,
                comm_cart, &status);


    MPI_Sendrecv(&data[(DOMAINSIZE -2) * DOMAINSIZE + DOMAINSIZE -2], 1, MPI_DOUBLE, rank_northwest, 7,
                &data[DOMAINSIZE * DOMAINSIZE -1], 1, MPI_DOUBLE, rank_southeast, 7,
                comm_cart, &status);
    //part2

    if (rank==9) {
        printf("data of rank 9 after communication\n");
        for (j=0; j<DOMAINSIZE; j++) {
            for (i=0; i<DOMAINSIZE; i++) {
                printf("%4.1f ", data[i+j*DOMAINSIZE]);
            }
            printf("\n");
        }
    }

    // Free MPI resources (e.g., types and communicators)
    // TODO
    MPI_Type_free(&data_ghost);
    MPI_Comm_free(&comm_cart);
    // Finalize MPI
    MPI_Finalize();

    return 0;
}

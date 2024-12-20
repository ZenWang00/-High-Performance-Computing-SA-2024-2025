//******************************************
// operators.cpp
// based on min-app code written by Oliver Fuhrer, MeteoSwiss
// modified by Ben Cumming, CSCS
// *****************************************

// Description: Contains simple operators which can be used on 2/3d-meshes
#include <mpi.h>
#include "data.h"
#include "operators.h"
#include "stats.h"
#include <iostream>

namespace operators {

// compute the diffusion-reaction stencils
// s_old is the population concentration at time step k-1, s_new at k,
// and f is the residual (see Eq. (7) in Project 3).
void diffusion(data::Field const& s_old, data::Field const& s_new,
               data::Field& f) {
    using data::options;
    using data::domain;

    using data::bndE;
    using data::bndW;
    using data::bndN;
    using data::bndS;

    using data::buffE;
    using data::buffW;
    using data::buffN;
    using data::buffS;

    double alpha = options.alpha;
    double beta = options.beta;

    int nx = domain.nx;
    int ny = domain.ny;
    int iend  = nx - 1;
    int jend  = ny - 1;

    MPI_Comm comm_cart = domain.comm_cart; // 修复未声明的 comm_cart
    MPI_Status statuses[8]; // 用于存储通信的状态
    MPI_Request requests[8]; // 用于存储通信的请求
    int req_count = 0; // 计数器

    // Start north and south communication
    if (domain.neighbour_north != MPI_PROC_NULL) {
        // Receive from north neighbor
        MPI_Irecv(&bndN[0], nx, MPI_DOUBLE, domain.neighbour_north, 0, comm_cart, &requests[req_count++]);

        // Pack north buffer and send to north neighbor
        for (int i = 0; i < nx; i++) {
            buffN[i] = s_new(i, ny - 1);
        }
        MPI_Isend(&buffN[0], nx, MPI_DOUBLE, domain.neighbour_north, 1, comm_cart, &requests[req_count++]);
        std::cout << "Rank " << domain.rank << ": Sending data to north neighbor " << domain.neighbour_north << std::endl;
        std::cout << "Rank " << domain.rank << ": Receiving data from north neighbor " << domain.neighbour_north << std::endl;
    }

    if (domain.neighbour_south != MPI_PROC_NULL) {
        // Receive from south neighbor
        MPI_Irecv(&bndS[0], nx, MPI_DOUBLE, domain.neighbour_south, 2, comm_cart, &requests[req_count++]);

        // Pack south buffer and send to south neighbor
        for (int i = 0; i < nx; i++) {
            buffS[i] = s_new(i, 0);
        }
        MPI_Isend(&buffS[0], nx, MPI_DOUBLE, domain.neighbour_south, 3, comm_cart, &requests[req_count++]);
        std::cout << "Rank " << domain.rank << ": Sending data to south neighbor " << domain.neighbour_south << std::endl;
        std::cout << "Rank " << domain.rank << ": Receiving data from south neighbor " << domain.neighbour_south << std::endl;
    }

    // Wait for north-south communication to complete
    if (req_count > 0) {
        MPI_Waitall(req_count, requests, statuses);
        std::cout << "Rank " << domain.rank << ": Completed north-south communication" << std::endl;
    }

    // Reset request count for east-west communication
    req_count = 0;

    // Start east and west communication
    if (domain.neighbour_east != MPI_PROC_NULL) {
        // Receive from east neighbor
        MPI_Irecv(&bndE[0], ny, MPI_DOUBLE, domain.neighbour_east, 4, comm_cart, &requests[req_count++]);

        // Pack east buffer and send to east neighbor
        for (int j = 0; j < ny; j++) {
            buffE[j] = s_new(nx - 1, j);
        }
        MPI_Isend(&buffE[0], ny, MPI_DOUBLE, domain.neighbour_east, 5, comm_cart, &requests[req_count++]);
        std::cout << "Rank " << domain.rank << ": Sending data to east neighbor " << domain.neighbour_east << std::endl;
        std::cout << "Rank " << domain.rank << ": Receiving data from east neighbor " << domain.neighbour_east << std::endl;
    }

    if (domain.neighbour_west != MPI_PROC_NULL) {
        // Receive from west neighbor
        MPI_Irecv(&bndW[0], ny, MPI_DOUBLE, domain.neighbour_west, 6, comm_cart, &requests[req_count++]);

        // Pack west buffer and send to west neighbor
        for (int j = 0; j < ny; j++) {
            buffW[j] = s_new(0, j);
        }
        MPI_Isend(&buffW[0], ny, MPI_DOUBLE, domain.neighbour_west, 7, comm_cart, &requests[req_count++]);
        std::cout << "Rank " << domain.rank << ": Sending data to west neighbor " << domain.neighbour_west << std::endl;
        std::cout << "Rank " << domain.rank << ": Receiving data from west neighbor " << domain.neighbour_west << std::endl;
    }

    // Wait for east-west communication to complete
    if (req_count > 0) {
        MPI_Waitall(req_count, requests, statuses);
        std::cout << "Rank " << domain.rank << ": Completed east-west communication" << std::endl;
    }

    // Compute interior grid points
    for (int j = 1; j < jend; j++) {
        for (int i = 1; i < iend; i++) {
            f(i, j) = -(4. + alpha) * s_new(i, j)     // central point
                    + s_new(i - 1, j) + s_new(i + 1, j) // east and west
                    + s_new(i, j - 1) + s_new(i, j + 1) // north and south
                    + alpha * s_old(i, j)
                    + beta * s_new(i, j) * (1.0 - s_new(i, j));
        }
    }

    // Compute boundary conditions
    // East boundary
    int i = nx - 1;
    for (int j = 1; j < jend; j++) {
        f(i, j) = -(4. + alpha) * s_new(i, j)
                + s_new(i - 1, j) + bndE[j]
                + s_new(i, j - 1) + s_new(i, j + 1)
                + alpha * s_old(i, j)
                + beta * s_new(i, j) * (1.0 - s_new(i, j));
    }

    // West boundary
    i = 0;
    for (int j = 1; j < jend; j++) {
        f(i, j) = -(4. + alpha) * s_new(i, j)
                + bndW[j] + s_new(i + 1, j)
                + s_new(i, j - 1) + s_new(i, j + 1)
                + alpha * s_old(i, j)
                + beta * s_new(i, j) * (1.0 - s_new(i, j));
    }

    // North boundary (plus NE and NW corners)
    int j = ny - 1;
    // NW corner
    i = 0;
    f(i, j) = -(4. + alpha) * s_new(i, j)
            + bndW[j] + s_new(i + 1, j)
            + s_new(i, j - 1) + bndN[i]
            + alpha * s_old(i, j)
            + beta * s_new(i, j) * (1.0 - s_new(i, j));

    // North boundary
    for (i = 1; i < iend; i++) {
        f(i, j) = -(4. + alpha) * s_new(i, j)
                + s_new(i - 1, j) + s_new(i + 1, j)
                + s_new(i, j - 1) + bndN[i]
                + alpha * s_old(i, j)
                + beta * s_new(i, j) * (1.0 - s_new(i, j));
    }

    // NE corner
    i = nx - 1;
    f(i, j) = -(4. + alpha) * s_new(i, j)
            + s_new(i - 1, j) + bndE[j]
            + s_new(i, j - 1) + bndN[i]
            + alpha * s_old(i, j)
            + beta * s_new(i, j) * (1.0 - s_new(i, j));

    // South boundary (plus SW and SE corners)
    j = 0;
    // SW corner
    i = 0;
    f(i, j) = -(4. + alpha) * s_new(i, j)
            + bndW[j] + s_new(i + 1, j)
            + bndS[i] + s_new(i, j + 1)
            + alpha * s_old(i, j)
            + beta * s_new(i, j) * (1.0 - s_new(i, j));

    // South boundary
    for (i = 1; i < iend; i++) {
        f(i, j) = -(4. + alpha) * s_new(i, j)
                + s_new(i - 1, j) + s_new(i + 1, j)
                + bndS[i] + s_new(i, j + 1)
                + alpha * s_old(i, j)
                + beta * s_new(i, j) * (1.0 - s_new(i, j));
    }

    // SE corner
    i = nx - 1;
    f(i, j) = -(4. + alpha) * s_new(i, j)
            + s_new(i - 1, j) + bndE[j]
            + bndS[i] + s_new(i, j + 1)
            + alpha * s_old(i, j)
            + beta * s_new(i, j) * (1.0 - s_new(i, j));

    // Accumulate the flop counts
    // 8 ops total per point
    stats::flops_diff += 12 * (nx - 2) * (ny - 2) // interior points
                      +  11 * (nx - 2  +  ny - 2) // NESW boundary points
                      +  11 * 4;                  // corner points
}

} // namespace operators

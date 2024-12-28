/*
 * HPC Class 2024 Project 7: Poisson Equation Solver with PETSc
 * Author: Aryan Eftekhri (based on PETSc tutorial ex29.c)
 * 
 * Description:
 * Solves the Poisson equation:
 *     -div(grad u) = f, 0 < x, y < 1,
 * with:
 *     - Forcing function f = c.
 *     - Dirichlet boundary conditions: u = f(x, y) on all boundaries.
 * 
 * Utilizes PETSc's Krylov subspace methods and DMDA for parallelized grid management.
 * Based on: https://petsc.org/release/src/ksp/ksp/tutorials/ex29.c.html
 * 
 * Compilation:
 * > source /apps/miniconda3/bin/activate
 * > conda activate /apps/miniconda3/envs/petsc/
 * > export PETSC_DIR=/apps/miniconda3/envs/petsc
 * > module load gcc openmpi
 * > make
 * 
 * Execution:
 * Run on multiple MPI processes:
 *     mpirun -np 4 poisson_petsc -ksp_type cg -log_view :performance_log.txt
 * 
 * Key Options:
 * - `-ksp_monitor_short`: Short Krylov solver residual output (avoid for timing).
 * - `-log_view :performance_log.txt`: Logs runtime performance.
 * - `-ksp_type`: Solver type (optional). See details:
 *       gmres: Generalized Minimal Residual Method (default).
 *       cg: Conjugate Gradient (for symmetric positive definite systems).
 *       More options: https://petsc.org/release/manualpages/KSP/KSPType/
 * - `-pc_type`: Preconditioner (optional). See details:
 *       none: No preconditioner (default).
 *       jacobi: Lightweight diagonal preconditioner.
 *       ilu: Efficient for sparse systems.
 *       gamg: Algebraic multigrid for large symmetric problems.
 *       asm: Additive Schwarz Method for distributed systems.
 *       More options: https://petsc.org/release/manualpages/PC/PCType/
 * 
 * Visualization:
 * Solution stored in `solution_petsc.dat` and grid metadata in `solution_petsc.info`.
 * Plot results using:
 *     python3 draw.py solution_petsc
 * 
 * Acknowledgment:
 * Based on PETSc tutorial examples.
 */

#include <petscdm.h>
#include <petscdmda.h>
#include <petscksp.h>

// Struct to hold the constant source term
typedef struct {
  PetscScalar c; // Constant value for f = c
} UserContext;

PetscErrorCode ComputeRHS(KSP ksp, Vec b, void *ctx) {
  UserContext   *user = (UserContext *)ctx;  // User-provided context for constant source term
  DM             da;                         // Distributed array
  DMDALocalInfo  info;                       // Local grid information
  PetscScalar  **array;                      // Local portion of vector array
  PetscReal      hx, hy;                     // Grid spacing in x and y directions
  PetscInt       i, j;

  PetscFunctionBeginUser;

  // Get the distributed array and local grid information
  PetscCall(KSPGetDM(ksp, &da));
  PetscCall(DMDAGetLocalInfo(da, &info));

  // Calculate grid spacing
  hx = 1.0 / (PetscReal)(info.mx - 1);
  hy = 1.0 / (PetscReal)(info.my - 1); 

  // Access the local portion of the right-hand side vector
  PetscCall(DMDAVecGetArray(da, b, &array));

  // TODO 
  // Loop over the local grid points
  for (j = info.ys; j < info.ys + info.ym; j++) {
    for (i = info.xs; i < info.xs + info.xm; i++) {
      if (i == 0 || j == 0 || i == info.mx - 1 || j == info.my - 1) {
        array[j][i] = 0.0;  // u = 0 on boundaries
      } else {
        array[j][i] = user->c;  // RHS = constant source term
      }
    }
  }
  // Set the values of the RHS based on boundary and interior
  //! Note "info" contains everything you need ... see (https://petsc.org/release/manualpages/DMDA/DMDAGetInfo/)

  // Restore the array and assemble the global vector
  PetscCall(DMDAVecRestoreArray(da, b, &array));
  PetscCall(VecAssemblyBegin(b));
  PetscCall(VecAssemblyEnd(b));

  PetscFunctionReturn(PETSC_SUCCESS);
}

PetscErrorCode ComputeMatrix(KSP ksp, Mat A, Mat P, void *ctx) {
  DM            da;
  DMDALocalInfo info;
  PetscReal     hx, hy, hxd, hyd, hxhyd;
  MatStencil    row, col[5];
  PetscInt      i, j;
  PetscScalar   v[5];

  PetscFunctionBeginUser;

  // Retrieve the distributed array, grid information, and global grid dimensions
  PetscCall(KSPGetDM(ksp, &da));
  PetscCall(DMDAGetLocalInfo(da, &info)); // info.mx and info.my include boundary points
  hx = 1.0 / (PetscReal)(info.mx - 1);
  hy = 1.0 / (PetscReal)(info.my - 1);
  hxd = hx * hx;
  hyd = hy * hy;
  hxhyd = 2.0 / hxd + 2.0 / hyd;

  // TODO 
  // Loop over the local grid points
  // Set the values of the matrix based on 5 point Stencil 
  //! Note "info" contains everything you need ... see (https://petsc.org/release/manualpages/DMDA/DMDAGetInfo/)

  // For boundary points, we enforce Dirichlet conditions: u = 0 at boundaries
  // This means the row in the matrix should be: A_{ii} = 1 and no other non-zero values.
  // For interior points, we use the standard 5-point stencil:
  //   A_{ij,ij} = - (2/hx^2 + 2/hy^2)
  //   A_{ij,i±1j} = 1/hx^2
  //   A_{ij,ij±1} = 1/hy^2
  
  for (j = info.ys; j < info.ys + info.ym; j++) {
    for (i = info.xs; i < info.xs + info.xm; i++) {

      row.i = i; row.j = j;

      if (i == 0 || j == 0 || i == info.mx - 1 || j == info.my - 1) {
        // Boundary point: Dirichlet condition
        v[0] = 1.0;
        PetscCall(MatSetValuesStencil(A, 1, &row, 1, &row, v, INSERT_VALUES));
      } else {
        // Interior point: 5-point stencil
        v[0] = 1.0 / hyd;     col[0].i = i;     col[0].j = j - 1; // Bottom
        v[1] = 1.0 / hxd;     col[1].i = i - 1; col[1].j = j;     // Left
        v[2] = -hxhyd;        col[2].i = i;     col[2].j = j;     // Center
        v[3] = 1.0 / hxd;     col[3].i = i + 1; col[3].j = j;     // Right
        v[4] = 1.0 / hyd;     col[4].i = i;     col[4].j = j + 1; // Top

        PetscCall(MatSetValuesStencil(A, 1, &row, 5, col, v, INSERT_VALUES));
      }
    }
  }

  // Assemble the matrix after all values have been set
  PetscCall(MatAssemblyBegin(A, MAT_FINAL_ASSEMBLY));
  PetscCall(MatAssemblyEnd(A, MAT_FINAL_ASSEMBLY));

  PetscFunctionReturn(PETSC_SUCCESS);
}

int main(int argc, char **argv){
  KSP         ksp;
  DM          da;
  UserContext user;
  Vec         b, x;
  PetscMPIInt rank;
  PetscInt    nx = 64, ny = 18; // Default grid size .. 
  PetscBool   flg;
  PetscLogDouble t1, t2;

  PetscFunctionBeginUser;

  // Initialize PETSc
  PetscCall(PetscInitialize(&argc, &argv, NULL, "Poisson's Equation Solver PETSc\n"));
  PetscCallMPI(MPI_Comm_rank(PETSC_COMM_WORLD, &rank));

  // Get nx and ny from command-line options
  PetscCall(PetscOptionsGetInt(NULL, NULL, "-nx", &nx, &flg));
  if (!flg) PetscCall(PetscPrintf(PETSC_COMM_WORLD, "Using default nx = %d (override with -nx <value>)\n", nx));

  PetscCall(PetscOptionsGetInt(NULL, NULL, "-ny", &ny, &flg));
  if (!flg) PetscCall(PetscPrintf(PETSC_COMM_WORLD, "Using default ny = %d (override with -ny <value>)\n", ny));

  PetscCall(PetscPrintf(PETSC_COMM_WORLD, "----------------------------------------------------------------------------------------------------\n"));
  PetscCall(PetscPrintf(PETSC_COMM_WORLD, "Poisson's Equation Solver PETSc for %dx%d\n", nx, ny));
  PetscCall(PetscPrintf(PETSC_COMM_WORLD, "----------------------------------------------------------------------------------------------------\n"));

  // Initialize user context with constant forcing term c = 20
  user.c = 20.0;

  // Create the DMDA with initial grid size
  PetscCall(DMDACreate2d(PETSC_COMM_WORLD, DM_BOUNDARY_NONE, DM_BOUNDARY_NONE, DMDA_STENCIL_STAR, nx, ny, PETSC_DECIDE, PETSC_DECIDE, 1, 1, 0, 0, &da));
  PetscCall(DMSetFromOptions(da));
  PetscCall(DMSetUp(da));

  // Retrieve the updated grid size after refinement
  PetscCall(DMDAGetInfo(da, NULL, &nx, &ny, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL));
  PetscPrintf(PETSC_COMM_WORLD, "Grid size after refinement: nx=%d, ny=%d\n", nx, ny);

  // Setup KSP with matrix and RHS computation
  PetscCall(PetscTime(&t1));

  PetscCall(KSPCreate(PETSC_COMM_WORLD, &ksp));
  PetscCall(KSPSetComputeRHS(ksp, ComputeRHS, &user));
  PetscCall(KSPSetComputeOperators(ksp, ComputeMatrix, NULL));
  PetscCall(KSPSetDM(ksp, da));
  PetscCall(KSPSetFromOptions(ksp));
  PetscCall(KSPSetUp(ksp));

  PetscCall(PetscTime(&t2));
  PetscCall(PetscPrintf(PETSC_COMM_WORLD, "Time for RHS & Matrix Assembly: %.6f seconds\n", t2 - t1));

  // Solve the system
  PetscCall(PetscTime(&t1));

  PetscCall(KSPGetSolution(ksp, &x));
  PetscCall(KSPGetRhs(ksp, &b));
  PetscCall(KSPSetDMActive(ksp, PETSC_FALSE));
  PetscCall(KSPSetTolerances(ksp, 1e-6, 1e-10, PETSC_DEFAULT, 1000));
  PetscCall(KSPSolve(ksp, b, x));

  PetscCall(PetscTime(&t2));
  PetscCall(PetscPrintf(PETSC_COMM_WORLD, "Time for Solve: %.6f seconds\n", t2 - t1));

  // Write the refined grid size to a file from rank 0
  if (rank == 0) {
    FILE *file = fopen("solution_petsc.info", "w");
    if (!file) {
      PetscCall(PetscPrintf(PETSC_COMM_WORLD, "Error opening file solution.info\n"));
    } else {
      PetscCall(PetscFPrintf(PETSC_COMM_WORLD, file, "# nx, ny, a\n"));
      PetscCall(PetscFPrintf(PETSC_COMM_WORLD, file, "%d, %d, %c\n", nx, ny, '-'));
      fclose(file);
    }
  }

  // Write the solution to a file
  PetscViewer viewer;
  PetscCall(PetscViewerASCIIOpen(PETSC_COMM_WORLD, "solution_petsc.dat", &viewer));
  PetscCall(PetscViewerPushFormat(viewer, PETSC_VIEWER_ASCII_COMMON));
  PetscCall(VecView(x, viewer));
  PetscCall(PetscViewerDestroy(&viewer));

  PetscScalar norm;
  PetscCall(VecNorm(x, NORM_2, &norm));
  PetscCall(PetscPrintf(PETSC_COMM_WORLD, "L2 Norm of the solution: %.6f\n", norm));
    
  // Clean up
  PetscCall(DMDestroy(&da));
  PetscCall(KSPDestroy(&ksp));
  PetscCall(PetscFinalize());
  return 0;
}

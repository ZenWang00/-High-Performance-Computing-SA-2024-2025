#include <omp.h>
#include "walltime.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>

#define NUM_ITERATIONS 100
#define EPSILON 0.1

using namespace std;

// Function to calculate dot product using reduction
void dot_product_reduction(double* a, double* b, long double& result, int N) {
    #pragma omp parallel for reduction(+:result)
    for (int i = 0; i < N; i++) {
        result += a[i] * b[i];
    }
}

// Function to calculate dot product using critical with reduced overhead
void dot_product_critical(double* a, double* b, long double& result, int N) {
    long double local_result = 0.0;

    #pragma omp parallel private(local_result)
    {
        local_result = 0.0;
        
        #pragma omp for
        for (int i = 0; i < N; i++) {
            local_result += a[i] * b[i];
        }

        #pragma omp critical
        {
            result += local_result;
        }
    }
}

// Function to record the execution time of a dot product function
double time_record(void (*dot_function)(double*, double*, long double&, int), double* a, double* b, long double& result, int N) {
    double time_start = wall_time();
    for (int iterations = 0; iterations < NUM_ITERATIONS; iterations++) {
        result = 0.0;  // Reset result before each iteration
        dot_function(a, b, result, N);
    }
    return wall_time() - time_start;
}

// Function to append results to CSV file
void append_to_csv(const string& filename, int thread_count, const string& method, double time) {
    ofstream file;
    file.open(filename, ios::app);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }
    file << thread_count << "," << method << "," << time << endl;
    file.close();
}

int main(int argc, char** argv) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <Vector Length> <Thread Count>" << endl;
        return 1;
    }

    int N = atoi(argv[1]);
    int thread_count = atoi(argv[2]);
    omp_set_num_threads(thread_count);

    // Allocate memory for the vectors as 1-D arrays
    double *a = new double[N];
    double *b = new double[N];

    // Initialize the vectors with some values
    for (int i = 0; i < N; i++) {
        a[i] = i;
        b[i] = i / 10.0;
    }

    // Set up CSV file for this experiment
    string filename = "experiment_results_" + to_string(N) + ".csv";
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cerr << "Error creating file: " << filename << endl;
        return 1;
    }
    // Write header if the file is empty
    file.seekp(0, ios::end);
    if (file.tellp() == 0) {
        file << "Thread_Count,Method,Execution_Time\n";
    }
    file.close();

    // Serial execution
    long double alpha_serial = 0.0;
    double time_start = wall_time();
    for (int iterations = 0; iterations < NUM_ITERATIONS; iterations++) {
        alpha_serial = 0.0;
        for (int i = 0; i < N; i++) {
            alpha_serial += a[i] * b[i];
        }
    }
    double time_serial = wall_time() - time_start;
    cout << "Serial execution time = " << time_serial << " sec" << endl;

    // Parallel version using reduction
    long double alpha_reduction = 0.0;
    double time_reduction = time_record(dot_product_reduction, a, b, alpha_reduction, N);
    append_to_csv(filename, thread_count, "Reduction", time_reduction);

    // Parallel version using critical
    long double alpha_critical = 0.0;
    double time_critical = time_record(dot_product_critical, a, b, alpha_critical, N);
    append_to_csv(filename, thread_count, "Critical", time_critical);

    // Check correctness of results with serial version
    if ((fabs(alpha_reduction - alpha_serial) / fabs(alpha_serial)) > EPSILON) {
        cerr << "Reduction method incorrect: parallel = " << alpha_reduction
             << ", serial = " << alpha_serial << "\n";
        cerr << "Alpha not yet implemented correctly for reduction!\n";
        return 1;
    }
    if ((fabs(alpha_critical - alpha_serial) / fabs(alpha_serial)) > EPSILON) {
        cerr << "Critical method incorrect: parallel = " << alpha_critical
             << ", serial = " << alpha_serial << "\n";
        cerr << "Alpha not yet implemented correctly for critical!\n";
        return 1;
    }

    // Print results for each method
    cout << "Vector Length = " << N << ", Threads = " << thread_count << endl;
    cout << "Reduction Method: Result = " << alpha_reduction
         << ", Time = " << time_reduction << " sec" << endl;
    cout << "Critical Method: Result = " << alpha_critical
         << ", Time = " << time_critical << " sec" << endl;

    // De-allocate memory
    delete[] a;
    delete[] b;

    return 0;
}

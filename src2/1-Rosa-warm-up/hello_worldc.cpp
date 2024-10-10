#include <cstdlib>
#include <iostream>

using namespace std;

int main(){
    const char *nodes = std::getenv("SLURM_JOB_NODELIST");
    cout << "Hello World" << '\n';
    cout << "host(node) name:" << nodes << '\n';
    return 0;
}

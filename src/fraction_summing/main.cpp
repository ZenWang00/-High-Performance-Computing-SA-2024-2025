#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "fraction_toolbox.hpp"

using namespace std;

// read command line arguments
static void readcmdline(fraction & frac, int argc, char* argv[])
{
    if (argc!=3)
    {
        printf("Usage: n d\n");
        printf("  n        numerator of fraction\n");
        printf("  d        denominator of fraction\n");
        exit(1);
    }

    // read n
    frac.num = atoi(argv[1]);

    // read d
    frac.denom = atoi(argv[2]);
}

static void test23467(int argc, char* argv[])
{
    //TODO: implement function
    fraction frac;                
    readcmdline(frac, argc, argv);

    printf("test 2:\n");
    printf("orgin fraction: %d/%d; square fraction: %d/%d \n",
        frac.num, frac.denom, square_fraction(frac).num, square_fraction(frac).denom );

    printf("test 4:\n");
    printf("double value of fraction: %f\n", fraction2double(frac));

    printf("test 6:\n");
    printf("greatest common divisor: %d\n", gcd(frac));
    
    printf("test 7:\n");
    reduce_fraction_inplace(frac);
    printf("reduce fraction: %d/%d\n", frac.num, frac.denom);

    printf("test 3:\n");
    square_fraction_inplace(frac);
    printf("square fraction: %d/%d \n", frac.num, frac.denom );
}

static void test5()
{
    //TODO: implement function
    int inputInt1, inputInt2;
    printf("please input two integers to find out greatest common divisor:\n");
    scanf("%d %d", &inputInt1, &inputInt2);
    printf("%d \n", gcd(inputInt1,inputInt2));
}

static void test_array_functions(int n)
{
    //TODO: implement function

    //TODO: find n for which sum function breaks. Explain what is happening.
}

static void test_toolbox(int argc, char* argv[])
{
    cout << "\n===============  test23467  =============== " << endl;
    test23467(argc, argv);

    cout << "\n=================  test5  ================= " << endl;
    test5();

    cout << "\n==========  test_array_functions  ========= " << endl;
    int n = 5;
    test_array_functions(n);
}

int main(int argc, char* argv[])
{
    test_toolbox(argc, argv);
    return 0;
}
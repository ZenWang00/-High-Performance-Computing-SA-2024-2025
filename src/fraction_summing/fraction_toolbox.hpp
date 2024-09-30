#include <iostream>
//TODO: implement fraction datatype
//define struct fraction to store a rational number
struct fraction{
    int num; 
    int denom;
};
//without returning any data
void square_fraction_inplace(fraction &frac){
    frac.num = frac.num*frac.num;
    frac.denom = frac.denom*frac.denom;
};
//**********************************//
void print_fraction(fraction frac);

void print_fraction_array(fraction frac_array[], int n);
//**********************************//
//
fraction square_fraction(fraction frac){
    fraction square_frac;
    square_frac.num = frac.num*frac.num;
    square_frac.denom = frac.denom*frac.denom;
    return square_frac;
};

//TODO: add function declaration for function 3

double fraction2double(fraction frac){
    return double(frac.num/frac.denom);
};

int gcd(int a, int b){
    if (b = 0){
        return a;
    }
    else
    {
        return gcd(b, a%b);
    }
};

int gcd(fraction frac){
    int t;
    while (frac.denom != 0)
    {
        t = frac.denom;
        frac.denom = frac.num%frac.denom;
        frac.num = t;
    }
    return frac.num;
};

//TODO: add function declaration for function 7
fraction reduce_fraction_inplace(fraction &frac){
    gcd(frac)

};
fraction add_fractions(fraction frac1, fraction frac2);

double sum_fraction_array_approx(fraction frac_array[], int n);

fraction sum_fraction_array(fraction frac_array[], int n);

void fill_fraction_array(fraction frac_array[], int n);
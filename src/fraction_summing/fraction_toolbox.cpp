#include <iostream>

#include "fraction_toolbox.hpp"

void print_fraction(fraction frac)
{
    std::cout << frac.num << '/' << frac.denom << std::endl;
}

void print_fraction_array(fraction frac_array[], int n)
{
    std::cout << "[ " << frac_array[0].num << '/' << frac_array[0].denom << std::endl;
    for (int i = 1; i < n-1; i++)
    {
        std::cout << "  ";
        print_fraction(frac_array[i]);
    }
    std::cout << "  " << frac_array[n-1].num << '/' << frac_array[n-1].denom << " ]" << std::endl;
}

fraction square_fraction(fraction frac)
{
    //TODO: implement function 2
    fraction square_frac;
    square_frac.num = frac.num*frac.num;
    square_frac.denom = frac.denom*frac.denom;
    return square_frac;
}

//TODO: implement function 3
void square_fraction_inplace(fraction &frac){
    frac.num = frac.num*frac.num;
    frac.denom = frac.denom*frac.denom;
};

double fraction2double(fraction frac)
{
    //TODO: implement function 4
    return double(frac.num)/double(frac.denom);
}

int gcd(int a, int b)
{
    //TODO: implement function 5
    if (b == 0){
        return a;
    }
    else
    {
        return gcd(b, a%b);
    }
}

//TODO: implement function 6
int gcd(fraction frac){
    int temp_denom;
    while (frac.denom != 0)
    {
        temp_denom = frac.denom;
        frac.denom = frac.num%frac.denom;
        frac.num = temp_denom;
    }
    return frac.num;
};

void reduce_fraction_inplace(fraction & frac)
{
    //TODO: implement function 7
    int gcd_val = gcd(frac);
    frac.num /= gcd_val;
    frac.denom /= gcd_val;
    //TODO: add short comment to explain which of the gcd() functions your code is calling
    //Use iterative Euclid’s algorithm, gcd() from line 54 - 64
    //Above functions are overload in C++
    //Since the entire fraction structure is passed, the iterative gcd() is the one being used
}

fraction add_fractions(fraction frac1, fraction frac2)
{
    //TODO: implement function 8
    fraction add_frac;
    add_frac.num = frac1.num + frac2.num;
    add_frac.denom = frac1.denom + frac2.denom;
    reduce_fraction_inplace(add_frac);

}

double sum_fraction_array_approx(fraction frac_array[], int n)
{
    //TODO: implement function 9
    double sum = 0.0;
    for (int i = 0; i < n; i++)
    {
         sum += double(frac_array[i].num)/double(frac_array[i].denom);
    }
    return sum;
    //TODO: add short comment to explain why this function is approximate
    //The significant figures of double type is limited
    //If the int value is too large will make result as an approximate function 
}

//TODO: implement function 10
fraction sum_fraction_array(fraction frac_array[], int n){
    fraction sum = {0,1};
    for (int i = 0; i < n; i++)
    {
        sum = add_fractions(sum, frac_array[i]);
    }
    return sum;
};

void fill_fraction_array(fraction frac_array[], int n)
{
    fraction temp_frac;
    temp_frac.num = 1;
    for (int i = 1; i <= n; i++)
    {
        temp_frac.denom = i * (i+1);
        frac_array[i-1] = temp_frac;
    }
};
/* 
 * File:   main.cpp
 * Author: alpha
 *
 * Created on July 23, 2012, 8:24 AM
 */

#include <cstdlib>
#include <iostream>
#include <armadillo>
#include <bitset>

using namespace std;
using namespace arma;

/*
 * 
 */
int main(int argc, char** argv) {

    srand(time(NULL));

    cout<<"hello world"<<endl;

    mat a = randu<mat>( 5 , 6 );
    mat b = randu<mat>( 6 , 3 );

    mat c;

    c = a * b;

    cout<< a << b <<endl;
    cout<<"a(4,3) ="<<a(4,3)<<endl;
    cout << c <<endl;


    bitset< 33 > bs;   //try bit set

    cout << "the size of bs is :" << sizeof( bs ) <<endl;


    return 0;
}


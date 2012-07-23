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

    cout<<"hello world"<<endl;

    mat a = randu( 5 , 6 );
    mat b = randu( 6 , 3 );

    mat c;

    c = a * b;

    cout << c <<endl;

    bitset< 33 > bs;   //try bit set

    cout << "the size of bs is :" << sizeof( bs ) <<endl;

    bitset< unsigned int> * p_bs;

    p_bs = new bitset< unsigned int >( 10000 );

    return 0;
}


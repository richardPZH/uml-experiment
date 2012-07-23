/* 
 * File:   main.cpp
 * Author: alpha
 * IMS don't be lazy. Remember to compile
 * Stop the compile error at the source. Stop them from growing.
 * When you are idel. Compile & Compile it.
 * Created on July 23, 2012, 8:24 AM
 */

#include <cstdlib>
#include <iostream>
#include <armadillo>
#include <bitset>
#include <vector>

using namespace std;
using namespace arma;

/*
 * 
 */
int main(int argc, char** argv) {

    srand(time(NULL));           //the randu may need this

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


    cout<< "the size of size_t is " << sizeof( size_t ) <<endl;
    
    size_t s1=5;
    size_t s2=7;

    cout<< "s1=" <<s1 <<"  s2="<<s2<<endl;
    cout<< "s2 - 21 = "<<s2 - s1 <<endl;
    cout<< "s1 - s2 = "<< s1 - s2 << endl;

    bool ba=0;
    
    cout<< "a = "<<ba<<endl;
    cout<< "!a = "<<!(ba)<<endl;

    vector< Col<double> > vec_coldouble(5);
    vector<int> vec_array(10);

    vec_array.push_back( 5 );
    vec_array.push_back( 4 );

    size_t numi= vec_array.size();
    for( size_t i=0; i< numi ; i++ )
    {
        cout<<vec_array[i]<<"\t";
    }

    return 0;
}


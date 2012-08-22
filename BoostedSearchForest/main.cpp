/* 
 * File:   main.cpp
 * Author: alpha
 * IMS don't be lazy. Remember to compile
 * Stop the compile error at the source. Stop them from growing.
 * When you are idel. Compile & Compile it.
 *
 * Open the option -O1 when all things are ok!!!
 * when -01 is used, it's a little bit hard to debug
 *
 * Created on July 23, 2012, 8:24 AM
 */

#include <cstdlib>
#include <iostream>
#include <armadillo>
#include <bitset>
#include <limits>
#include <vector>
#include <cmath>
#include <stdlib.h>

#include "Preprocess.h"
#include "BSF.h"

using namespace std;
using namespace arma;

Col<double> sampleClass;
/*
 * 
 */
int main(int argc, char** argv) {

    Mat<double> *p_q = NULL;  //the queries matrix
    Mat<double> *p_x = NULL;  //the build samples matrix
    Mat<double> *p_d = NULL;  //the database matrix
    Mat<char>   *p_s = NULL;  //the given similarity matrix
    unsigned int * iArray;

    double lamda = 0.444;    //lamda is user define lamda must between 0 and 1 , becasue the c=log( (1-lamda)/lamda * p11/p10 );
    int cm = 10;         //how many trees

    generateQBS( "optidigit.txt" , &p_q , &p_x , &p_s , &p_d , &iArray );

    BSF bsf( p_x , p_s , p_d , lamda , cm );

    bsf.boost();

    bsf.printTreeWeightCm();

    //cout<<sampleClass<<endl;
    
    delete p_q;
    delete p_x;
    delete p_d;
    delete p_s;
    delete []iArray;

    return 0;
}

#if 0
//
//    cout<< p_q->n_rows <<endl;
//    cout<< *p_q <<endl;
//    cout<<endl;
//    for( int i=0 ; i<7 ; i++ )
//    {
//        cout<< iArray[i] <<" ";
//    }
//    cout<< endl << p_x->n_rows <<endl;
//    cout<< p_d->n_rows <<endl;
//    cout<< p_s->n_rows <<endl;
//
//
//    cout<< endl << *p_x <<endl;
//    cout<< endl << p_s->at( 0 , 7 ) + '0' <<endl;
//    cout<< endl << p_s->at( 4 , 1 ) + '0' <<endl;



    //Test is fine!!! no we can load samples just like the matlab load command
    //This need to test the initial of the Mat<double> of the armadillo
    Mat<double> t;

    if( ( t.load("wine.txt" , raw_ascii ) ) == false )
    {
        cerr<< "Error Loading wine.txt\n";
        exit( 1 );
    }

    //cout<< t.row( 5 ) << endl << t.row( 123 ) << endl;
    cout<< t.n_rows << endl << t.n_cols << endl << t.n_elem <<endl;

    //t.save( "wine2.txt" , raw_ascii ); // <-- this will save the data in raw ascii in file wine2.txt, can use diff to see the different!!

#endif

#if 0

    cout<< "e = " << M_E << endl;
    cout<< "log(M_E) "<< log(M_E) <<endl;

    double x;
    x = 1234;
    cout<< "log(x=12345) = "<<log( x ) <<endl;
    cout<< "log10(x=12345) ="<< log10( x )<<endl;

    double mina =  numeric_limits< double >:: max() * -1 ;
    double min;

    cout << mina <<endl;

    if( numeric_limits<double>:: has_infinity )
    {
        min = -1 * numeric_limits< double > ::infinity();
        cout<<  min <<endl;
        cout<<" mina > min ? =  " << ( mina < min ) << endl;
    }


//    mat a = randu<mat>(5 , 5 );
//
//    cout<< a <<endl;
//
//    cout<< a.t() <<endl;
//
//    trans( a );
//    cout<< a <<endl;

    vec eigval;
    mat eigvec;
    mat symetric;

    symetric << 1 << 2 << 2 << endr \
             << 2 << 1 << 2 << endr \
             << 2 << 2 << 1 << endr;

    // use standard algorithm by default
    eig_sym(eigval, eigvec, symetric );

    cout << "eigval.col " << eigval.n_cols << endl << "eigval.row " << eigval.n_rows <<endl;
    cout << eigval <<endl;

    cout << "eigvec.col " << eigvec.n_cols << endl << "eigvec.row " << eigvec.n_rows <<endl;
    cout << eigvec <<endl;

    cout<< "eigvec.col(2) : \n" << eigvec.col(2) <<endl;
/*
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

*/
#endif

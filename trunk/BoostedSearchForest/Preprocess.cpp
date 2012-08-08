#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <algorithm>
#include <time.h>

#include "Preprocess.h"

bool generateQBS( const char * infile , Mat<double> ** p_q , Mat<double> ** p_x , Mat<char> ** p_s ,  Mat<double> ** p_d , int ** iArray)
{
    //first check the fragment error
    if( fabs( fragment_q + fragment_b + fragment_d - 1) > 0.001  )
    {
        cerr<<"Error Fragment, set the fragment_* in Preprocess.h properly"<<endl;
        return false;
    }

    //load the matrix in the *infile
    Mat<double> t;
    if( ( t.load( infile , raw_ascii ) ) == false )
    {
        cerr<< "Error Loading wine.txt\n";
        return false;
    }

    //new the index array and initial it to 0 - t.n_rows-1
    int isize = t.n_rows;
    *iArray = new int[ isize ];  //user shall remember to free this array
    for( int i=0 ; i<isize ; i++ )
    {
        (*iArray)[i] = i;
    }

    //shuffle the iArray to get
    srand((unsigned) time(NULL));   //random_shuffle needs this
    random_shuffle( *iArray , (*iArray)+isize );

//    for( int i=0 ; i<isize ; i++ )
//    {
//        cout<< (*iArray)[i] <<" ";
//    }


    return true;
}


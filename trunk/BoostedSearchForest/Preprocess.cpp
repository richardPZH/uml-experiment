#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <algorithm>
#include <time.h>

#include "Preprocess.h"

bool generateQBS( const char * infile , Mat<double> ** p_q , Mat<double> ** p_x , Mat<char> ** p_s ,  Mat<double> ** p_d , unsigned int ** iArray)
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
    *iArray = new unsigned int[ isize ];  //user shall remember to free this array
    for( int i=0 ; i<isize ; i++ )
    {
        (*iArray)[i] = i;
    }

    //shuffle the iArray to get random samples as queries and buildSamples and search database
    srand((unsigned) time(NULL));   //random_shuffle needs this
    random_shuffle( *iArray , (*iArray)+isize );

    Col<uword> * p_indices;
    //new the p_q
    *p_q = new Mat<double>;
    int num_q;
    num_q = (int) ( t.n_rows * fragment_q );
    p_indices = new Col<uword>( (*iArray) , num_q  , true ,  true);
    **p_q = t.rows( *p_indices );
    delete p_indices;

    //new the p_x
    *p_x = new Mat<double>;
    int num_x;
    num_x = (int)( t.n_rows * fragment_b );
    p_indices = new Col<uword>( (*iArray)+num_q , num_x  , true ,  true);
    **p_x = t.rows( *p_indices );
    delete p_indices;

    //new the p_d
    *p_d = new Mat<double>;
    int num_d;
    num_d = t.n_rows - num_q - num_x;
    p_indices = new Col<uword>( (*iArray)+num_q+num_x , num_d  , true ,  true);
    **p_d = t.rows( *p_indices );
    delete p_indices;

    //new the p_s , remember the p_s is symmetric
    *p_s = new Mat<char>( num_x , num_x );
    int finalCol = (*p_x)->n_cols - 1;
    for( int i=0 ; i<num_x ; i++ )
    {
        (*p_s)->at( i , i ) = 1;           //similar is 1, dissimilar is 0
        for( int j=i+1; j<num_x ; j++ )
        {
            double f,s;
            f = (*p_x)->at( i , finalCol );
            s = (*p_x)->at( j , finalCol );
            if( fabs( f - s ) > 0.0001 )
            {
                (*p_s)->at( i , j ) = 0;
                (*p_s)->at( j , i ) = 0;
            }else
            {
                (*p_s)->at( i , j ) = 1;
                (*p_s)->at( j , i ) = 1;
            }
        }
    }


    //change the p_q p_x p_d 's final col to 1, this is require input for the BSF

    return true;
}


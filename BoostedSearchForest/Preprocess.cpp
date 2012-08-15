#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <algorithm>
#include <time.h>

#include "Preprocess.h"

template< class Tmat >
static void setMatrixFinalColTo1( Tmat * );

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
    //random_shuffle( *iArray , (*iArray)+isize );

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

    cout<< (*p_x)->col( finalCol ) <<endl;
    
    //setMatrixFinalColTo1( (*p_q));   //the request do not need to be set to 1 at the final column
    setMatrixFinalColTo1( (*p_x));
    //setMatrixFinalColTo1( (*p_d));   //the database do net need to be set to 1 at the final column

    return true;
}

template< class Tmat >
void setMatrixFinalColTo1( Tmat * p )
{
    int i,j;
    j = p->n_cols - 1;
    i = p->n_rows;
    for( int k=0 ; k<i ; k++ )
    {
        p->at( k , j ) = 1;
    }

}


//    //verify the p_s matrix, is it correct? yes the similarity matrix is symmetric and correct!
//    cout<< (*p_x)->col( finalCol ) <<endl;
//    for( int i=0 ; i<(*p_s)->n_rows ; i++ )
//    {
//        for( int j=0 ; j<(*p_s)->n_cols; j++ )
//        {
//            cout<< char( (**p_s).at( i , j ) + '0' ) << " ";
//        }
//        cout<<endl;
//    }


    //change the p_q p_x p_d 's final col to 1, this is require input for the BSF
//    int i,j;
//    j = (*p_q)->n_cols - 1;
//    i = (*p_q)->n_rows;
//    for( int k=0 ; k<i ; k++ )
//    {
//        (*p_q)->at( k , j ) = 1;
//    }
//
//    j = (*p_x)->n_cols - 1;
//    i = (*p_x)->n_rows;
//    for( int k=0 ; k<i ; k++)
//    {
//        (*p_x)->at( k , j ) = 1;
//    }
//
//    j = (*p_d)->n_cols - 1;
//    i = (*p_d)->n_rows;
//    for( int k=0 ; k<i ; k++)
//    {
//        (*p_x)->at( k , j ) = 1;
//    }
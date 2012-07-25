/* 
 * File:   BSF.cpp
 * Author: alpha
 * 
 * Created on July 23, 2012, 8:49 AM
 */

#include "BSF.h"

//Paramaters:
// cp_x   the address of the sample x matrix
// cp_s   the address of the similarity s matrix; these two matrix won't be copied during process, in order to safe memory
// clamda the tuning paramater that balances quality and computational cost
// cm     the number of trees in the forest
BSF::BSF( const Mat< double > * cp_x , const Mat< char > * cp_s , const double clamda , const size_t cm )
{
    p_x = cp_x;            //point to the sample x matrix
    p_s = cp_s;            //point to the similarity s matrix
    lamda = clamda;        //tuning paramater
    m = cm;                //store the number of trees

    forestEntrance = new Tree[m];  //we have m trees
    treesWeight = new double[m];   //the weight to m trees

    p_w = new Mat<double>( p_x->n_rows , p_x->n_rows ); //new an nxn weight matrix start from 00
    p_w->fill( 1 );                                     //initial wij=1 because wij=1/(n*n) ==> wij=1 Zhen.Li@gmail.com


}

//This will follow the Algorithm 1 in the paper
//Boosted Selection Function Learning
// input: void
// retval: bool , true --> learn succeed; false --> learn fail
// this function can be modified to take the lamda and m than boost again and again for test purpose
// but current this function should call only once!

// a set of data points X is pointed by p_x;
// pairwise similarities sij is pointed by p_s;
// weights wij is pointed by p_w and initial all to 1;
bool BSF::boost( void )
{
    size_t i;
    for( i=0 ; i< m ; i++ ){

        //here learn a tree ti


        //here we calculate the ci


        //here we update weights wij

    }

    return true;
}


BSF::~BSF() {
    
    delete []forestEntrance;
    delete []treesWeight;

    delete p_w;

}


/* 
 * File:   BSF.cpp
 * Author: alpha
 * 
 * Created on July 23, 2012, 8:49 AM
 */

#include "BSF.h"
#include <map>
#include <iostream>

using namespace std;

//Paramaters:
// cp_x   the address of the sample x matrix
// cp_s   the address of the similarity s matrix; these two matrix won't be copied during process, in order to safe memory
// cp_d   the address of the database matrix
// clamda the tuning paramater that balances quality and computational cost
// cm     the number of trees in the forest
BSF::BSF( const Mat< double > * cp_x , const Mat< char > * cp_s , const Mat<double> * cp_d , const double clamda , const size_t cm )
{
    p_x = cp_x;            //point to the sample x matrix
    p_s = cp_s;            //point to the similarity s matrix
    p_d = cp_d;            //point to the database d matrix
    lamda = clamda;        //tuning paramater
    m = cm;                //store the number of trees

    Tree tmp( p_x->n_rows);
    forestEntrance = new vector< Tree >( m , tmp);  //we have m trees

    p_w = new Mat<double>( p_x->n_rows , p_x->n_rows ); //new an nxn weight matrix start from 00
    p_w->fill( 1 );                                     //initial wij=1 because wij=1/(n*n) ==> wij=1 Zhen.Li@gmail.com

    pResult = NULL;

}

const double BSF:: confidence = 0;

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

    //size_t nRows,nCols;

    //nRows = p_w->n_rows;
    //nCols = p_w->n_cols;

    for( i=0 ; i< m ; i++ ){

        //here learn a tree ti
        forestEntrance->at(i).grow( p_x , p_s , p_w , lamda );

        //build inverted indices after the tree is grown
        forestEntrance->at(i).addDatabaseItems( p_d );

        //here we calculate the ci, the ci is store in the tree i, now omitting the retval
        forestEntrance->at(i).findCi( p_s , p_d , lamda );

        //here we update weights wij
        forestEntrance->at(i).updateWeights( p_w , p_s , lamda );

    }

    return true;
}





//BSF search function
//input : a pointer to a sample Row<double> : [ x0 , x1 , x2 , x3 , x4 .... xk , classlable ]  <--- be aware!
//retval: a pointer to the submatrix of the *p_x, those samples are consider similar to the given sample
//        user doesn't have to free(delete) this pointer, BSF will handle it.
Mat<double> * BSF:: search( const Row< double > * psample )
{
    if( NULL == pResult )
    {
        pResult = new Mat<double>;
    }

    map< unsigned int , double > mx;
    map< unsigned int , double > md;
   

    Row<double> tsample;
    tsample = * psample;
    tsample.at( tsample.n_elem -1 ) = 1;

    for( size_t i=0 ; i < m ; i++ )
    {
        forestEntrance->at(i).findImage( &tsample , mx , md );
    }

    //we need to form the pResult matrix
    //the confidence occurs here
    map<unsigned int , double>::iterator  iter;       //will the erase function return the next location?
    for(iter = mx.begin(); iter != mx.end(); )
    {
        if( iter->second <= confidence )
        {
            mx.erase( iter++ );                      //they said this is stl std and iter++ not ++iter
        }
        else
        {
            iter++;
        }
    }

    for( iter = md.begin() ; iter != md.end() ; )
    {
        if( iter->second <= confidence )
        {
            md.erase( iter++ );                   //why iter++, because this is very special!!! warn the programmers
        }
        else
        {
            iter++;
        }
    }

    Col<uword> indexX( mx.size() );  //matlab does care the index is col or row vector, but armadillo does
    Col<uword> indexD( md.size() );  //so here must use the Col<uword> instead of row<uword>


    iter = mx.begin();
    for( int i = 0 ; iter != mx.end() ; )
    {
        indexX.at( i ) = iter->first;

        i++;
        iter++;
    }

    iter = md.begin();
    for( int i = 0 ; iter != md.end() ; )
    {
        indexD.at( i ) = iter->first;

        i++;
        iter++;
    }

    //we need to get the sx lables correct
    Mat<double> sx = p_x->rows( indexX ); 
    for( int i=0 ; i < sx.n_rows ; i++ )
    {
        sx.at( i , sx.n_cols-1 ) = sampleClass.at( indexX.at(i) );
    }

    Mat<double> sd = p_d->rows( indexD );

    *pResult = join_cols( sx , sd );

    //let's calculate the accuracy and the return num fragment of total samples


    return pResult;
}

//Just Print Out the c of trees in the forest
void BSF::printTreeWeightCm( void )
{
    int i;
    cout<<"There are "<<m<<" trees in the forest, weight c:"<<endl;
    for( i=0; i<m ; i++ )
    {
        cout<< forestEntrance->at( i ).getCm()<<endl;
    }

}


BSF::~BSF() {
    
    delete forestEntrance;

    delete p_w;

    if( pResult != NULL )
    {
        delete pResult;
    }

}


////Build inverted indices by passing all data points through the learned search trees
////input: no, because the p_d is saved in the forest
////retval: bool, true --> everything is ok
////              false--> something wrong happen
//bool BSF:: buildInvertedIndices( void )
//{
//    size_t i;
//
//    for( i=0 ; i < m ; i++ )
//    {
//        forestEntrance->at(i).addDatabaseItems( p_d );
//    }
//
//
//    return true;
//}

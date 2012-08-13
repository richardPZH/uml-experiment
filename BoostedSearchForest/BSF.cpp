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
    candaArry = new double[p_x->n_rows];

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

    size_t nRows,nCols;

    //nRows = p_w->n_rows;
    //nCols = p_w->n_cols;

    for( i=0 ; i< m ; i++ ){

        //here learn a tree ti
        forestEntrance->at(i).grow( p_x , p_s , p_w , lamda );


        //here we calculate the ci, the ci is store in the tree i, now omitting the retval
        forestEntrance->at(i).findCi( p_s , lamda );

        //here we update weights wij
        forestEntrance->at(i).updateWeights( p_w , p_s , lamda );

    }

    return true;
}

//BSF search function
//input : a pointer to a sample Row<double> : [ x0 , x1 , x2 , x3 , x4 .... xk , 1 ]  <--- be aware!
//retval: a pointer to the submatrix of the *p_x, those samples are consider similar to the given sample
//        user doesn't have to free(delete) this pointer, BSF will handle it.
Mat<double> * BSF:: search( const Row< double > * psample )
{
    if( pResult != NULL )    //should I delete it myself or user delete it?
    {
        delete pResult;
    }

    size_t i,j;
    j = p_x->n_rows;

    for( i=0 ; i<j ; i++ )
    {
        candaArry[i] = 0;
    }

    //walk through the trees
    for( i=0 ; i<m ; i++ )
    {
        forestEntrance->at(i).findImage( psample , candaArry );
    }

    //only those values bigger that confidence is collected ?
    int num;
    double *p;

    p = candaArry;
    num = 0;
    for( i=0 ; i<j ; i++ )
    {
        if( candaArry[i] > confidence )
        {
            *p = (int) i;
            p++;
            num++;
        }
    }

    unsigned int *tmp = new unsigned int[num];
    for( i=0 ; i<num ; i++ )
    {
        tmp[i] = (unsigned int) candaArry[i];
    }

    Col<uword> *p_col = new Col<uword>( tmp , num  , true ,  true);

    Mat<double> * p_mat = new Mat<double>;

    *p_mat = p_x->rows( *p_col );

    delete p_col;
    delete []tmp;

    return p_mat;
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

    delete[] candaArry;
    if( pResult != NULL )
    {
        delete pResult;
    }

}



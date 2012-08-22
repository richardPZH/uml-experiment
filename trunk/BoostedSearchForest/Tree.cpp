/* 
 * File:   Tree.cpp
 * Author: alpha
 * 
 * Created on July 23, 2012, 8:54 AM
 */

#include "Tree.h"
#include "TreeNode.h"

#include <cmath>
#include <iostream>
#include <queue>
#include <limits>
#include <armadillo>
#include <string.h>

using namespace std;
using namespace arma;

Tree:: Tree( const size_t numOfSamples ) {

    root = NULL;
    numFruit =( unsigned int )numOfSamples;
    fruit = new unsigned int[numFruit];

    for( unsigned int i=0 ; i<numFruit ; i++ )
    {
        fruit[i]=i;                    //the index to the sample in the matrix. There are numFruit of samples.
    }

}

Tree:: Tree( const Tree &obj )  //vital ??
{
    root = obj.root;
    numFruit = obj.numFruit;
    fruit = new unsigned int[numFruit];

    memcpy( fruit , obj.fruit , numFruit * sizeof( * fruit ) );
}

//Mat<double> Tree:: NmyZero = zeros< Mat<double> >(1,1); //initial the static member zero matrix

//Search Tree Construction: grow
//Input: p_x -> address of the sample matrix
//       p_s -> address of the similarity matrix
//       p_w -> address of the weight matrix
//       lamda -> zij need him
//retval: true -> tree grows succeed; false -> tree grows fail
bool Tree:: grow( const Mat<double> *p_x ,const Mat<char> *p_s ,const Mat<double> *p_w , const double lamda )
{   
    queue< TreeNode * > qu_p;
    
    root = new TreeNode;
    root->setLeaf();                                     //root initial is a leaf conatining all the samples
    (root->leafL).lFruit = fruit;
    (root->leafL).rFruit = fruit + numFruit - 1;
    
    if( numeric_limits<double>:: has_infinity )
    {
        (root->leafL).J = -1 * numeric_limits< double > ::infinity();
        //here we should calculate the root's J, but since root must be splited so set to -inf
    }
    else
    {
        (root->leafL).J = -111111111;
        //no infinity just a very small number
    }

    qu_p.push( root );                          //Assign X as root; enqueue root

    TreeNode * cLeaf, * cLeftChild, * cRightChild;
    unsigned int * array;
    Col< uword > * p_indices;
    Mat<double> * p_M;

    while( ! qu_p.empty() )
    {
        cLeaf = qu_p.front();                   //here the cLeaf must point to a leafnode not a internal node
        qu_p.pop();                             //Find a leaf node l in the queue; dequeue l

        //Find the optimal split for l by solving (18)
        //1. get X~
        Mat<double> X;
        Mat<double> Xt;
        size_t numSamples = (cLeaf->leafL).rFruit - (cLeaf->leafL).lFruit + 1;
        array =(unsigned int *)( (cLeaf->leafL).lFruit );
        
        p_indices = new Col< uword >( array , (uword)numSamples , true , true ); //may improve this

        X = p_x->rows( *p_indices );           //X is still column, not X~,
        Xt = X.t();

        cout<< X.n_rows <<" <- rows; cols -> "<< X.n_cols << endl;
        //cout<< X.col( 0 );   //here the X.col(5) out of bound

        //2. get M
        p_M = new Mat<double>( numSamples , numSamples );

        for( size_t i=0 ; i < numSamples ; i++ )                      //May be I can use armadillo to improve this Mij is symmetric, can prove it
        {
            for( size_t j=i ; j < numSamples ; j++ )
            {
                p_M->at(i,j) = p_w->at(array[i],array[j]) * (p_s->at(array[i],array[j])-lamda);
                p_M->at(j,i) = p_M->at(i,j);
            }
        }

        //3. find p~  because XMXt is real and symmetry so p must be real
        vec eigval;
        mat eigvec;

        //cout<< Xt * ( *p_M ) * X << endl;
        eig_sym(eigval, eigvec, Xt * (*p_M) * X );
            //since the eig_sym The eigenvalues are in ascending order
            //The largest eigenvalue of XMXt is the numSamplesth one, in C++, the numSampes - 1
        Col<double> p = eigvec.col( eigvec.n_cols - 1);         //the p is a col vec

        //cout<< eigval << endl;
        //cout<< p << endl;

        //if criteria in (17) increases then split l into l1 and l2; enqueue l1 and l2
        unsigned int * l , * r;

        l = (cLeaf->leafL).lFruit;
        r = (cLeaf->leafL).rFruit;                          //will l == r??

        while( l <= r )
        {
            //cout<< *l <<endl;
            //cout<< p_x->row( (unsigned int) *l ) << endl;
            //cout<< p << endl;

            mat ZZ = ( p_x->row( (unsigned int) *l ) * p );           //read the armadillo document

            //cout<< ZZ <<endl;

            if(  ZZ.at(0) > 0 )
            {
                l++;
            }
            else
            {
                unsigned int tmp;
                tmp = *r;
                *r = *l;
                *l = tmp;
                r--;
            }
        }     //ok here lFruit~r is left child ; l~rFruit is richt child

        double lJ, rJ;

        //calculate lJ
        lJ = findJ( p_s , p_w , lamda , ( (cLeaf->leafL).lFruit ) , r - ( (cLeaf->leafL).lFruit ) + 1 );


        //calculate rJ
        rJ = findJ( p_s , p_w , lamda , l , ( (cLeaf->leafL).rFruit) - l + 1 );


        // lJ + rJ > J ?
        if( (( lJ + rJ ) - (cLeaf->leafL).J ) > 0.0001  )  //use this compare to avoid error
        {
            //need to split the node, the make this node become a internal node,
            //change this to and internal node
            unsigned int * ll, * lr;
            unsigned int * rl, * rr;

            ll = (cLeaf->leafL).lFruit;
            lr = r;

            rl = l;
            rr = (cLeaf->leafL).rFruit;

            cLeaf->setInternal();

            cLeaf->intL.left = cLeftChild = new TreeNode;

            cLeftChild->setLeaf();
            cLeftChild->leafL.J = lJ;
            cLeftChild->leafL.lFruit = ll;
            cLeftChild->leafL.rFruit = lr;

            cLeaf->intL.right = cRightChild = new TreeNode;

            cRightChild->setLeaf();
            cRightChild->leafL.J = rJ;
            cRightChild->leafL.lFruit = rl;
            cRightChild->leafL.rFruit = rr;

            cLeaf->intL.pvector = new vector< Col<double> >;
            (cLeaf->intL.pvector)->push_back( p );

            //enqueue l1 and l2
            qu_p.push( cLeftChild );
            qu_p.push( cRightChild );

        }
        else
        {
            //this is a leaf node forever, link it to the leafnode list, for the future's sake.
            leafLink.push_back( cLeaf );
        }



        delete p_indices;
        delete p_M;
    }


    return true;
}


Tree::~Tree() {

    //deleteing the root may recursively delete the whole tree
    rmHelp( root );

    //delete fruit
    delete []fruit;
}

//private remove the tree function
//recursively delete the tree node
bool Tree:: rmHelp( TreeNode * rNode )
{
    if( rNode == NULL )
        return true;

    if( rNode->isInternal() )
    {
        //delete (rNode->intL).pvector; don't delete the pvector here because the delete rNode will do it.

        rmHelp( (rNode->intL).left );
        rmHelp( (rNode->intL).right );

        delete rNode;
    }
    else{
        delete rNode;
    }

    return true;
}


double Tree:: findJ( const Mat<char> *p_s ,const Mat<double> *p_w , const double lamda , const unsigned int* array , const size_t num )
{
    double sum;
    
    sum = 0;
    
    size_t i,j;
    for( i=0 ; i < num ; i++ )
    {
        for( j=0 ; j < num ; j++ )
        {
            sum += p_w->at( array[i] , array[j] ) * ( p_s->at( array[i] , array[j]) - lamda );
        }
    }
    
    return sum;
}

//findCi function
//input: the pointer to similarity matrix  : p_s
//       the lamda
//retval: double, the weight of the tree ci
double Tree:: findCi( const Mat<char> *p_s , const double lamda )
{
    int p11,p10;
    double c;
    unsigned int * array;
    size_t num;

    std:: list < TreeNode *> ::iterator itb ;
    std:: list < TreeNode *> ::iterator ite ;

    p11 = p10 = 0;

    cout << leafLink.size() << endl;

    itb = leafLink.begin();
    ite = leafLink.end();

    for(  ; itb != ite ; itb++ )
    {
        array = (*itb)->leafL.lFruit;
        num = (*itb)->leafL.rFruit - array + 1;

        for( int ii=0 ; ii < num ; ii++ )
        {
            cout<<sampleClass.at( array[ii] )<<" ";
        }
        cout<<endl;

        for( size_t i=0 ; i<num ; i++ )
        {
            for( size_t j=0 ; j<num ; j++ )
            {
                if( p_s->at( array[i] , array[j] ) == 1 )
                {
                    p11 ++;
                }else
                {
                    p10 ++;
                }
            }
        }

    }

    //ln function in cmath is ? <--  double log( double x )
    c = log( (1-lamda) *p11 /( lamda * p10 + EPS));     //becasue the p10 may be 0, which is bad!

    cm = c;

    return cm;
}

//updateWeights Function
//input: pointer to the weight matrix : p_w
//       pointer to similarity matrix : p_s
//       lamda
//retval: bool, true for update succeed; false for update failure
bool Tree:: updateWeights( Mat<double> *p_w , const Mat<char> *p_s , const double lamda )
{
    //zij = sij - lamda
    //cm
    //tm(xi,xj)         M_E is e!!
    
    unsigned int * array;
    size_t num;

    std:: list < TreeNode *> ::iterator itb ;
    std:: list < TreeNode *> ::iterator ite ;

    itb = leafLink.begin();
    ite = leafLink.end();

    for(  ; itb != ite ; itb++ )
    {
        array = (*itb)->leafL.lFruit;
        num = (*itb)->leafL.rFruit - array + 1;

        for( size_t i=0 ; i<num ; i++ )
        {
            for( size_t j=0 ; j<num ; j++ )
            {
                p_w->at( array[i] , array[j] ) = p_w->at( array[i] , array[j] ) * pow( M_E , -1 * (p_s->at(array[i] , array[j] ) - lamda) * cm );
            }
        }

    }

    return true;
}

bool Tree:: findImage( const Row<double> * p_sample , double * array )
{
    TreeNode * p;
   
    p = root;

    while( ( p != NULL ) && ( p->isInternal() ) )
    {
        mat ZZ = ((*p_sample) * ((p->intL).pvector)->at(0) );

        if( ZZ.at(0) > 0 )
        {
            p = p->intL.left;
        }
        else
        {
            p = p->intL.right;
        }
    }

    if( ( NULL == p )  || ( p->isInternal() ) )
        return false;

    unsigned int * beg;
    unsigned int * end;

    beg = p->leafL.lFruit;
    end = p->leafL.rFruit;

    for( ; beg <= end ; beg++ )
    {
        array[ *beg ] += array[ *beg ] + cm;
    }


    return true;
}

//return this tree's cm
double Tree:: getCm( void )
{
    return cm;
}

//add database items into this tree, be careful!
//input: the database items pointer p_d
//retval: bool; true--> add all of them ok
//              false--> something goes wrong
bool Tree:: addDatabaseItems( const Mat<double> * p_d )
{
    size_t r,c;
    TreeNode * current;
    Mat<double> result;

    r = p_d->n_rows;
    c = p_d->n_cols - 1 ;

    Row< double > tmp;
    for( size_t i = 0 ; i < r ; i ++ )
    {
        tmp = p_d->row( i );

        tmp.at( c ) = 1;    //the x sample needs to become this x=[x0 x1 x2 ... 1 ]

        current = root;

        while( (current != NULL) && ( current->isInternal() ))
        {
            result = tmp * (current->intL).pvector->at(0) ;

            if( result.at( 0 ) > 0 )
            {
                current = current->intL.left;
            }else
            {
                current = current->intL.right;
            }
        }

        if( NULL == current )
        {
            cerr <<"current is null in the addDatabaseIntems()\b";
            return false;
        }

        if( NULL == current->leafL.puivector )
        {
            current->leafL.puivector = new vector< unsigned int >;
        }

        (current->leafL).puivector->push_back( i );

    }

    return true;
}


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

using namespace std;
using namespace arma;

Tree::Tree( const size_t numOfSamples ) {

    root = NULL;
    numFruit = numOfSamples;
    fruit = new int[numFruit];

    for( int i=0 ; i<numFruit ; i++ )
    {
        fruit[i]=i;                    //the index to the sample in the matrix. There are numFruit of samples.
    }

}

//Search Tree Construction: grow
//Input: p_x -> address of the sample matrix
//       p_s -> address of the similarity matrix
//       p_w -> address of the weight matrix
//       lamda -> zij need him
//retval: true -> tree grows succeed; false -> tree grows fail
bool Tree:: grow( const Mat<double> *p_x ,const Mat<char> *p_s ,const Mat<double> *p_w , const double lamda )
{
    queue< TreeNode * > qu_p;
    Mat<double> myZero = zeros< Mat<double> >(1,1);

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
        size_t numSamples = (cLeaf->leafL).rFruit - (cLeaf->leafL).lFruit + 1;
        array =(unsigned int *)( (cLeaf->leafL).lFruit );
        
        p_indices = new Col< uword >( array , (uword)numSamples , true , true ); //may improve this

        X = p_w->rows( *p_indices );           //X is still column, not X~,
        X = X.t();

        //2. get M
        p_M = new Mat<double>( numSamples , numSamples );

        for( size_t i=0 ; i < numSamples ; i++ )                      //May be I can use armadillo to improve this
        {
            for( size_t j=0 ; j < numSamples ; j++ )
            {
                p_M->at(i,j) = p_w->at(array[i],array[j]) * (p_s->at(array[i],array[j])-lamda);
            }
        }

        //3. find p~  because XMXt is real and symmetry so p must be real
        vec eigval;
        mat eigvec;

        eig_sym(eigval, eigvec, X * (*p_M) * X.t() );
            //since the eig_sym The eigenvalues are in ascending order
            //The largest eigenvalue of XMXt is the numSamplesth one, in C++, the numSampes - 1
        Col<double> p = eigvec.col( numSamples - 1);         //the p is a col vec


        //if criteria in (17) increases then split l into l1 and l2; enqueue l1 and l2
        int * l , * r;

        l = (cLeaf->leafL).lFruit;
        r = (cLeaf->leafL).rFruit;                          //will l == r??

        while( l <= r )
        {
            umat ZZ = ( p_x->row( (unsigned int) *l ) * p ) > myZero;           //read the armadillo document

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
        if( ( lJ + rJ ) > (cLeaf->leafL).J  )
        {
            //need to split the node, the make this node become a internal node,
            //change this to and internal node
            int * ll, * lr;
            int * rl, * rr;

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

bool Tree:: rmHelp( TreeNode * rNode )
{
    if( rNode == NULL )
        return true;

    if( rNode->isInternal() )
    {
        delete (rNode->intL).pvector;

        rmHelp( (rNode->intL).left );
        rmHelp( (rNode->intL).right );
    }
    else{
        delete rNode;
    }

    return true;
}


double Tree:: findJ( const Mat<char> *p_s ,const Mat<double> *p_w , const double lamda , const int* array , const size_t num )
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

double Tree:: findCi( const Mat<char> *p_s , const double lamda )
{
    //ln function in cmath is ? <--  double log( double x )?



    return 0;
}


/*
 X不用对称都可以。 M=UDU', 令Y=UD^(1/2),这M=YY'

则XMX' = XYY'X' = XY*(XY)' 这个是对称的 
 */


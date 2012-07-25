/* 
 * File:   Tree.cpp
 * Author: alpha
 * 
 * Created on July 23, 2012, 8:54 AM
 */

#include "Tree.h"

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

}


Tree::~Tree() {

    //deleteing the root may recursively delete the whole tree

    delete []fruit;
}


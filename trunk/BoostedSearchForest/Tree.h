/* 
 * File:   Tree.h
 * Author: alpha
 *
 * Created on July 23, 2012, 8:54 AM
 */

#ifndef TREE_H
#define	TREE_H

#include <cstdlib>
#include <iostream>
#include <armadillo>
#include <list>

#include "TreeNode.h"

using namespace std;
using namespace arma;

class Tree {
public:
    Tree( const size_t numOfSamples );
    virtual ~Tree();
    bool grow( const Mat<double> *p_x ,const Mat<char> *p_s ,const Mat<double> *p_w , const double lamda );
    double findCi( const Mat<char> *p_s , const double lamda );
    bool updateWeights( Mat<double> *p_w , const Mat<char> *p_s , const double lamda );
    bool findImage( const Row<double> * p_sample , double * array );
private:

    double findJ( const Mat<char> *p_s ,const Mat<double> *p_w , const double lamda , const unsigned int* array , const size_t num );
    bool rmHelp( TreeNode * rNode );

    double cm;         //the weight c of this tree  it is stored in the forest too
    TreeNode * root;     //the root of this tree
    unsigned int * fruit;         //pointer to the index of the whole samples --> this may be replace the sample rather than its index here
    size_t numFruit;     //how many fruits we have

    list< TreeNode * > leafLink;

    static Mat<double> myZero;

};

#endif	/* TREE_H */


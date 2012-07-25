/* 
 * File:   Tree.h
 * Author: alpha
 *
 * Created on July 23, 2012, 8:54 AM
 */

#ifndef TREE_H
#define	TREE_H

#include <cstdlib>
#include "TreeNode.h"
using namespace std;

class Tree {
public:
    Tree( const size_t numOfSamples );
    virtual ~Tree();
private:
    //double cm;         //the weight c of this tree  it is stored in the forest so useless
    TreeNode * root;     //the root of this tree
    int * fruit;         //pointer to the index of the whole samples --> this may be replace the sample rather than its index here
    size_t numFruit;     //how many fruits we have
};

#endif	/* TREE_H */


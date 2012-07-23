/* 
 * File:   TreeLeafNode.h
 * Author: alpha
 *
 * Created on July 23, 2012, 9:28 AM
 */

#ifndef TREELEAFNODE_H
#define	TREELEAFNODE_H

#include <vector>
#include <iostream>

#include "TreeNode.h"
#include "TreeInternalNode.h"

using namespace std;


class TreeLeafNode : public TreeNode{
public:
    TreeLeafNode();
    virtual ~TreeLeafNode();
private:
    TreeInternalNode * p_father;     //this leaf node has a father, point to him
    double j;                        // the J(k) in (17 BSF) of this leaf
    int * fruit_l;                   // This implementation use the index of the fruit array
    int * fruit_r;                   // means this leaf store fruit_l-> [] .... [] <- fruit_r


};

#endif	/* TREELEAFNODE_H */


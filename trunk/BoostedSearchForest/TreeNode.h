/* 
 * File:   TreeNode.h
 * Author: alpha
 *
 * Created on July 23, 2012, 9:23 AM
 */

#ifndef TREENODE_H
#define	TREENODE_H

//#define LEAF_NODE 0
//#define INTERNAL_NODE 1
#include <iostream>
#include <vector>
#include <armadillo>


using namespace std;
using namespace arma;

enum NodeType{ LEAF , INTERNAL };

class TreeNode {
public:
    TreeNode();
    virtual ~TreeNode();
    bool isInternal( void );
    bool isLeaf( void );

    NodeType myType;
    
    union{
        struct{
            TreeNode *left;
            TreeNode *right;
            vector< Row<double> > * pvector;
        }intL;

        struct{
            int * lFruit;
            int * rFruit;
            double J;
        }leafL;
    };

};

#endif	/* TREENODE_H */


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

#include <vector>
#include <iostream>
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
    bool setLeaf( void );
    bool setInternal( void );

    NodeType myType;
    
    union{
        struct{
            TreeNode *left;
            TreeNode *right;
            vector< Col<double> > * pvector;  //here must be proved
        }intL;

        struct{
            int * lFruit;
            int * rFruit;
            double J;
        }leafL;
    };

};

#endif	/* TREENODE_H */


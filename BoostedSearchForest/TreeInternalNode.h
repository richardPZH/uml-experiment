/* 
 * File:   TreeInternalNode.h
 * Author: alpha
 * IMS@SCUT Once
 * Created on July 23, 2012, 9:28 AM
 */

#ifndef TREEINTERNALNODE_H
#define	TREEINTERNALNODE_H

//#define NumOfHyperplane 1         //how many hyperplanes we store in the interalnode, default is 1

#include "TreeNode.h"
#include <vector>
#include <armadillo>

using namespace std;
using namespace arma;

class TreeInternalNode : public TreeNode{
public:
    TreeInternalNode();
    virtual ~TreeInternalNode();

private:
    std::vector< Row<double>   > p_vector;
    //well, use Row<double>  or Row<cx_double> ??? ask someone; we store the p~ not p, (p~)t * x~ > 0  (p~)t is 1*n

    TreeNode * leftChild;
    TreeNode * rightChild;

};

#endif	/* TREEINTERNALNODE_H */


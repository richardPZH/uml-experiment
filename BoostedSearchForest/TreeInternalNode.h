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
    std::vector< Col<double>  > p_vector;  //well, use Col<double>  or Col<cx_double> ??? ask someone

    TreeNode * leftChild;
    TreeNode * rightChild;

};

#endif	/* TREEINTERNALNODE_H */


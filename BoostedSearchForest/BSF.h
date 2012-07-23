/* 
 * File:   BSF.h
 * Author: alpha
 *
 * Created on July 23, 2012, 8:49 AM
 */

#ifndef BSF_H
#define	BSF_H

#include "Tree.h"
#include <iostream>
#include <armadillo>

using namespace std;
using namespace arma;

class BSF {
public:
    BSF();
    virtual ~BSF();
private:
    size_t m;      //there are m trees in the forest here m must >= 0
    Tree * forestEntrance;   //Entrance of the forest, in fact is a place where trees reside
    Mat< char > * s;
    Mat< double > * x;

};

#endif	/* BSF_H */


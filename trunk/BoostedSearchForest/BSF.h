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
    size_t m;                        //there are m trees in the forest here m must >= 0 so size_t
    double lamda;                    //the λ the tuning paramemter, balances the retrieval quality and computational cost, need to ask the author Zhen Li
    Tree * forestEntrance;           //Entrance of the forest, in fact is a place where trees reside
    const Mat< char > * p_s;         //a pointer to the similarity matrix [ s11 s12 ... s1n ; s21 s22 .. s2n ; .... ; sn1 sn2 ... snn ] the similarity matrix's diagno is 1 and symmetrical
    const Mat< double > * p_x;       //a pointer to the original sample matrix [ x11 x12 x13 .. x1k ; x21 x22 x23 ..x2k ; ... ; xn1 xn2 xn3 ... xnk ]


};

#endif	/* BSF_H */


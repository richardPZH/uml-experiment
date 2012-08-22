/* 
 * File:   Preprocess.h
 * Author: alpha
 *
 * This head file provides some preprocess function to read and generate sample matrix need by the BSF class
 *
 * Created on August 8, 2012, 7:15 AM
 */

#ifndef PREPROCESS_H
#define	PREPROCESS_H

#include <iostream>
#include <armadillo>

using namespace std;
using namespace arma;

extern Col<double> sampleClass;
//This is a little bit different from #define, still need to recompile the whole source!!!
static const double fragment_q = 0.1;
static const double fragment_b = 0.4;
static const double fragment_d = 0.5 ; //1 - fragment_q - fragment_d <-- this is error!!!

//generate queries build database matrix
//Input: the sample file name
//       the address of a pointer, will new a Mat<double> to store the queries matrix
//       the address of a pointer, will new a Mat<double> to store the build matrix
//       the address of a pointer, will new a Mat<char> to store the similarity matrix of build
//       the address of a pointer, will new a Mat<double> to store the database
//       the address of a pointer, will new a int array to index to the original matrix
//retval: true, generate ok; false, generate fail
//Require: It's user's responsibility to free the p_q p_x p_s p_d iArray
bool generateQBS( const char * infile , Mat<double> ** p_q , Mat<double> ** p_x , Mat<char> ** p_s ,  Mat<double> ** p_d ,unsigned int ** iArray);


#endif	/* PREPROCESS_H */


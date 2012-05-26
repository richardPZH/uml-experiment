/* 
 * File:   Apriori.h
 * Author: IMS@SCUT
 *
 * Created on 2012年5月22日, 下午5:55
 */

#ifndef APRIORI_H
#define	APRIORI_H

#include "DBreader.h"
#include <list>
#include <vector>

using namespace std;

typedef struct{
    vector<int> avt;
    int count;
}FreqPattCnt;

class Apriori
{
public:
    void generate( DBreader * p_dbr , const double min_support_rate );
    Apriori( void ){ c = new list< FreqPattCnt > ; p = new list < FreqPattCnt > ; };
    
private:
    list< FreqPattCnt > *c;
    list< FreqPattCnt >*p;
};


#endif	/* APRIORI_H */


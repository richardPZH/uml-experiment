/* 
 * File:   Apriori.h
 * Author: IMS@SCUT
 *
 * Created on 2012年5月22日, 下午5:55
 */

#ifndef APRIORI_H
#define	APRIORI_H

#include "DBreader.h"

class Apriori
{
public:
    void generate( DBreader * p_dbr , double min_support_rate );
    
private:
    
};


#endif	/* APRIORI_H */


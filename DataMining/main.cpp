/* 
 * File:   Apriori.cpp
 * Author: IMS@SCUT
 *
 * Created on 2012年5月20日, 下午1:41
 */

#include <cstdlib>
#include <iostream>
#include <algorithm>

#include "DBreader.h"
#include "StoreRow.h"
#include "DBreaderImp.h"
#include "Apriori.h"

#define PROGRAM_NAME "Apriori"
#define DATA_FILE "E:\\c\\DataMining\\generateData\\dist\\Debug\\Cygwin-Windows\\itemSet"

using namespace std;

/*
 * 
 */

void print_usage( void );

int main(int argc, char** argv) {
    
//    if( argc < 2 )
//    {
//        print_usage();
//        exit( 1 );
//    }
    
    double min_support;
        
    min_support = 0.22; //atoi( argv[1] ) * 1.0 / 100 ;
    
    DBreader * dbreader = new DBreaderImp( DATA_FILE );                 //try to implement some of those rules I learnt over the years
  
    Apriori ap;
    
    ap.generate( dbreader , min_support );
    
    delete dbreader;
        
    return 0;
}

void print_usage( void )
{
    cerr<<PROGRAM_NAME<<" usage:\n";
    cerr<<PROGRAM_NAME<<" precent_of_support(e.g 30 40 60)\n";
}

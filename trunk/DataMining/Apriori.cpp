/* 
 * File:   Apriori.cpp
 * Author: IMS@SCUT
 *
 * Created on 2012年5月20日, 下午1:41
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <set>
#include <algorithm>


#define PROGRAM_NAME "Apriori"
#define DATA_FILE "E:\\c\\DataMining\\generateData\\dist\\Debug\\Cygwin-Windows\\itemSet"

using namespace std;

/*
 * 
 */
void print_usage( void );

int main(int argc, char** argv) {
    
    if( argc < 2 )
    {
        print_usage();
        exit( 1 );
    }
    
    ifstream ifile;
    ifile.open( DATA_FILE , ios::in );
    //default is txt file right...but to me they are just squence of bytes
    
    if( !ifile )
    {
        cerr<<"Open file "<<DATA_FILE<<" Failed\n";
        exit(1);
    }
    
    multiset<int> c1;
    
    int numEachRow;
    int tmp;
    while( ifile >> numEachRow )
    {
        for( ; numEachRow >0 ; numEachRow-- )
        {
            ifile >> tmp;
            c1.insert( tmp );
        }
    }
    
    
    for( multiset<int>::iterator p=c1.begin(); p != c1.end() ;++p )
    {
        cout<<*p<<" times : "<<c1.count( *p )<<endl;
    }
        
     return 0;
}

void print_usage( void )
{
    cerr<<PROGRAM_NAME<<" usage:\n";
    cerr<<PROGRAM_NAME<<" precent_of_support(e.g 30 40 60)\n";
}

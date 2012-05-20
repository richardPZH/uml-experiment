/* 
 * File:   generator.cpp
 * Author: IMS@SCUT
 *
 * Created on 2012年5月20日, 下午1:54
 */

#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sys/types.h>
#include <Math.h>

using namespace std;

#define PROGRAM_NAME "generator"
#define G_FILE_NAME "itemSet"
/*
 * 
 */
void print_usage( void );

int main(int argc, char** argv) {

	if( argc < 4 )
	{
		print_usage();
		exit( -1 );
	}
		
	int rows,max_length,num_items;

	rows = atoi( argv[1] );
	max_length = atoi( argv[2] );
	num_items = atoi( argv[3] );

	cout<<"rows = "<<rows<<"   max_length="<<max_length<<"   num_items="<<num_items<<endl;

	if( rows <=0 || max_length <=0 || ( max_length > num_items))
	{
		print_usage();
		exit( 1 );
	}

	//every things is check now we can generate random rows
        ofstream ofile;
        
        ofile.open( G_FILE_NAME , ios::out );          //remember ios::out will erase the file first
        if( !ofile )
        {
            cerr<<"File open fail "<<" FILE: "<<__FILE__<<" LINE: "<<__LINE__<<endl;
            exit( 1 );            
        }
        
              
	srand((unsigned) time(NULL));

        int tansItems;
        //use the vector the store num 1...num_items
        vector<int> v_array;
        for( ; num_items>0 ; num_items-- )
        {
            v_array.push_back( num_items );
        }
        
        for( ; rows>0 ; rows-- )
        {
            tansItems = rand() % max_length + 1;     //now tansItems from 1..maxlength
            
            //now we have a random accessable bottle
            vector<int>::iterator start,end;
            start = v_array.begin();
            end = v_array.end();                 //begin end  not begin back
            
            random_shuffle( start , end );
            
            ofile<<tansItems;
            for( ; tansItems>0 ; tansItems-- )
            {
                ofile<<" "<<v_array.at( tansItems-1 );
            }
            
            ofile<<endl;
        }
        
        
        ofile.close();
        
    return 0;
}

void print_usage( void )
{
	cerr<<PROGRAM_NAME<<" usage :\nThis is a transacton generator;\n";	
	cerr<<PROGRAM_NAME<<" rows_of_transaction max_length_of_each_row kinds_of_items\n";
	cerr<<"This version of generator will not generate duplicate items\n";
}

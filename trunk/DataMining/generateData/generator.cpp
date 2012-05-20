/* 
 * File:   generator.cpp
 * Author: IMS@SCUT
 *
 * Created on 2012年5月20日, 下午1:54
 */

#include <cstdlib>
#include <iostream>
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

	if( argc < 3 )
	{
		print_usage();
		exit( 1 );
	}
		
	int rows,max_length;

	rows = atoi( argv[1] );
	max_length = atoi( argv[2] );

	cout<<"rows = "<<rows<<"   max_length="<<max_length<<endl;

	if( rows <=0 || max_length <=0 )
	{
		print_usage();
		exit( 1 );
	}

	//every things is check now we can generate random rows
	ofstream outfile;

	srand((unsigned) time(NULL));

    return 0;
}

void print_usage( void )
{
	cerr<<PROGRAM_NAME<<" usage :\nThis is a transacton generator;\n";	
	cerr<<PROGRAM_NAME<<" num_of_transaction max_length_of_each_row\n";
	cerr<<"This version of generator will not generate duplicate items\n";
}

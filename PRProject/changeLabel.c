#include "mex.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FILE_NAME_INPUT "yeast.data"
#define FILE_NAME_OUTPUT "yeast.out"

typedef enum 
{
	CYT = 1,ERL,EXC,ME1,ME2,ME3,MIT,NUC,POX,VAC
}Localization;

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) 
{
	//Start process the file 
	char name[100];           //shall be enough no arbitiry limit
	double value;
	FILE * iFile;
	FILE * oFile;
	int i;
	Localization location_type;

	iFile = oFile = NULL;

	if( ( iFile = fopen( FILE_NAME_INPUT , "r") ) == NULL )
	{
		mexPrintf("Open %s Failed" , FILE_NAME_INPUT );
		exit( 1 );
	}

	if( ( oFile = fopen( FILE_NAME_OUTPUT , "w") ) == NULL )
	{
		mexPrintf("Open %s Failed" , FILE_NAME_INPUT );
		exit( 1 );
	}
		
	while( fscanf( iFile , "%s" , name ) != EOF ) //unused sequence name
	{

		for( i=0 ; i<8 ; i++ )
		{
			fscanf( iFile , "%lf" , &value );
			fprintf( oFile , "%lf " , value );
		}
		
		fscanf( iFile , "%s" , name );

		location_type = 0;
		
		//We can do better than this
		if( strcmp( name , "CYT" ) == 0 )
		{
			location_type=CYT;
		}
		else if( strcmp( name , "ERL" ) == 0 )
		{
			location_type=ERL;
		}
		else if( strcmp( name , "EXC" ) == 0 )
		{
			location_type=EXC;
		}
		else if( strcmp( name , "ME1" ) == 0 )
		{
			location_type=ME1;
		}
		else if( strcmp( name , "ME2" ) == 0 )
		{
			location_type=ME2;
		}
		else if( strcmp( name , "ME3" ) == 0 )
		{
			location_type=ME3;
		}
		else if( strcmp( name , "MIT" ) == 0 )
		{
			location_type=MIT;
		}
		else if( strcmp( name , "NUC" ) == 0 )
		{
			location_type=NUC;
		}
		else if( strcmp( name , "POX" ) == 0 )
		{
			location_type=POX;
		}
		else if( strcmp( name , "VAC" ) == 0 )
		{
			location_type=VAC;
		}
			

		fprintf( oFile , "%d\n" , (int) location_type );
	}

	fclose( iFile );
	fclose( oFile );

	//
	mexPrintf("OK!\n"); 
}


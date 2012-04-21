//2012/2/10 IMS@SCUT
//retrun a char from a number
//0 -> a 
//1 -> a?
//

#include <stdio.h>
#include <stdlib.h>

int main( int argc , char ** argv )
{
    int num;

    int i;
    
    if( argc < 2 )
	return 0;
    
    for( i=1 ; i < argc ; i++ )
    {
	printf("%c", atoi( argv[i] ) - 1 + 'a');	
    }

    return 0;

}

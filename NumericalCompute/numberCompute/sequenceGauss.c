//2012/04/30 IMS@SCUT Once Numberal computing
//Kind of sad..

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define MATRIX_SIZE 50
#define PROGRAM_NAME "SequenceGauss"

void print_usage( void );
void sequenceGauss( double m[][MATRIX_SIZE] , const int n , const double e);

int main( int argc , char ** argv )
{
    if( argc < 2 )
    {
	print_usage();
	exit( 1 );
    }

    FILE * pFile = NULL;
    double m[MATRIX_SIZE][MATRIX_SIZE];
    double t[MATRIX_SIZE*MATRIX_SIZE];
    size_t length;

    pFile = fopen( argv[1] , "r");

    length=0;
    while( EOF != fscanf( pFile , "%lf" , t+length ) )
    {
	//printf("%lf %lf\n" , x[length] , y[length] );
	length++;
    }
    
    if( 3 > length )
    {
	print_usage();
	exit(1);
    }

    int n;
    n =(int) ( -1 + sqrt( 4 * length - 3 ) ) /2;

    if( ( n * n + n + 1 ) != length )
    {
	fprintf( stderr , "Error File, row or coloum erroe!\n");
	exit(1);
    }

    //store in the matrix
    double e;
    int i;
    int j;
    int l;
    l = 0;
    for( i=0 ; i < n ; i++ )
    {
	for( j=0 ; j < n ; j++ )
	{
	    m[i][j] = t[l];
	    l++;
	}
    }

    for( i=0 ; i < n ; i++ )
    {
	m[i][n] = t[l];
	l++;
    }

    e = t[l];

    //now the matrix m and size n , e is usable
    
    sequenceGauss( m , n , e );

    

    (void) fclose( pFile );

    return 0;
}

void sequenceGauss( double m[][MATRIX_SIZE] , const int n , const double e)
{
    int k;
    int i;
    int j;

    double x[MATRIX_SIZE];

    for( k=0 ; k < n-1 ; k++ )
    {
	if( fabs( m[k][k] ) <= e )
	{
	    fprintf( stderr , "Sequence Gauss Can't solve it!");
	    return;
	}
	
	for( i=k+1 ; i < n ; i++ )
	{
	    double t = m[i][k] / m[k][k];
	    for( j=k+1 ; j<=n ; j++ )
	    {
		m[i][j] = m[i][j] - t * m[k][j];
	    }
	}
    }

    if( fabs( m[n-1][n-1] ) <= e )
    {
	fprintf( stderr , "Sequence Gausse Can't solve it!\n");
	return ;
    }

    x[n-1] = m[n-1][n] / m[n-1][n-1];
    double tSum;
    for( i=n-2 ; i>=0 ; i-- )
    {
	tSum=0;
	for( j=i+1 ; j<n ; j++ )
	{
	    tSum += m[i][j] * x[j];
	}

	x[i] = ( m[i][n] - tSum ) / m[i][i];

    }


    //now the x[...] store the answers print them out
    for( i=0 ; i<n ; i++ )
    {
	printf("X[%d] = %lf\n" , i , x[i]);
    }

}

void print_usage( void )
{
    fprintf( stderr , "%s Usage:\n",PROGRAM_NAME);
    fprintf( stderr , "%s inputFile\n" , PROGRAM_NAME );
}

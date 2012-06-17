//2012/04/30 IMS@SCUT Once
//Page 89 implement the Romberg
//

#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#define PROGRAM_NAME "Romberg"
#define MATRIX_SIZE 100

double p88( const double x);
double fsqrt( const double x);
double romberg( const double a , const double b , const double e , double ( * f ) ( const double x ));

int main( int argc , char ** argv )
{
    double a,b,e;

    printf("Please input a: ");
    scanf("%lf",&a);
    printf("Please input b: ");
    scanf("%lf",&b);
    printf("Please input e: ");
    scanf("%lf",&e);


    printf("Result : %lf\n" , romberg( a , b , e , fsqrt ));

    return 0;
}

double fsqrt( const double x)
{
    return sqrt( x );
}

double p88( const double x)
{
    return ( 4 / ( 1 + x * x ));
}

double romberg( const double a , const double b , const double e, double ( * f ) (const double x ))
{
    double T[MATRIX_SIZE][MATRIX_SIZE];
    size_t k;
    size_t c2k1;   //means 2^(k-1) 
    double temp;

    T[0][0] = ( b - a ) / 2 * ( f(a) + f(b) );

    k=1;    
    c2k1 = 1;

    while( 1 )
    {
	size_t i;
	temp=0;
	for( i=1; i<=c2k1 ; i++ )
	{
	    temp += f( a + ( 2 * i - 1 ) * ( b - a ) / ( c2k1 * 2 ) ); 
	}
	    
	T[0][k] = 0.5 * ( T[0][k-1] + ( b - a ) / c2k1 * temp );

	size_t m;
	int c4m=4;
	for( m = 1 ; m <= k ; m++ )
	{
	    T[m][k-m] =( c4m * T[m-1][k-m+1] - T[m-1][k-m] )/(c4m - 1);
	    c4m *=4;
	}

	if( fabs( T[k][0] - T[k-1][0] ) < e )
	{
	    //finish calcalating print the trangle
	    int i,j;
	    for( i=0 ; i <= k ; i++ )
	    {
		for( j=i ; j>=0 ; j-- )
		{
		    printf("%lf\t" , T[i-j][j] );
		}
		printf("\n");
	    }

	    return T[k][0];
	}

	k++;
	c2k1 *=2; //why?
    }

    return 0.0;   //to keep the compiler happy
}

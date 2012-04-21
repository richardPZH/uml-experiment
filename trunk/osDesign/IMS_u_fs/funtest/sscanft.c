//test sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);

#include <stdio.h>

int main( int argc , char ** argv )
{
    char buffer[200];
    char directory[200];
    char filename[200];
    char extension[200];

    printf("Input your string : ");
    while( scanf("%s",buffer) !=EOF )
    {
	directory[0]='\0';
	filename[0]='\0';
	extension[0]='\0';

	//sscanf( buffer , "%[^.].%s" , filename , extension );
	sscanf( buffer , "/%[^/]/%s", directory, filename);

	printf("directory : %s\n",directory);
	printf("filename  : %s\n",filename);
	printf("extension : %s\n",extension);

    	printf("Input your string : ");
    }

    return 0;

}

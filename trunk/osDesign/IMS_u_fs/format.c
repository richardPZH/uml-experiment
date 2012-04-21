/*
 * 2012/2/7 IMS@SCUT nemosail@gmail.com
 * format.c
 *
 * This file is used at the first time you create the diskimg file.
 * dd bs=1k count=5k if=/dev/zero of=/dev/diskimg
 * It will format the disk image file.
 *
 * You should also write a format program to init this file, i.e. write its super block and bitmap  blocks data .
 *
*/

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "Global.h"
#include "Super_block.h"
#include "Files.h"

#include "Block_manpulate.c"

#define PROGRAM_NAME "format"

void print_usage( void );
unsigned long get_file_size(const char *filename);

int main ( int argc , char ** argv )
{

    if( argc != 2 )
    {
	print_usage();
	return 0;
    }

    fprintf( stderr , "This program wil format the %s file, continue? y/n : ", argv[1]);

    char in;
    in = getchar();

    if( 'y' != in && 'Y' != in )
    {
	fprintf( stderr , "format cancelled!\n");
	return 0;
    }

    int fd;
    if( (fd = open( argv[1] , O_WRONLY )) < 0 )
    {
	fprintf( stderr , "Error on open():%s\n", strerror( errno ) );
	return 1;
    }

    struct sb tSuperBlock;
    
    //good habit is to memset
    memset( &tSuperBlock , 0 , sizeof( struct sb ) );

    tSuperBlock.fs_size = (long)get_file_size( argv[1] ) / BLOCK_SIZE;
    tSuperBlock.us_size = 1 + SUPER_BLOCK_N + BITMAP_BLOCK_N ;     //because the superblock and bitmapblock and root alread occupy some blocks
    tSuperBlock.bk_size = BLOCK_SIZE;
    tSuperBlock.first_blk = BLOCK_SIZE * ( SUPER_BLOCK_N + BITMAP_BLOCK_N );
    tSuperBlock.bitmap = BITMAP_BLOCK_N;

    printf("Disk image file:%s\n" , argv[1] );
    printf("Ready to Write :\n");
    printf("fs_size(in blocks)  : %ld\n", tSuperBlock.fs_size );
    printf("us_size(in blocks)  : %ld\n", tSuperBlock.us_size );
    printf("bk_size(in bytes )  : %ld\n", tSuperBlock.bk_size );
    printf("first_blk(location) : %ld\n", tSuperBlock.first_blk );
    printf("bitmap(in blocks)   : %ld\n\n", tSuperBlock.bitmap );


    //diskimage too small no way to do
    if( tSuperBlock.fs_size < (SUPER_BLOCK_N + BITMAP_BLOCK_N + 1 ) )
    {
	fprintf( stderr , "Diskimage file too small! Can't format, sorry!\n");
	exit(1);
    }

    //write super block
    lseek( fd , (off_t) 0 , SEEK_SET );
    
    ssize_t wc;
    struct u_fs_disk_block block;
    memset( &block , 0 , sizeof( struct u_fs_disk_block ) );
    memcpy( &block , &tSuperBlock , sizeof( struct sb ) );

    wc = write( fd , (void *) &block , sizeof( struct u_fs_disk_block ));
    if( wc != sizeof( struct u_fs_disk_block ))
    {
	fprintf( stderr , "Error on write():%s\n",strerror(errno));
	exit(1); //it will help me to close file ?
    }
    //super block done

    //write bitmap block
    lseek( fd , (off_t) (SUPER_BLOCK_N * BLOCK_SIZE) , SEEK_SET );
    char buffer[BLOCK_SIZE];
    (void *) memset((void *) buffer , 0 , BLOCK_SIZE);

    int count;
    for( count=0; count < tSuperBlock.bitmap ; count++ )
    {
	wc = write( fd , (void *)buffer , BLOCK_SIZE);
	if( wc != BLOCK_SIZE )
	{
	    fprintf( stderr , "Error on write():%s\n",strerror(errno));
	    exit(1); //it will help me to close file ?
	}
    }

    //the first block is occupy by the root dir
    lseek( fd , SUPER_BLOCK_N * BLOCK_SIZE , SEEK_SET );
    unsigned char oc = 0x80;
    write( fd , (void *)&oc , sizeof( oc ));
    //bitmap block done

    //write data block. no much to do. clear the root block
    struct u_fs_disk_block root;
    memset( &root , 0 , sizeof( root ));
    root.size=0;
    root.nNextBlock=0;

    lseek( fd , tSuperBlock.first_blk , SEEK_SET );
    write( fd , (void*)&root , sizeof( root));

    (void) close(fd);

    //read and comfirm
    if( (fd = open( argv[1] , O_RDONLY )) < 0 )
    {
	fprintf( stderr , "Error on open():%s\n", strerror( errno ) );
	return 1;
    }

    struct sb uSuperBlock;
    struct u_fs_disk_block ablock;

    read( fd , (void *)&ablock , sizeof( struct u_fs_disk_block ) );
    memcpy( &uSuperBlock , &ablock , sizeof( struct sb ) );

    printf("Disk image file:%s\n" , argv[1] );
    printf("After Writing :\n");
    printf("fs_size(in blocks)  : %ld\n", tSuperBlock.fs_size );
    printf("us_size(in blocks)  : %ld\n", tSuperBlock.us_size );
    printf("bk_size(in bytes )  : %ld\n", tSuperBlock.bk_size );
    printf("first_blk(location) : %ld\n", tSuperBlock.first_blk );
    printf("bitmap(in blocks)   : %ld\n\n", tSuperBlock.bitmap );

    if( 0 == memcmp( (void*)&tSuperBlock , (void*)&uSuperBlock , sizeof( struct sb) ))
    {
	printf("Check....OK. Format Complete!\n");
    }
    else
    {
	fprintf( stderr , "Check....FAILED. Format Failed!\n");
    }

    ( void ) close( fd );
    
    return 0;
}

void print_usage( void )
{
    fprintf( stderr , "Usage :\n\t%s Disk_Image_Name\n" ,  PROGRAM_NAME );
}

unsigned long get_file_size(const char *filename)
{

        struct stat buf;

	if(stat(filename, &buf)<0)
        {
	   return 0;
	}
	return (unsigned long)buf.st_size;
}

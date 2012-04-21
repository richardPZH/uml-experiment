/*
 * 2012/2/8 IMS@SCUT nemosail@gmail.com
 * Block_manpulate.c
 *
 * provide general fucntion to visit the formated image file(disk)
 * Life is tough....
 * Song of Myself
 *
*/

#ifndef IMS_BLOCK_MANPULATE_C
#define IMS_BLOCK_MANPULATE_C

#include "Global.h"
#include "Files.h"
#include "Directory.h"
#include "Block_manpulate.h"

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


#define DISK_IMAGE_NAME "diskimg"

//go to the given directory
//find the entry(given by the entry_name) and copy to the entry struct
//
//return value: 0 entry found and copy; <0 entry not found , no copy 
int u_fs_find_entry( long block_location, const  char * fname , struct u_fs_file_directory_entry * e )
{
    int res = -ENOENT;

    struct u_fs_disk_block block;
    struct u_fs_file_directory_entry *p;
    int rc;

    while( block_location != 0 )     //have next block?
    {
	rc = u_fs_read_block( &block , block_location );

	if( rc < 0 )
	{
	    fprintf(stdout,"*******Message******* read faild: %s , %d",__FILE__ , __LINE__ );
	    res = -ENOENT;
	    break;
	}

	block_location = block.nNextBlock;
	p = (struct u_fs_file_directory_entry *)block.data;  //it is a directory

	int count= block.size / sizeof( struct u_fs_file_directory_entry );
	int i;
	for( i=0; i<count ; i++ )
	{
	    if( p->fname[0] == '\0') //empty entry i--
	    {
		i--;
	    }
	    else
	    {
		if( strcmp( p->fname , fname ) == 0 )
		{
		    	res=0;
		        memcpy( e , p , sizeof(struct u_fs_file_directory_entry ));
		        block_location = 0;
		        break;
			
	    	}
	    }
	    p++;  //move forward to search another entry
	}
    }

    return res;
}

//u_fs_read_block
//read a block from the given location
//return 0 success, <0 fail
int u_fs_read_block( struct u_fs_disk_block *p , const long offset )
{
    int fd;

    if( offset < 0 )
	return -ENOENT;

    if( (fd=open( DISK_IMAGE_NAME , O_RDONLY )) < 0 )
    {
	fprintf( stderr , "Error on open()%s",strerror(errno));
	return -ENOENT;
    }

    lseek( fd , offset , SEEK_SET );

    int rc;
    rc = read( fd , p , sizeof( struct u_fs_disk_block ) );

    (void) close(fd);

    if( rc == sizeof( struct u_fs_disk_block ) )
	return 0;
    else
	return -1;
}

//u_fs_write_block
//write a block to the given location
//return 0 success, <0 fail
int u_fs_write_block( struct u_fs_disk_block *p , const long offset )
{
    int fd;

    if( offset < 0 )
	return -ENOENT;

    if( (fd=open( DISK_IMAGE_NAME , O_WRONLY )) < 0 )
    {
	fprintf( stderr , "Error on open()%s",strerror(errno));
	return -ENOENT;
    }

    lseek( fd , offset , SEEK_SET );

    int wc;
    wc = write( fd , p , sizeof( struct u_fs_disk_block ) );

    (void) close(fd);

    if( wc == sizeof( struct u_fs_disk_block ) )
	return 0;
    else 
	return -1;
}


//u_fs_get_entry
//follow the path and copy the entry
//return 0 success, <0 fail
int u_fs_get_entry( const char * path , struct u_fs_file_directory_entry * p )
{
    char drt[MAX_FILENAME + 2];
    char fln[MAX_FILENAME + MAX_EXTENSION + 2];
    char ext[MAX_EXTENSION +2];
    char * fname;

    struct u_fs_file_directory_entry entry;
    
    drt[0]='\0'; //zero initialize
    fln[0]='\0';
    ext[0]='\0';

    sscanf( path , "/%[^/]/%s", drt, fln );

    if( drt[0] == '\0' ) //  path is '/' error
	return -1;

    long dir_location;

    dir_location = ( SUPER_BLOCK_N + BITMAP_BLOCK_N ) * BLOCK_SIZE ;

    fname = drt;

    if( drt[0] != '\0' && fln[0] != '\0' )
    {   
        if( u_fs_find_entry( dir_location , drt ,  &entry ) < 0 ) 
            return -ENOENT;
        if( entry.flag != iDIRECTORY )
            return -ENOENT;

         dir_location = entry.nStartBlock;
	 fname = fln;
    }   

    return u_fs_find_entry( dir_location , fname , p );
}

//u_fs_find_next_avaiable_block
//return the location of next location of block
//return val >=0 success; <0 error
long u_fs_find_next_avaiable_block( void )
{
    int bitmat_offset;
    bitmat_offset = SUPER_BLOCK_N * BLOCK_SIZE;

    unsigned char bit8;
    long location;
    
    //superblock do we have enough space ?
    struct sb sb_info;
    (void) u_fs_read_superblock( &sb_info );
    if( ( sb_info.fs_size - sb_info.us_size ) < 1 )
	return -1;

    sb_info.us_size ++;
    (void) u_fs_write_superblock( &sb_info );

    int fd;
    if( (fd=open( DISK_IMAGE_NAME , O_RDWR )) < 0 )
    {
	fprintf( stderr , "Error on open()%s",strerror(errno));
	return -ENOENT;
    }

    lseek( fd , bitmat_offset , SEEK_SET );

    bit8 = 255;

    int count=0;
    int wb;

    while( bit8 >= 255 )
    {
	read( fd , &bit8 , sizeof( unsigned char ));
	count++;
    }

    if( (bit8 & 0x80) == 0 )
    {
	bit8 |= 0x80;
	wb=0;
    }
    else if( (bit8 & 0x40) == 0 )
    {
	bit8 |= 0x40;
	wb=1;
    }
    else if( (bit8 & 0x20) == 0 )
    {
	bit8 |= 0x20;
	wb=2;
    }
    else if( (bit8 & 0x10) == 0 )
    {
	bit8 |= 0x10;
	wb=3;
    }
    else if( (bit8 & 0x08) == 0 )
    {
	bit8 |= 0x08;
	wb=4;
    }
    else if( (bit8 & 0x04) == 0 )
    {
	bit8 |= 0x04;
	wb=5;
    }
    else if( (bit8 & 0x02) == 0 )
    {
	bit8 |= 0x02;
	wb=6;
    }
    else 
    {
	bit8 |= 0x01;
	wb=7;
    }

    lseek( fd , -1 , SEEK_CUR );
    write( fd , &bit8 , sizeof( unsigned char ));  //update the bitmap

    count--;
    location = 8 * count + wb ;
    location *= BLOCK_SIZE;
    location += ( SUPER_BLOCK_N + BITMAP_BLOCK_N ) * BLOCK_SIZE ;

    (void)close(fd);

    u_fs_initial_block( location );

    return location;
}

//u_fs_initial_block
//initial a block to.... block.szie =0 ; block.nNextBlock=0 ; block.data 0000
//return val 0 on success; <0 on fail
int u_fs_initial_block( long location )
{
    //when return a block location, this block location shall be clean
    struct u_fs_disk_block block;

    memset( &(block.data) , 0 , sizeof( MAX_DATA_IN_BLOCK ) );  //I'm kind of a greedy miser. Can be optimised.
    block.size = 0 ;
    block.nNextBlock = 0 ;

    u_fs_write_block( &block , location );

    return 0;
}

//u_fs_free_block
//when a block is no longer use free it, give it back to the system
//this will update the bitmap section
//return val 0 on success; <0 fail
int u_fs_free_block( long block_location )
{
    int shang;
    int yuShu;

    if( block_location < ((SUPER_BLOCK_N + BITMAP_BLOCK_N ) * BLOCK_SIZE ))
	return 0;   //this block can't be free

    block_location -= ( SUPER_BLOCK_N + BITMAP_BLOCK_N ) * BLOCK_SIZE ;
    block_location /= BLOCK_SIZE;

    shang = block_location / 8;
    yuShu = block_location % 8;

    int bitmat_offset;
    unsigned char bit8;

    bitmat_offset = SUPER_BLOCK_N * BLOCK_SIZE;
    bitmat_offset += shang;

    int fd;
    if( (fd=open( DISK_IMAGE_NAME , O_RDWR )) < 0 )
    {
	fprintf( stderr , "Error on open()%s",strerror(errno));
	return -ENOENT;
    }

    lseek( fd , bitmat_offset , SEEK_SET );

    read( fd , &bit8 , sizeof( unsigned char ));

    switch( yuShu )
    {
	case 0: bit8 &= 0x7F;break;
	case 1: bit8 &= 0xBF;break;
	case 2: bit8 &= 0xDF;break;
	case 3: bit8 &= 0xEF;break;
	case 4: bit8 &= 0xF7;break;
	case 5: bit8 &= 0xFB;break;
	case 6: bit8 &= 0xFD;break;
	case 7: bit8 &= 0xFE;break;
    }

    lseek( fd , bitmat_offset , SEEK_SET );

    int wc;
    wc = write( fd , &bit8 , sizeof( unsigned char ));
    
    (void) close( fd );

    //superblock us_size of used decrease by one
    struct sb sb_info;
    (void) u_fs_read_superblock( &sb_info );
    sb_info.us_size --;
    (void) u_fs_write_superblock( &sb_info );

    if( wc == sizeof( unsigned char ))
	return 0;
    else
	return -1;
}

//u_fs_insert_entry
//insert the given entry from the full path 
//e.g if path=/hello will insert a hello entry to the '/'
//    if path=/hello/world will insert a world entry to the '/hello' directory
//return val 0 on success; <0 on fail
int u_fs_insert_entry( const char * npath ,const struct u_fs_file_directory_entry *p )
{
    char drt[ MAX_FILENAME * 2 + 2];
    char fln[ MAX_FILENAME * 2 + 2];
    char ext[ MAX_FILENAME * 2 + 2];
    char path[ MAX_FILENAME * 2 + 2];

    drt[0]='\0';
    fln[0]='\0';
    ext[0]='\0';
    path[0]='/';
    path[1]='\0';

    sscanf( npath,"/%[^/]/%[^.].%s", drt , fln ,ext ); 
    if( strlen( ext ) > MAX_EXTENSION )
	return -ENAMETOOLONG;

    sscanf( npath, "/%[^/]/%s", drt, fln);

    strcpy( path+1 , drt );

    if( fln[0] == '\0' )
    {
	strcpy( path , "/" );
	sscanf( npath , "/%[^/]", fln );
    }

    long dir_location;
    struct u_fs_file_directory_entry entry;
    struct u_fs_file_directory_entry *array;


    if( strcmp( path , "/" ) == 0 )
    {
	dir_location = ( SUPER_BLOCK_N + BITMAP_BLOCK_N ) * BLOCK_SIZE ;
    }
    else
    {
	if(  u_fs_get_entry( path , &entry) < 0 )
	    return -ENOENT;

	dir_location = entry.nStartBlock;
    }

    struct u_fs_disk_block ablock;
    long final_location;

    while( dir_location != 0 )
    {
	u_fs_read_block( &ablock , dir_location );

	if( (MAX_DATA_IN_BLOCK - ablock.size) > sizeof( struct u_fs_file_directory_entry) ) //there is room for entry
	{
	    array = ( struct u_fs_file_directory_entry *)ablock.data;
	    while( array->fname[0] != '\0' )
		array++;
	    
	    memcpy( array , p , sizeof( struct u_fs_file_directory_entry ));

	    ablock.size+= sizeof( struct u_fs_file_directory_entry );

	    u_fs_write_block( &ablock , dir_location ); 

            return u_fs_increase_entry_fsize( path , 1 );
	}

	final_location = dir_location;
	dir_location = ablock.nNextBlock;
    }

    //if it comes here running out of room so new one 
    long new_location;

    new_location = u_fs_find_next_avaiable_block();

    u_fs_read_block( &ablock , final_location );

    ablock.nNextBlock = new_location;
 
    u_fs_write_block( &ablock , final_location ); 

    memset( &ablock , 0 , sizeof( struct u_fs_disk_block ) );
    ablock.size = sizeof( struct u_fs_file_directory_entry );
    ablock.nNextBlock = 0;
    array = (struct u_fs_file_directory_entry *)ablock.data;
    memcpy( array , p , sizeof( struct u_fs_file_directory_entry ));

    u_fs_write_block( &ablock , new_location);

    return u_fs_increase_entry_fsize( path , 1 );
}

//u_fs_remove_entry
//remove entry from the full path
//e.g if path=/hello. the hello entry will be remove from '/'
//    if path=/hello/ims. the ims entry will be remove from '/hello' directory
//return val 0 on success , <0 fail
int u_fs_remove_entry( const char * npath ,const struct u_fs_file_directory_entry * p )
{
    char drt[MAX_FILENAME + MAX_EXTENSION + 2];
    char fln[MAX_FILENAME + MAX_EXTENSION + 2];
    char ext[MAX_EXTENSION + 2];
    char path[ MAX_FILENAME * 2 + MAX_EXTENSION + 2];

    drt[0]='\0';
    fln[0]='\0';
    ext[0]='\0';
    path[0]='/';
    path[1]='\0';

    sscanf( npath, "/%[^/]/%s", drt, fln);

    strcpy( path+1 , drt );

    if( fln[0] == '\0' )
    {
	strcpy( path , "/" );
	sscanf( npath , "/%[^/]", fln );
    }

    long dir_location;
    struct u_fs_file_directory_entry entry;
    struct u_fs_file_directory_entry *pointer;

    if( strcmp( path , "/" ) == 0 )
    {
	dir_location = ( SUPER_BLOCK_N + BITMAP_BLOCK_N ) * BLOCK_SIZE ;
    }
    else
    {
	u_fs_get_entry( path , &entry );
	dir_location = entry.nStartBlock;
    }

    struct u_fs_disk_block block;
    long old_location;
    size_t size;
    int i;

    while( dir_location !=0 )
    {
	if( u_fs_read_block( &block , dir_location ) < 0 )
        {
	    fprintf( stderr , "Error on reading block():%s" , strerror(errno));
	    return -ENOENT;
        }

	old_location = dir_location;
	dir_location = block.nNextBlock;
	size = block.size;
	pointer = ( struct u_fs_file_directory_entry * ) block.data;
	
        fprintf(stderr,"*************Meaasge*********remove entry: block.size : %d , block.nNextBlock : %ld\n",size,dir_location );

	for( i=0; i< (size / sizeof( struct u_fs_file_directory_entry )); i++ )
	{
            fprintf(stderr,"*************Meaasge*********readdir: pointer->fname : %s\n",pointer->fname );

	    if( pointer->fname[0] == '\0' )
	    {
		i--;    
	    }
	    else if( strcmp( pointer->fname , p->fname ) == 0 )  //find it
	    {
		memset( pointer , 0 , sizeof( struct u_fs_file_directory_entry ));
		block.size -= sizeof( struct u_fs_file_directory_entry );    

		u_fs_write_block( &block , old_location ); 
		dir_location=0;
		break;
	    }
	    pointer++;
	}
    }

    return u_fs_increase_entry_fsize( path , -1 );

}

//u_fs_increase_entry_fsize
//increase or decrease the entry fsize for directory use
//the path shall like "/" "/aaa" 
//inc_dec > 0 increase , inc_dec < 0 decrease
//return val 0 on success; <0 on fail
int u_fs_increase_entry_fsize( const char *path , int inc_dec )
{
    if( strcmp( path , "/" ) == 0 )  // just return on root
	return 0;

    char drt[ MAX_FILENAME * 2];
    long dir_location;

    strcpy( drt , path+1 );
    
    dir_location = ( SUPER_BLOCK_N + BITMAP_BLOCK_N ) * BLOCK_SIZE ;

    struct u_fs_disk_block block;
    struct u_fs_file_directory_entry *p;
    long old_location;

    while( dir_location != 0 )
    {
	u_fs_read_block( &block , dir_location );

	old_location   = dir_location;
	dir_location = block.nNextBlock;

	p = (struct u_fs_file_directory_entry *)block.data;  //it is a directory

	int count= block.size / sizeof( struct u_fs_file_directory_entry );
	int i;
	for( i=0; i<count ; i++ )
	{
	    if( p->fname[0] == '\0') //empty entry i--
	    {
		i--;
	    }
	    else
	    {
		if( strcmp( p->fname , drt ) == 0 )
		{
		    p->fsize += inc_dec;
		    u_fs_write_block( &block , old_location );
		    dir_location = 0;
		    break;
		} 

	    }
	    p++;  //move forward to search another entry
	}
    }
    return 0;
}

//read the superblock info
//return 0 on success ; <0 on fail
int u_fs_read_superblock( struct sb *p )
{
    struct u_fs_disk_block block;

    u_fs_read_block( &block , 0 );

    memcpy( p , &block , sizeof( struct sb ) );
    
    return 0;

}

//u_fs_update_entry
//update the entry , from the given path 
//e.g.  /hello  the hello entry is updated. /hello/ims.c ims.c entry is update
//the path shall be the full path. so : /hello /hello/ims.c
//return val 0 on success; <0 on fail
int u_fs_update_entry( const char * path , const struct u_fs_file_directory_entry *p )
{
    if( strcmp( "/" , path ) == 0 )  //update the '/' entry. No way
	return -1;

    int rres;
    int ires;

    //simple approach.
    //First delete the old entry
    rres = u_fs_remove_entry( path , p );

    //Second insert back the entry
    ires = u_fs_insert_entry( path , p );

    //return the val
    if( rres < 0 || ires < 0 )
	return rres;
    else
	return 0;
}

//write the superblock info
//return 0 on success ; <0 on fail
int u_fs_write_superblock( struct sb *p )
{
    struct u_fs_disk_block block;

    memset( &block , 0 , sizeof( struct u_fs_disk_block ) );

    memcpy( &block , p , sizeof( struct sb ) );

    u_fs_write_block( &block , 0 );

    return 0;
}

/*
//test purpose //c need to struct struct_name
int main( int argc , char ** argv )
{
    return 0;
}
*/



#endif

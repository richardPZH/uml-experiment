/*
 * 2012/2/13 IMS@SCUT nemosail@gmail.com B8
 * Beautiful place, but we can not stay.
 * Block_manpulate.h
 *
 * This header will provide basic function of File operating in the fuse 
 * assignment.
 *
 *
*/

#ifndef BLOCK_MANPULATE_H
#define BLOCK_MANPULATE_H

#include "Global.h"
#include "Files.h"
#include "Directory.h"
#include "Super_block.h"

int u_fs_initial_block( long location );

int u_fs_write_superblock( struct sb *p );

int u_fs_read_superblock( struct sb *p );

int u_fs_read_block( struct u_fs_disk_block *p , const long offset );

int u_fs_write_block( struct u_fs_disk_block *p , const long offset );

int u_fs_increase_entry_fsize( const char *path , int inc_dec );

int u_fs_find_entry( long block_location, const  char * fname , struct u_fs_file_directory_entry * e );

int u_fs_get_entry( const char * path , struct u_fs_file_directory_entry * p );

long u_fs_find_next_avaiable_block( void );

int u_fs_free_block( long block_location );

int u_fs_insert_entry( const char * npath , const struct u_fs_file_directory_entry * p );

int u_fs_remove_entry( const char * npath , const struct u_fs_file_directory_entry * p );

int u_fs_increase_entry_fsize( const char *path , int inc_dec);

int u_fs_update_entry( const char * path , const struct u_fs_file_directory_entry * p );

#endif

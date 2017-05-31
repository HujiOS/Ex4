#include <cstdio>
#include <sys/stat.h>
#import "CacheFS.h"

using namespace std;

static int blksize;

int CacheFS_init(int blocks_num, cache_algo_t cache_algo,
                 double f_old , double f_new  ){
    struct stat fi;
    stat("/tmp", &fi);
    blksize = fi.st_blksize;

}



int CacheFS_destroy(){

}



int CacheFS_open(const char *pathname){

}


int CacheFS_close(int file_id){

}



int CacheFS_pread(int file_id, void *buf, size_t count, off_t offset){

}



int CacheFS_print_cache (const char *log_path){

}


int CacheFS_print_stat (const char *log_path){

}
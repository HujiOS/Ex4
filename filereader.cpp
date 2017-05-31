#include <stdio.h>
#include <cstdio>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string>
#include <unistd.h>

int main(int argc, char * argv[]){
    struct stat fi;
    stat("/tmp", &fi);
    size_t blksize = fi.st_blksize;
    int fd = open("test.txt", O_RDONLY|O_DIRECT|O_SYNC);
    char * buf = (char*)aligned_alloc(blksize, blksize);
    int num = read(fd ,buf, blksize);
    for(int i = 0; i < num; ++i)
    {
        printf("%c", buf[i]);
    }


}
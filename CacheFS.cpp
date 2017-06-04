#include <cstdio>
#include <sys/stat.h>
#include <string>
#include <map>
#include <vector>
#include <fcntl.h>
#include <cstring>

class Block;
#include "CacheFS.h"
#include "myFile.h"
#include "LRUAlg.h"
#include "LFUAlg.h"
#include "FBRAlg.h"
#include "Block.h"
#include <math.h>
#define ERR -1
#define SUCCESS 0
#define MANDATORY_LOC "/tmp"
using namespace std;


static int num_files = 0;
static size_t blksize;
static map<string, myFile> file_map;     //Map for full-path.
static map<int, string> open_files;     //open files map
static Algorithm *algo = nullptr;                 //Algorithm of choice

int CacheFS_init(int blocks_num, cache_algo_t cache_algo,
                 double f_old , double f_new  ){
    struct stat fi;
    stat("/tmp", &fi);
    blksize = (size_t)fi.st_blksize;
    switch(cache_algo){
        case LRU:
            *algo = LRUAlg(blocks_num);
            break;
        case FBR:
            *algo = FBRAlg(blocks_num, f_old, f_new);
            break;
        case LFU:
            *algo = LFUAlg(blocks_num);
            break;
        default:
            return -1;
    }
    return 1;

}



int CacheFS_destroy(){

}


int CacheFS_open(const char *pathname){
    int local_fd = num_files++;

    auto iter = file_map.find(string(pathname));
    string path_str(pathname);

    if(iter != file_map.end())
    {
        open_files[local_fd] = path_str;
        (*iter).second.inc_instance_count();
        return local_fd;
    }

    if(path_str.find(MANDATORY_LOC) != 0) return ERR;

    int fd = open(pathname, O_RDONLY|O_DIRECT|O_SYNC);

    if(fd == ERR) return ERR;

    myFile f(path_str, blksize, fd);

    file_map[path_str] = f;
    open_files[local_fd] = path_str;

    return local_fd;
}


int CacheFS_close(int file_id){

    if(open_files.find(file_id) == open_files.end()) return ERR;
    file_map[open_files[file_id]].dec_instance_count();
    //Note: close here? or keep the close in myFile?
}


/**
 *
 * @param size size of the file
 * @param n_bytes number of bytes to read
 * @param offset  offset of file
 * @return vector representing block numbers to fetch and bytes to fetch off of them
 *         if lastblock=firstblock we return beginning index then end index
 */
vector<pair<int,int>> blocksToFetch(size_t size, size_t n_bytes, off_t offset)
{
    size_t begin = min((size_t)offset, size);
    size_t end = min(offset + n_bytes, size);

    int first_block = (int)floor(begin / blksize);
    int num_bytes_first = (int)(blksize - (offset % blksize));
    int last_block = (int)floor((offset + n_bytes)/(blksize));
    int num_bytes_last = (int)(end%blksize);

    vector<pair<int,int>> res;


    if(first_block == last_block)                                   //  case first and last blocks are the same
    {
        res.push_back(pair<int,int>(num_bytes_first, num_bytes_last));
        return res;
    }

    // case theres a gap between first and last - meaning the first is till
    // the last bit of the block and last if from
    // the first bit of the block

    res.push_back(pair<int,int>(first_block, num_bytes_first));
    for(int i = first_block + 1; i < last_block; i++)
    {
        res.push_back(pair<int,int>(i, blksize));
    }
    res.push_back(pair<int,int>(last_block, num_bytes_last));

    return res;

}


int CacheFS_pread(int file_id, void *buf, size_t count, off_t offset)
{

    //TODO: REMINDER FOR GAL to recheck this funciton because it might contain bugs
    //Notice all the edge cases - same block, the different handling of the first block and the rest of the blocks
    //Maybe more?

    int bytes_read = 0;
    if(buf == nullptr || offset < 0) return ERR;

    auto iter = open_files.find(file_id);
    if(iter == open_files.end()) return ERR;

    myFile & f = file_map[(*iter).second];
    auto blocks_to_fetch = blocksToFetch(f.getSize(), count, offset);//returns a vector of pairs of ints, see doc'


    /**
     *
     * case everything is in the same block(not necessarily from the beginning.
     * NOTE THE SPECIAL RETURNING VALUE WHEN LAST=FIRST in blocksToFetch.
     *
     */
    if (blocks_to_fetch.size() == 1)
    {
        int block = (int)(min(offset, f.getSize())/blksize);

        auto data = algo->get_block(&f,block); //TODO: Prone to problems. remember block returned is a copy and has a pointer
        if(data.getId() == ERR) return ERR;   //error is a block with id -1(macro ERR)

        void *data_ptr = data.getData();
        data_ptr += blocks_to_fetch[0].first;
        memcpy(buf, data_ptr, blocks_to_fetch[0].second - blocks_to_fetch[0].first + 1);  //+1 necessary?

        return blocks_to_fetch[0].second-blocks_to_fetch[0].first + 1;
    }


    /**
     * Otherwise
     */

    auto data = algo->get_block(&f, blocks_to_fetch[0].first); //TODO: Prone to problems. remember block returned is a copy and has a pointer
    if(data.getId() == ERR) return ERR;   //error is a block with id -1(macro ERR)

    void *data_ptr = data.getData();
    data_ptr += (blksize-blocks_to_fetch[0].second);
    memcpy(buf, data_ptr, blocks_to_fetch[0].second);

    buf += blocks_to_fetch[0].second;
    bytes_read += blocks_to_fetch[0].second;

    blocks_to_fetch.erase(blocks_to_fetch.begin());


    for (auto &block:blocks_to_fetch)
    {
        data = algo->get_block(&f, block.first); //TODO: Prone to problems. remember block returned is a copy and has a pointer
        if(data.getId() == ERR) return ERR;   //error is a block with id -1(macro ERR)

        data_ptr = data.getData();

        memcpy(buf, data_ptr, block.second);

        buf += block.second;
        bytes_read += block.second;
    }

    return bytes_read;
}

int CacheFS_print_cache (const char *log_path)
{
    int num_of_block;
    string s;

    vector<Block*> b;

    int fd = open(log_path, O_APPEND|O_CREAT|O_WRONLY);
    if (fd == -1) return ERR;

    for(Block* blk : b)
    {

    }
    ssize_t ret = write(fd, s.c_str(), s.size());
    if(ret == -1) return ERR;


    return SUCCESS;
}


int CacheFS_print_stat (const char *log_path){
    unsigned long hits = 9999;
    unsigned long misses = 1234;
    string s("Hits number: %d.\nMisses number: %d.\n", hits, misses);

    int fd = open(log_path, O_APPEND|O_CREAT|O_WRONLY);
    if (fd == -1) return ERR;

    ssize_t ret = write(fd, s.c_str(), s.size());
    if(ret == -1) return ERR;

    return SUCCESS;

}
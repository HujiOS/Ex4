#include <cstdio>
#include <linux/limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string>
#include <map>
#include <vector>
#include <fcntl.h>

#include <cstring>

#include <algorithm>
#include "CacheFS.h"
#include "myFile.h"
#include "LRUAlg.h"
#include "LFUAlg.h"
#include "FBRAlg.h"
class Block;

#include "Block.h"
#define ERR -1
#define SUCCESS 0
#define MANDATORY_LOC "/tmp"
using namespace std;


static int num_files = 0;
static size_t blksize;
static map<string, myFile*> file_map;     //Map for full-path.
static map<int, string> open_files;     //open files map
static Algorithm *algo = nullptr;                 //Algorithm of choice
static vector<myFile*> closedFiles;

int CacheFS_init(int blocks_num, cache_algo_t cache_algo,
                 double f_old , double f_new  ){
    struct stat fi;
    stat("/tmp", &fi);
    blksize = (size_t)fi.st_blksize;
    switch(cache_algo){
        case LRU:
            algo = new LRUAlg(blocks_num);
            break;
        case FBR:
            if(f_old < 0 || f_new <0 || f_new > 1 || f_old >1) return ERR;
            if((f_old+f_new > 1)) return ERR;

            algo = new FBRAlg(blocks_num, f_old, f_new);
            break;
        case LFU:
            algo = new LFUAlg(blocks_num);
            break;
        default:
            return ERR;
    }
    return SUCCESS;

}



int CacheFS_destroy(){
    open_files.clear();
    algo->destroy();
    for(auto file: closedFiles){
        delete file;
    }
    closedFiles.clear();
    delete algo;
    return SUCCESS;
}


int CacheFS_open(const char *pathname){
    int local_fd = num_files++;
    char path_tmp[PATH_MAX] ;
    realpath(pathname, path_tmp);

    if(path_tmp == nullptr) return ERR;

    string path_str(pathname);

    auto iter = file_map.find(path_str);


    if(iter != file_map.end())
    {
        open_files[local_fd] = path_str;
        (*iter).second->inc_instance_count();
        return local_fd;
    }

    if(path_str.find(MANDATORY_LOC) != 0) return ERR;

    int fd = open(path_str.c_str(), O_RDONLY|O_DIRECT|O_SYNC);

    if(fd == ERR) return ERR;


    myFile *f = new myFile(path_str, blksize, fd);

    file_map[path_str] = f;
    open_files[local_fd] = path_str;

    return local_fd;
}


int CacheFS_close(int file_id){

    if(open_files.find(file_id) == open_files.end()) return ERR;
    if(file_map[open_files[file_id]]->dec_instance_count()){
        myFile *tmpFile = file_map[open_files[file_id]];
        file_map.erase(open_files[file_id]);
        closedFiles.push_back(tmpFile);
    }
    open_files.erase(file_id);
    return SUCCESS;
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
    size_t end = min((size_t)offset + n_bytes, size);

    int first_block = (int)floor(begin / blksize);
    int num_bytes_first = (int)(blksize - (begin % blksize));
    int last_block = (int)floor(end / (blksize));
    int num_bytes_last = (int)(end % blksize);

    vector<pair<int,int>> res;


    if(first_block == last_block)                                   //  case first and last blocks are the same
    {
        res.push_back(pair<int,int>(blksize - num_bytes_first, num_bytes_last));
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
    char* c_buf = (char*)buf;
    int bytes_read = 0;
    if(c_buf == nullptr || offset < 0) return ERR;

    auto iter = open_files.find(file_id);
    if(iter == open_files.end()) return ERR;

    myFile *f = file_map[(*iter).second];
    auto blocks_to_fetch = blocksToFetch(f->getSize(), count, offset);//returns a vector of pairs
    // of ints, see doc'
    /**
     *
     * case everything is in the same block(not necessarily from the beginning.
     * NOTE THE SPECIAL RETURNING VALUE WHEN LAST=FIRST in blocksToFetch.
     *
     */
    if (blocks_to_fetch.size() == 1)
    {
        if(blocks_to_fetch[0].first == blocks_to_fetch[0].second) return 0;
        int block = (int)(min(offset, (off_t)f->getSize())/blksize);

        auto data = algo->get_block(f,block); //TODO: Prone to problems. remember block returned is a copy and has a pointer
        if(data == nullptr) return ERR;

        char *data_ptr = (char*)data->getData();
        data_ptr += blocks_to_fetch[0].first;
        memcpy(c_buf, data_ptr, blocks_to_fetch[0].second - blocks_to_fetch[0].first);  //TODO: +1 necessary?

        return blocks_to_fetch[0].second-blocks_to_fetch[0].first;  //TODO: +1 necessary?
    }


    /**
     * Otherwise
     */

    auto data = algo->get_block(f, blocks_to_fetch[0].first); //TODO: Prone to problems. remember block returned is a copy and has a pointer
    if(data == nullptr) return ERR;

    char *data_ptr = (char*)data->getData();
    data_ptr += (blksize-blocks_to_fetch[0].second);
    if(memcpy(c_buf, data_ptr, (size_t)blocks_to_fetch[0].second) == nullptr){
        return ERR;
    }

    c_buf += blocks_to_fetch[0].second;
    bytes_read += blocks_to_fetch[0].second;

    blocks_to_fetch.erase(blocks_to_fetch.begin());


    for (auto &block:blocks_to_fetch)
    {
        if(block.second == 0) continue;

        data = algo->get_block(f, block.first); //TODO: Prone to problems. remember block returned is a copy and has a pointer
        if(data->getId() == ERR) return ERR;   //error is a block with id -1(macro ERR)

        data_ptr = (char*)data->getData();

        if(memcpy(c_buf, data_ptr, (size_t)block.second) == nullptr){
            return ERR;
        }

        c_buf += (size_t)block.second;
        bytes_read += (size_t)block.second;
    }

    return bytes_read;
}

int CacheFS_print_cache (const char *log_path)
{
    string s;

    vector<Block*> b;

    int fd;
    if((fd = open(log_path, O_APPEND|O_CREAT|O_WRONLY)) < 0) return ERR;
    auto to_print = algo->printable();

    for(auto print_item : to_print)
    {
        s = print_item.first + string(" ") + to_string(print_item.second) + string("\n");
        ssize_t ret = write(fd, s.c_str(), s.size());
        if(ret == -1) return ERR;
    }

    return SUCCESS;
}


int CacheFS_print_stat (const char *log_path){
    int hits = algo->hits();
    int misses = algo->misses();

    string s=string("Hits number: ") + to_string(hits) +
            string("\nMisses number: ") + to_string(misses) + string("\n");//TODO:ADD DOTs

    int fd;
    if((fd = open(log_path, O_APPEND|O_CREAT|O_WRONLY)) < 0) return ERR;

    ssize_t ret = write(fd, s.c_str(), s.size());
    if(ret == -1) return ERR;

    return SUCCESS;

}
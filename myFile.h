//
// Created by bgalb on 6/1/17.
//

#ifndef EX3_FILE_H
#define EX3_FILE_H
#include <cstdio>
#include "Block.h"
#include <vector>
#include <string>
#include <map>
#include <sys/stat.h>

using namespace std;
class myFile {
private:
    string _fullPath;
    map<int, Block&> _blocks;    //map of block number to
    size_t _n_bytes;             //size of file in bytes
    size_t _blockSize;           //system block size
    int _fd;                     //system file desc
    int num_instances;           // num instances open from our filesystem
    int& algo;


    /**
     * Close itself
     */
    void closeFile();
public:
    /**
     * Constructor
     * @param path path of the file
     * @param blocksize size of a block
     * @param fd file descriptor
     * @return
     */
    myFile(string path, size_t blocksize, int fd, int a):
            _fullPath(path), _blockSize(blocksize), _fd(fd), num_instances(1), algo(a){
        struct stat st;
        stat(path.c_str(), &st);
        this -> _n_bytes = (size_t)st.st_size;

    }

    /**
     *
     * @param block_num block number to fetch
     * @return a pair of a pointer to the block data or NULL if non-existent and flag to signify error
     */
    pair<void *, bool> fetchBlock(int block_num);
    /**
     *
     * @return overall size of the current file
     */
    size_t getSize();

    /**
     *
     * @return get full path of the file
     */
    string getFullPath();

    /**
     *
     * @param blocksize a block's size in the system
     * @return number of blocks
     */
    size_t numOfBlocks();

    /**
     *
     * @return file descriptor in system
     */
    int get_fd();

    /**
     *
     * @return block size
     */
    size_t getBlockSize();

    /**
     *  increases instance count
     */
    void inc_instance_count();

    /**
     *
     * @return true if the file still lives, false if it dies.
     */
    bool dec_instance_count();


};


#endif //EX3_FILE_H

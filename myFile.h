//
// Created by bgalb on 6/1/17.
//

#ifndef EX3_FILE_H
#define EX3_FILE_H
#include <cstdio>
#include <vector>
#include <string>
#include <map>
#include <sys/stat.h>
using namespace std;
class Block;
class myFile {
private:
    string _fullPath;
    map<int, Block*> _blocks;    //map of block number to
    size_t _n_bytes;             //size of file in bytes
    size_t _blockSize;           //system block size
    int _fd;                     //system file desc
    int num_instances;           // num instances open from our filesystem


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
    myFile(string path, size_t blocksize, int fd):
            _fullPath(path), _blockSize(blocksize), _fd(fd), num_instances(1){
        struct stat st;
        stat(path.c_str(), &st);
        this -> _n_bytes = (size_t)st.st_size;

    }

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

    /**
     * function who get an id of block to delete and remove it from the array.
     * @param id
     */

    void removeBlock(int id);


    void addBlock(Block *block);

};


#endif //EX3_FILE_H

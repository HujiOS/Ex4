#ifndef EX3_BLOCK_H
#define EX3_BLOCK_H

#include <string>
//#include <zconf.h>
#include <unistd.h>
#define INIT_NUM_REF 1
#include "myFile.h"
class Block {
private:
    int num_references;
    myFile _file ;
    std::string _fname;
    int _block_number;
    void *blk;
public:
    Block(myFile& file, int block_number):
            _file(file), _block_number(block_number), num_references(INIT_NUM_REF)
    {
        if(block_number == -1) return;
        _fname = file.getFullPath();
        size_t size = file.getBlockSize();
        if(blk < 0){
            throw bad_alloc();
        }
        off_t offsite = block_number * (off_t)size;
        pread(file.get_fd(), blk, size, offsite);
    }

    /**
     *
     * @return the data contained in the block
     */
    void *getData();

    /**
     *
     * @return returns the number of references this block has
     */
    int numReferences();


    /**
     *
     * @param num new number of
     */
    void setNumReferences(int num);

    /**
     * @return filename associated for this block
     */
    std::string getFname();

    /**
     * @return block internal Id
     */
    int getId();


    void deleteBlock(){
        _file.removeBlock(_block_number);
    }
    bool operator==(const Block& b){
        return this->_fname == b._fname && this->_block_number == b._block_number;
    };

};


#endif //EX3_BLOCK_H

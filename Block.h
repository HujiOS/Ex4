#ifndef EX3_BLOCK_H
#define EX3_BLOCK_H

#include <string>
#include <zconf.h>
#include "myFile.h"
#define INIT_NUM_REF 1


class Block {
private:
    int num_references;
    myFile& file const;
    string _fname;
    int _block_number;
    void *blk;
    bool _deleted = false;
public:
    Block(myFile& file, int block_number):
            file(file), _block_number(block_number), num_references(INIT_NUM_REF)
    {
        if(block_number == -1) return;
        _fname = file.getFullPath();
        size_t size = file.getBlockSize();
        blk = new void*(size);
        if(blk < 0){
            throw bad_alloc();
        }
        pread(file.get_fd(), blk, size, block_number * size);
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
    string getFname();

    /**
     * @return block internal Id
     */
    int getId();


    void deleteBlock(){
        file.removeBlock(_block_number);
    }

};


#endif //EX3_BLOCK_H

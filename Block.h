#ifndef EX3_BLOCK_H
#define EX3_BLOCK_H

#include <string>
#include <zconf.h>
#include "File.h"
#define INIT_NUM_REF 1


class Block {
private:
    int num_references;
    const File& file;
    int block_number;
    void *blk;
public:
    Block(File & file, int block_number):
            file(file), block_number(block_number), num_references(INIT_NUM_REF)
    {
        size_t size = file.getBlockSize();
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

};


#endif //EX3_BLOCK_H

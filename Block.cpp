//
// Created by bgalb on 5/31/17.
//

#include "Block.h"



/**
 *
 * @return the data contained in the block
 */
void* Block::getData()
{
    num_references++;
    return blk;
}

/**
 *
 * @return returns the number of references this block has
 */
int Block::numReferences()
{
    return num_references;
}


/**
 *
 * @param num new number of
 */
void Block::setNumReferences(int num)
{
    num_references = num;
}


string Block::getFname(){
    return _fname;
}

int Block::getId(){
    return _block_number;
}


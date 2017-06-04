//
// Created by bgalb on 6/1/17.
//

#include "myFile.h"
#include "Block.h"
/**
 *
 * @param block_num block number to fetch
 * @return a pair of a pointer to the block data or NULL if non-existent and flag to signify error
 */


void myFile::removeBlock(int blkId){
    auto elem = _blocks.find(blkId);
    _blocks.erase(elem);
}

void myFile::addBlock(Block &block) {
    _blocks.insert(make_pair(block.getId(), block));
}

/**
 *
 * @return overall size of the current file in bytes
 */
size_t myFile::getSize()
{
    return _n_bytes;
}

/**
 *
 * @return get full path of the file
 */
string myFile::getFullPath()
{
    return _fullPath;
}

/**
 *
 * @param blocksize a block's size in the system
 * @return number of blocks
 */
size_t myFile::numOfBlocks()
{
    return (_n_bytes / _blockSize);
}

/**
 *
 * @return file descriptor in system
 */
int myFile::get_fd()
{
    return _fd;
}


/**
 *
 * @return block size
 */
size_t myFile::getBlockSize()
{
    return _blockSize;
}



/**
 *  increases instance count
 */
void myFile::inc_instance_count()
{
    num_instances++;
}



/**
 * Close itself
 */
void myFile::closeFile()
{
    //TODO: close fd, free memory allocated, etc.
}

/**
 *
 * @return true if the file still lives, false if it dies.
 */
bool myFile::dec_instance_count()
{
    if(--num_instances <= 0)
    {
        closeFile();
        return false;
    }

    return true;

}
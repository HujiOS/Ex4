//
// Created by bgalb on 6/1/17.
//

#include "myFile.h"

/**
 *
 * @param block_num block number to fetch
 * @return a pair of a pointer to the block data or NULL if non-existent and flag to signify error
 */
pair<void *, bool> myFile::fetchBlock(int block_num)
{
    if(block_num >= numOfBlocks())
    {
        //ERROR, not possible
        return std::make_pair<void *, bool>(nullptr, true);
    }

    auto blk =  _blocks.find(block_num);  //Type is map iterator

    if(blk == _blocks.end())
    {
        algo.add_block(this, block_num);
        blk = _blocks.find(block_num);
    }

    return std::make_pair<void *, bool>((*blk).second.getData(), false);
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
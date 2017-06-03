//
// Created by bgalb on 6/1/17.
//

#ifndef EX3_ALGORITHM_H
#define EX3_ALGORITHM_H

#include "myFile.h"

class Algorithm
{
protected:
    int _blkNum;
    vector<Block> _blocks;
    int _hits;
    int _misses;
    bool verifyBlock(myFile *f, int id){
        return f->numOfBlocks() > id;
    }
public:
    Algorithm(int blkNum):_blkNum(blkNum){

    }

    int misses(){
        return _misses;
    }

    int hits(){
        return _hits;
    }

    virtual void removeBlock() = 0;
    virtual void evalBlock(Block &blk){
      blk.getData();
    };

    Block get_block(myFile* file, int id){
        if(!this->verifyBlock(file, id)) return Block(*file, -1);
        Block tmpBlock(*file,-1);
        bool hit = false;

        for(vector<Block>::iterator it = _blocks.begin(); it != _blocks.end() ; it++){
            if(it->getFname() == file->getFullPath() && it->getId() == id){
                // we got an hit.
                _hits++;
                tmpBlock = *it;
                _blocks.erase(it);
                hit = true;
                break;
            }
        }
        if(!hit){
            try{
                tmpBlock = Block(*file, id);
            }
            catch(bad_alloc& ba){
                return Block(*file, -1);
            }
            file->addBlock(tmpBlock);
            _misses++;
            if(_blocks.size() == _blkNum) this->removeBlock();
            // maintain the last recent used block in the beginning of the vector
        }

        auto it = _blocks.begin();
        _blocks.insert(it, tmpBlock);
        return tmpBlock;
    }
};

#endif //EX3_ALGORITHM_H

//
// Created by bgalb on 6/1/17.
//

#ifndef EX3_ALGORITHM_H
#define EX3_ALGORITHM_H

#include <string>
#include <math.h>
#include "myFile.h"
#include "Block.h"
#include <algorithm>
#include <functional>
#include <array>
#include <iostream>

class Algorithm
{
protected:
    unsigned int _blkNum;
    vector<Block*> _blocks;
    int _hits;
    int _misses;
    bool verifyBlock(myFile *f, unsigned int id){
        return f->numOfBlocks() > id;
    }
public:
    Algorithm(int blkNum):_blkNum((unsigned int)blkNum){
        _hits = 0;
        _misses = 0;
    };
    virtual ~Algorithm(){
        for(auto blk:_blocks){
            delete blk;
        }
        _blocks.clear();
    }

    int misses(){
        return _misses;
    }

    int hits(){
        return _hits;
    }

    virtual void removeBlock() = 0;
    virtual void evalBlock(Block *blk){
      blk->ref();
    };

    Block *get_block(myFile* file, int id){
        if(!this->verifyBlock(file, id)) return nullptr;
        Block *tmpBlock = nullptr;
        bool hit = false;

        for(vector<Block*>::iterator it = _blocks.begin(); it != _blocks.end() ; it++){
            if((*it)->getFname() == file->getFullPath() && (*it)->getId() == id){
                // we got an hit.
                _hits++;
                tmpBlock = *it;
                evalBlock(tmpBlock);
                _blocks.erase(it);
                hit = true;
                break;
            }
        }
        if(!hit){
            try{
                tmpBlock = new Block(file, id);
            }
            catch(bad_alloc& ba){
                return nullptr;
            }
            file->addBlock(tmpBlock);
            _misses++;
            if(_blocks.size() == _blkNum) this->removeBlock();
            // maintain the last recent used block in the beginning of the vector
        }
        auto itt = this->_blocks.begin();
        this->_blocks.insert(itt, tmpBlock);
        return *_blocks.begin();
    }

    void destroy(){
        for(auto blk : _blocks){
            delete blk;
        }
        _blocks.clear();
    }

    virtual vector<pair<string, int>> printable() = 0;
};

#endif //EX3_ALGORITHM_H

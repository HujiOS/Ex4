//
// Created by omer on 6/3/17.
//
#include "Algorithm.h"

#ifndef EX3_FBRALG_H
#define EX3_FBRALG_H
class FBRAlg : public Algorithm{
public:
    FBRAlg(int blknum, double f_old, double f_new):Algorithm(blknum),_old(f_old),_new(f_new){
    }
    // TODO pay attention to case that we are deleting block from last round.

private:
    double _old;
    double _new;
    void removeBlock(){
        size_t oldIdx = floor(_blkNum * _old);
        minRef = min(map(_blocks.begin() + oldIdx, _blocks.end(), [](Block a){return a.numReferences();}));
        for(vector<Block>::iterator it = _blocks.end() ; it != _blocks.begin() + oldIdx ; --it){
            if(it->numReferences() == minRef){
                _blocks.erase(it);
                break;
            }
        }
    }

    void evalBlock(Block &blk){
        ptrdiff_t idx = std::find(_blocks.begin(), _blocks.end(), blk) - _blocks.begin();
        if(_new < idx / _blkNum){
            blk.getData();
        }
    };
};

#endif //EX3_FBRALG_H

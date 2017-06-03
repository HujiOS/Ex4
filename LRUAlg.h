//
// Created by omer on 6/3/17.
//
#include "Algorithm.h";

#ifndef EX3_LRUALG_H
#define EX3_LRUALG_H

class LRUAlg : Algorithm{
public:
    LRUAlg(int blknum){
        super(blknum);
    }
    // TODO pay attention to case that we are deleting block from last round.

private:
    void removeBlock(){
        Block blk = _blocks.pop_back();
        blk.deleteBlock();
    }

};

#endif //EX3_LRUALG_H

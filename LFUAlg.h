//
// Created by omer on 6/3/17.
//
#include "Algorithm.h"

#ifndef EX3_LFU_H
#define EX3_LFU_H

class LFUAlg : public Algorithm{
public:
    LFUAlg(int blknum):Algorithm(blknum){
    }
    // TODO pay attention to case that we are deleting block from last round.

private:
    void removeBlock(){
        std::stable_sort(_blocks.begin(), _blocks.end(), [](Block *a, Block *b){ return a->numReferences()
                                                                               > b->numReferences();});
        Block *blk = _blocks.back();
        _blocks.pop_back();
        delete blk;
    }

    vector<pair<string, int>> printable(){
        vector<pair<string, int>> vec;
        std::stable_sort(_blocks.begin(), _blocks.end(), [](Block *a, Block *b){ return a->numReferences()
                                                                                 > b->numReferences();});
        for(auto blk : _blocks){
            vec.push_back(make_pair(blk->getFname(), blk->getId()));
        }
        return vec;
    };
};

#endif //EX3_LFU_H

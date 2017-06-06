//
// Created by omer on 6/3/17.
//
#include "Algorithm.h"
#include <math.h>
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
        vector<int> refs;
        for(auto it = _blocks.begin() + oldIdx; it != _blocks.end(); ++it){
            refs.push_back((*it)->numReferences());
        }
        auto minRef = min_element(begin(refs), end(refs));
        reverse(begin(refs),end(refs));
        size_t dist = distance(begin(refs), minRef) + oldIdx;
        auto blockIt = _blocks.begin() + dist;
        Block*tmpBlock = *blockIt;
        _blocks.erase(blockIt);
        delete tmpBlock;
    }

    void evalBlock(Block *blk){
        ptrdiff_t idx = find_if(_blocks.begin(), _blocks.end(),
                                [&blk](const Block* b)
                                {
                                    return *blk == *b;
                                }
        ) - _blocks.begin();
        if(_new < idx / _blkNum){
            blk->getData();
        }
    };

    vector<pair<string, int>> printable(){
        vector<pair<string, int>> vec;
        for(auto blk : _blocks){
            vec.push_back(make_pair(blk->getFname(), blk->getId()));
        }
        return vec;
    };
};

#endif //EX3_FBRALG_H

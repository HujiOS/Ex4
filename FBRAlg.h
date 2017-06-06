//
// Created by omer on 6/3/17.
//
#include "Algorithm.h"
#include <math.h>
#include <assert.h>

#ifndef EX3_FBRALG_H
#define EX3_FBRALG_H
class FBRAlg : public Algorithm{
public:
    FBRAlg(int blknum, double f_old, double f_new):Algorithm(blknum),_old(f_old),_new(f_new){}
    // TODO pay attention to case that we are deleting block from last round.

private:
    double _old;
    double _new;
    void removeBlock(){
        size_t startOfOld = (size_t)ceil(_blkNum * (1- _old));
        vector<int> refs;
        for(auto it = _blocks.begin() + startOfOld; it != _blocks.end(); ++it){
            refs.push_back((*it)->numReferences());
        }

        int minRef = *min_element(begin(refs), end(refs));

        for(auto itt = _blocks.end() - 1; itt != _blocks.begin() + startOfOld; --itt){
            if( (*itt)->numReferences() == minRef ){
                Block*tmpBlock = *itt;
                _blocks.erase(itt);
                delete tmpBlock;
                break;
            }
        }
    }

    void evalBlock(Block *blk){
        ptrdiff_t idx = find_if(_blocks.begin(), _blocks.end(),
                                [&blk](const Block* b)
                                {
                                    return *blk == *b;
                                }
        ) - _blocks.begin();
        if(_new <= (double)idx / (double)_blkNum){
            blk->ref();
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

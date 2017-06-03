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

public:
    Algorithm(int blkNum):_blkNum(blkNum){

    }
    virtual void add_block(myFile* f, int num) = 0;
};

#endif //EX3_ALGORITHM_H

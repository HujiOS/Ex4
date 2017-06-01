//
// Created by bgalb on 6/1/17.
//

#ifndef EX3_ALGORITHM_H
#define EX3_ALGORITHM_H

#include "File.h"

class Algorithm
{
private:


public:
    virtual void add_block(File* f, int num) = 0;
};

#endif //EX3_ALGORITHM_H

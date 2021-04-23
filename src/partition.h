//
// Created by Максим Черкасов on 16.04.2021.
//

#ifndef HG_NEW_PARTITION_H
#define HG_NEW_PARTITION_H

#include "hg.h"

using namespace std;

class Partition {
public:
    vector <bool> partitionVec; // determine partition: 0 - left, 1 - right
    uint32_t vertNum = 0;
    const HyperGraph* hg;
    int solutionCost = 0;
    int balance = 0; // показывает разницу между количеством правых и левых вершин
    int tolerance = 0;

    Partition () = delete;
    Partition (const HyperGraph& hg, bool is_random, int tolerance_percentage);
    ~Partition() = default;

    int getCost () const { return solutionCost; }

    void update (uint32_t vertId); // changes the side of vertice

    void print ();
    void print (string& fileName);

    void calculateSolutionCost();
    void staticInit();
    void randomInit();
};


#endif //HG_NEW_PARTITION_H

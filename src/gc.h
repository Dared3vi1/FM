//
// Created by Максим Черкасов on 17.04.2021.
//

#ifndef HG_NEW_GC_H
#define HG_NEW_GC_H
#include "partition.h"



using namespace std;

class GainContainer {
public:
    bool modified {false};

    Partition* par;

    map <int, list <int>> leftContainer;
    map <int, list <int>> rightContainer;

    vector <int> gainVec;
    vector <int> diffVec;

    vector <std::list<int>::iterator> iterators;

    vector <bool> is_deleted;

    GainContainer() = delete;
    GainContainer (Partition& partition, bool modified_);

    bool is_empty() const { return leftContainer.empty() || rightContainer.empty(); }

    pair <uint32_t, int> bestFeasibleMove (int balance, int tolerance);

    void update (uint32_t vertId, bool side, int delta);
    void erase (uint32_t vertId, bool side);

    void print() const;
};


#endif //HG_NEW_GC_H

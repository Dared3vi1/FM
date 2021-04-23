//
// Created by Максим Черкасов on 16.04.2021.
//

#ifndef HG_NEW_HG_H
#define HG_NEW_HG_H

#include "libs.h"

using namespace std;

class HyperGraph {
public:
    HyperGraph() = delete;
    ~HyperGraph() = default;

    HyperGraph(const string& fileName, ostream& logger_);

    vector <vector <int>> verts;
    vector <vector <int>> edges;
    uint32_t vertNum = 0;
    uint32_t edge_num = 0;

    ostream& log;


};


#endif //HG_NEW_HG_H

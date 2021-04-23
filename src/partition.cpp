//
// Created by Максим Черкасов on 16.04.2021.
//

#include "partition.h"

Partition::Partition (const HyperGraph& hg_, bool is_random, int tolerance_):
    tolerance{tolerance_}
{
    partitionVec = vector <bool>(hg_.vertNum);
    vertNum = hg_.vertNum;
    if (is_random)
        randomInit();
    else
        staticInit();
    hg = &hg_;
    hg->log << "Initial partition done: " << vertNum / 2 << " verteces left and " << vertNum - vertNum / 2 << " vertices right" << std::endl;
    calculateSolutionCost();
    balance = 0;
}

void Partition::staticInit () {
    for (uint32_t i = 0; i < vertNum / 2; i++)
        partitionVec[i] = false;
    for (uint32_t i = vertNum / 2; i < vertNum; i++)
        partitionVec[i] = true;
}

void Partition::randomInit () {
    srand((unsigned) time(0)); // каждый раз новое рандомное разбиение
    for (int i = 0; i < partitionVec.size(); i++) {
        if (i < partitionVec.size() / 2)
            partitionVec[i] = true;
        else
            partitionVec[i] = false;
    }
}

void Partition::update (uint32_t vertId) { // меняем сторону вершины
    if (partitionVec[vertId])
        balance--;
        // баланс меньше 0 если слева больше
    else
        balance++;
        // баланс больше 0 если справа больше
    partitionVec[vertId] = !partitionVec[vertId];
}

void Partition::print () {
    int count_left = 0;
    for (int i = 0; i < vertNum; i++) {
        if (!partitionVec[i])
            count_left++;
    }
    hg->log << "Left: " << count_left << endl;
    int count_right = 0;
    for (int i = 0; i < vertNum; i++) {
        if (partitionVec[i])
            count_right++;
    }
    hg->log << "Right: " << count_right << endl;
    hg->log << "Solution cost: " << solutionCost << endl;
    hg->log << "Balance: " << balance << endl;
}

void Partition::print (string& fileName) {
    ofstream out_file;
    out_file.open(fileName);
    for (int i = 0; i < vertNum; i++) {
        out_file << partitionVec[i] << "\n";
    }
    out_file.close();
}

void Partition::calculateSolutionCost() { // Считаем количество пересекающих сечение гиперребер
    for (uint32_t i = 0; i < hg->edges.size(); i++) {
        bool current_partition = partitionVec[hg->edges[i][0]];
        for (uint32_t j = 0; j < hg->edges[i].size(); j++) {
            if (current_partition != partitionVec[hg->edges[i][j]]){
                solutionCost++;
                break;
            }
        }
    }
}

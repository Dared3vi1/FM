//
// Created by Максим Черкасов on 18.04.2021.
//

#include "fm.h"

void applyMove (GainContainer& gc, Partition& partition, const pair <uint32_t, int>& bestMove) {
    for (uint32_t i = 0; i < partition.hg->verts[bestMove.first].size(); i++) { // смотрим гиперребра вершины
        uint32_t edgeId = partition.hg->verts[bestMove.first][i]; // все гиперребра связанные с вершиной
        bool isNoVertInDest = true;
        bool isOneVertInSrc = true;
        int counterInDest = 0;
        uint32_t vertIdDest = 0;
        int counterInSrc = 0;
        uint32_t vertIdSrc = 0;
        for (uint32_t j = 0; j < partition.hg->edges[edgeId].size(); j++) { // смотрим все вершины каждого гиперребра
            uint32_t vertId = partition.hg->edges[edgeId][j]; // каждая вершина связанного гиперребра
            if (partition.partitionVec[bestMove.first]) {
                if (!partition.partitionVec[vertId]) { // считаем связанные вершины слева и справа для гиперребра
                    isNoVertInDest = false;
                    counterInDest++;
                    vertIdDest = vertId;
                } else if (vertId != bestMove.first) {
                    counterInSrc++;
                    vertIdSrc = vertId;
                }
            } else {
                if (partition.partitionVec[vertId]) {
                    isNoVertInDest = false;
                    counterInDest++;
                    vertIdDest = vertId;
                } else if (vertId != bestMove.first) {
                    counterInSrc++;
                    vertIdSrc = vertId;
                }
            }
            if (partition.partitionVec[bestMove.first] == partition.partitionVec[vertId] && vertId != bestMove.first)
                isOneVertInSrc = false;
        }
        if (isNoVertInDest) { // если дестинейшн пустая
            for (uint32_t j = 0; j < partition.hg->edges[edgeId].size(); j++) {  // берем все вершины в гиперребре
                uint32_t vertId = partition.hg->edges[edgeId][j];
                gc.diffVec[vertId]++;
            }
        }
        if (isOneVertInSrc) {
            for (uint32_t j = 0; j < partition.hg->edges[edgeId].size(); j++) { // берем все вершины в гиперребре
                uint32_t vertId = partition.hg->edges[edgeId][j];
                gc.diffVec[vertId]--;
            }
        }
        if (counterInSrc == 1)
            gc.diffVec[vertIdSrc]++;
        if (counterInDest == 1)
            gc.diffVec[vertIdDest]--;
    }
    gc.erase(bestMove.first, partition.partitionVec[bestMove.first]); // удаляем вершину
    gc.is_deleted[bestMove.first] = true;
    for (uint32_t i = 0; i < partition.hg->verts[bestMove.first].size(); i++) { // берем и смотрим все гиперребра
        uint32_t edgeId = partition.hg->verts[bestMove.first][i];
        for (uint32_t j = 0; j < partition.hg->edges[edgeId].size(); j++) { // смотрим все вершины в гиперребрах
            uint32_t vertId = partition.hg->edges[edgeId][j];
            if (gc.diffVec[vertId]) {
                gc.update(vertId, partition.partitionVec[vertId], gc.diffVec[vertId]);
                gc.diffVec[vertId] = 0;
            }
        }
    }
    partition.update(bestMove.first);

}

int FMpass (GainContainer& gc, Partition& partition) {
    int solutionCost = partition.getCost();
    int bestCost = solutionCost;
    list <uint32_t> vert_to_change;
    int steps {0};
    bool balanced {true};
    if (partition.tolerance == 0)
        balanced = false;
    while (!gc.is_empty()) {
        auto bestMove = gc.bestFeasibleMove(partition.balance, partition.tolerance);
        vert_to_change.push_back(bestMove.first);
        solutionCost -= bestMove.second;
        if (solutionCost < bestCost && (steps % 2 || balanced)) {
            bestCost = solutionCost;
            vert_to_change.erase(vert_to_change.begin(), vert_to_change.end());
        }
        applyMove(gc, partition, bestMove);
        steps++;
    }
    for (auto item : vert_to_change)
        partition.update(item);
    return bestCost;
}

uint32_t FM (Partition& Partition, bool modified) {
    bool isImproved = true;
    uint32_t epoch = 0;
    while (isImproved) {
        Partition.hg->log << "Epoch #" << epoch++ << endl;
        Partition.print();
        GainContainer gc(Partition, modified);
        int best = FMpass(gc, Partition);
        if (best == Partition.solutionCost) {
            isImproved = false;
            Partition.hg->log << endl;
            Partition.hg->log << "The last #epoch with the same cost" << endl;
            Partition.print();
            Partition.hg->log << "---------------------------------------";
        }
        else
            Partition.solutionCost = best;
        Partition.hg->log << endl;
    }
    return epoch;
}
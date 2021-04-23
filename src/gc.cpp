//
// Created by Максим Черкасов on 17.04.2021.
//

#include "gc.h"

GainContainer::GainContainer (Partition& partition, bool modified_):
    par{&partition}, modified{modified_}
{
    gainVec = vector <int> (partition.vertNum);
    is_deleted = vector <bool> (partition.vertNum, false);
    diffVec = vector<int> (partition.vertNum);
    iterators = vector <list<int>::iterator> (partition.vertNum);

    for (uint32_t e = 0; e < partition.hg->edge_num; e++) { // ALL HE
        int vertCounter_leftContainer = 0;
        int vertCounter_rightContainer = 0;
        int vertId_leftContainer = 0;
        int vertId_rightContainer = 0;

        for (uint32_t v = 0; v < partition.hg->edges[e].size(); v++) { // ALL VERTICES IN PARTICULAR HE
            int vertNum = partition.hg->edges[e][v];
            if (!partition.partitionVec[vertNum]) { // LS
                vertCounter_leftContainer++;
                vertId_leftContainer = vertNum;
            } else {                                // RS
                vertCounter_rightContainer++;
                vertId_rightContainer = vertNum;
            }
        }
        // Compute E(x)
        if ((vertCounter_rightContainer == 0 && vertCounter_leftContainer != 1) || (vertCounter_leftContainer == 0 && vertCounter_rightContainer != 1)) {
            for (uint32_t i = 0; i < partition.hg->edges[e].size(); i++) {
                int vertNum = partition.hg->edges[e][i];
                gainVec[vertNum]--;
            }
        }
        // Compute G(x)
        if (vertCounter_leftContainer == 1 && vertCounter_rightContainer != 0)
            gainVec[vertId_leftContainer]++;
        if (vertCounter_rightContainer == 1 && vertCounter_leftContainer != 0)
            gainVec[vertId_rightContainer]++;
    }

    for (uint32_t v = 0; v < partition.vertNum; v++) { // CREATE CONTAINERS
        if (!partition.partitionVec[v]) {
            auto& leftContainer_item = leftContainer[gainVec[v]];
            if (!modified_) {
                leftContainer_item.push_front(v);
                iterators[v] = leftContainer_item.begin();
            }
            else {
                leftContainer_item.push_back(v);
                iterators[v] = -- leftContainer_item.end();
            }
        } else {
            auto& rightContainer_item = rightContainer[gainVec[v]];
            if (!modified_) {
                rightContainer_item.push_front(v);
                iterators[v] = rightContainer_item.begin();
            }
            else {
                rightContainer_item.push_back(v);
                iterators[v] = -- rightContainer_item.end();
            }
        }
    }
}


pair <uint32_t, int> GainContainer::bestFeasibleMove (int balance, int tolerance) {

    auto leftContainerMaxGain = -- leftContainer.end();
    auto rightContainerMaxGain = -- rightContainer.end();

    int leftMaxGain = leftContainerMaxGain->first;
    int rightMaxGain = rightContainerMaxGain->first;
    bool is_right = false;

    if ((leftMaxGain < rightMaxGain && -balance < tolerance) || balance >= tolerance)
        is_right = true;
    if (is_right) { // Right side
        uint32_t vert {0};
        vert = (rightContainerMaxGain->second).front();
        (rightContainerMaxGain->second).pop_front();
        is_deleted[vert] = true;
        if ((rightContainerMaxGain->second).empty())
            rightContainer.erase(rightMaxGain);
        return {vert, rightMaxGain};
    }
    uint32_t vert{0};
    vert = (leftContainerMaxGain->second).front();
    (leftContainerMaxGain->second).pop_front();
    is_deleted[vert] = true;
    if ((leftContainerMaxGain->second).empty())
        leftContainer.erase(leftMaxGain);
    return {vert, leftMaxGain};
}

void GainContainer::update (uint32_t vertNum, bool side, int delta) {
    if (is_deleted[vertNum])
        return;
    erase(vertNum, side);
    gainVec[vertNum] += delta;
    if (side) {
        auto &rightContainerItem = rightContainer[gainVec[vertNum]];
        if (!modified) {
            rightContainerItem.push_front(vertNum);
            iterators[vertNum] = rightContainerItem.begin();
        } else {
            rightContainerItem.push_back(vertNum);
            iterators[vertNum] = -- rightContainerItem.end();
        }
    } else {
        auto& leftContainerItem = leftContainer[gainVec[vertNum]];
        if (!modified) {
            leftContainerItem.push_front(vertNum);
            iterators[vertNum] = leftContainerItem.begin();
        } else {
            leftContainerItem.push_back(vertNum);
            iterators[vertNum] = -- leftContainerItem.end();
        }
    }
}

void GainContainer::erase (uint32_t vertNum, bool side) {
    if (is_deleted[vertNum])
        return;
    if (side) {
        rightContainer[gainVec[vertNum]].erase(iterators[vertNum]);
        if (rightContainer[gainVec[vertNum]].empty())
            rightContainer.erase(gainVec[vertNum]);
    } else {
        leftContainer[gainVec[vertNum]].erase(iterators[vertNum]);
        if (leftContainer[gainVec[vertNum]].empty())
            leftContainer.erase(gainVec[vertNum]);
    }
}

void GainContainer::print () const {
    cout << "leftContainer" << endl;
    for (const auto& item : leftContainer) {
        cout << item.first << ": ";
        for (const auto& vert : item.second) {
            cout << vert << " ";
        }
        cout << endl;
    }
    cout << "rightContainer" << endl;
    for (const auto& item : rightContainer) {
        cout << item.first << ": ";
        for (const auto& vert : item.second) {
            cout << vert << " ";
        }
        cout << endl;
    }
}
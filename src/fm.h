//
// Created by Максим Черкасов on 18.04.2021.
//

#ifndef HG_NEW_FM_H
#define HG_NEW_FM_H
#include "gc.h"


void applyMove (GainContainer& gc, Partition& partition, const pair <uint32_t, int>& bestMove);
int FMpass (GainContainer& gc, Partition& partition);
uint32_t FM (Partition& Partition, bool modified);


#endif //HG_NEW_FM_H

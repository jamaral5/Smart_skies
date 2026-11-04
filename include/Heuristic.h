#ifndef HEURISTIC_H
#define HEURISTIC_H

#include "FlightGraph.h"

class Heuristic {
public:
    static double calculate(int sourceInternalId, int destInternalId, const FlightGraph& graph);
};

#endif

#pragma once

#include <vector>
#include "FlightGraph.h"

//stores the result of an a* shortest-path search
struct AStarResult {
    bool found = false;
    double totalDistance = 0.0;
    int airportsExplored = 0;
    std::vector<int> path;
};

//finds the shortest route using a* search
AStarResult aStar(const FlightGraph& graph, int startAirport, int destinationAirport);

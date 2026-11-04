
#pragma once

#include <vector>
#include "FlightGraph.h"

//stores the result of a shortest-path search
struct DijkstraResult {
    bool found = false;
    double totalDistance = 0.0;
    int airportsExplored = 0;
    std::vector<int> path;
};

//finds the shortest route between two airports
DijkstraResult dijkstra(const FlightGraph& graph,
                        int startAirport,
                        int destinationAirport);

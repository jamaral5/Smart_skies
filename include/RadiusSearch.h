#pragma once
#include <vector>
#include "FlightGraph.h"

//gives out airport found outside the radius
struct RadiusResult {
    int airportId;
    double distanceKm;
};
std::vector<RadiusResult> radiusSearch(const FlightGraph& graph, int centerAirport, double radiusKm);
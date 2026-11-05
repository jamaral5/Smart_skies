#include "RadiusSearch.h"
#include "Heuristic.h"
#include <algorithm>
#include <stdexcept>

std::vector<RadiusResult> radiusSearch(const FlightGraph& graph, int centerAirport, double radiusKm) {
    int airportCount = graph.getAirportCount();

    //checks a center airport exists
    if (centerAirport < 0 || centerAirport >= airportCount) {
        throw std::out_of_range("Invalid airport ID");
    }

    //cant have a negative radius of course
    if (radiusKm < 0.0) {
        throw std::invalid_argument("Radius must be non-negative");
    }

    std::vector<RadiusResult> results;

    for (int i = 0; i < airportCount; i++) {
        if (i == centerAirport) {
            continue;
        }

        double distance = Heuristic::calculate(centerAirport, i, graph);

        if (distance <= radiusKm) {
            results.push_back({i, distance});
        }
    }

    //sorts the airports closest first
    std::sort(results.begin(), results.end(),
              [](const RadiusResult& a, const RadiusResult& b) {
                  return a.distanceKm < b.distanceKm;
              });

    return results;
}
#include "Heuristic.h"
#include <cmath>
#include <stdexcept>

constexpr double PI = 3.14159265358979323846;
constexpr double EARTH_RADIUS_KM = 6371.0;

double Heuristic::calculate(int sourceInternalId, int destInternalId, const FlightGraph& graph) {
    const auto& airports = graph.getAirports();
    
    if (sourceInternalId < 0 || sourceInternalId >= airports.size() || 
        destInternalId < 0 || destInternalId >= airports.size()) {
        throw std::invalid_argument("Airport internal ID out of bounds.");
    }
    
    double lat1 = airports[sourceInternalId].latitude * (PI / 180.0);
    double lon1 = airports[sourceInternalId].longitude * (PI / 180.0);
    double lat2 = airports[destInternalId].latitude * (PI / 180.0);
    double lon2 = airports[destInternalId].longitude * (PI / 180.0);

    double dLat = lat2 - lat1;
    double dLon = lon2 - lon1;

    double a = std::sin(dLat / 2) * std::sin(dLat / 2) +
               std::cos(lat1) * std::cos(lat2) *
               std::sin(dLon / 2) * std::sin(dLon / 2);
    
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    return EARTH_RADIUS_KM * c;
}
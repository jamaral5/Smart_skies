#include "FlightGraph.h"
#include <stdexcept>

int FlightGraph::getOrAddAirport(const std::string& iata, double lat, double lon, const std::string& name) {
    auto it = iataToInternalId.find(iata);
    if (it != iataToInternalId.end()) {
        int internalId = it->second;
        if (airports[internalId].latitude == 0.0 && lat != 0.0) {
            airports[internalId].latitude = lat;
            airports[internalId].longitude = lon;
        }
        return internalId;
    }

    int internalId = airports.size();
    airports.push_back({internalId, iata, lat, lon, name});
    iataToInternalId[iata] = internalId;
    
    adjList.push_back(std::vector<FlightEdge>());
    return internalId;
}

void FlightGraph::addFlight(int srcInternalId, int destInternalId, double distance, const std::string& airline) {
    if (distance < 0.0) {
        throw std::invalid_argument("Edge distance must be non-negative.");
    }
    if (srcInternalId < 0 || srcInternalId >= adjList.size()) {
        return;
    }

    long long routeKey = (static_cast<long long>(srcInternalId) << 32) | destInternalId;

    if (existingRoutes.count(routeKey)) {
        return;
    }
    existingRoutes.insert(routeKey);
    adjList[srcInternalId].push_back({destInternalId, distance, airline});
}

int FlightGraph::getAirportCount() const {
    return airports.size();
}

const std::vector<FlightEdge>& FlightGraph::getNeighbors(int internalId) const {
    static const std::vector<FlightEdge> empty;
    if (internalId >= 0 && internalId < adjList.size()) {
        return adjList[internalId];
    }
    return empty;
}

const std::vector<Airport>& FlightGraph::getAirports() const {
    return airports;
}

int FlightGraph::getAirportId(const std::string& iata) const {
    auto it = iataToInternalId.find(iata);
    if (it == iataToInternalId.end()) {
        return -1;
    }
    return it->second;
}

int FlightGraph::getRouteCount() const {
    return existingRoutes.size();
}
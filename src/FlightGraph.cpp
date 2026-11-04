#include "FlightGraph.h"
#include <stdexcept>

int FlightGraph::getOrAddAirport(int realId, const std::string& iata, double lat, double lon) {
    auto it = realIdToInternalId.find(realId);
    if (it != realIdToInternalId.end()) {
        int internalId = it->second;
        // Update coordinates if this destination was previously added without them
        if (airports[internalId].latitude == 0.0 && lat != 0.0) {
            airports[internalId].latitude = lat;
            airports[internalId].longitude = lon;
        }
        return internalId;
    }

    int internalId = airports.size();
    airports.push_back({internalId, realId, iata, lat, lon});
    realIdToInternalId[realId] = internalId;
    
    // Ensure the adjacency list expands in sync with the airport vector
    adjList.push_back(std::vector<FlightEdge>());
    
    return internalId;
}

void FlightGraph::addFlight(int srcInternalId, int destInternalId, double distance, const std::string& airline) {
    if (distance < 0.0) {
        throw std::invalid_argument("Edge distance must be a non-negative value.");
    }
    
    if (srcInternalId >= 0 && srcInternalId < adjList.size()) {
        adjList[srcInternalId].push_back({destInternalId, distance, airline});
    }
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
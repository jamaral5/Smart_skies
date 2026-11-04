#ifndef FLIGHTGRAPH_H
#define FLIGHTGRAPH_H

#include <string>
#include <vector>
#include <unordered_map>

struct Airport {
    int internalId; // Consecutive index from 0 to N-1
    int realId;     // Original dataset ID
    std::string iata;
    double latitude;
    double longitude;
};

struct FlightEdge {
    int destInternalId;
    double distanceKm;
    std::string airlineCode;
};

class FlightGraph {
private:
    std::vector<Airport> airports; // Indexed by internalId
    std::vector<std::vector<FlightEdge>> adjList; // Indexed by internalId
    std::unordered_map<int, int> realIdToInternalId; // Translates real CSV IDs to internal IDs

public:
    // Returns the consecutive internal ID, updating coordinates if it already exists as a dummy
    int getOrAddAirport(int realId, const std::string& iata, double lat, double lon);
    
    // Connects two internal IDs
    void addFlight(int srcInternalId, int destInternalId, double distance, const std::string& airline);
    
    int getAirportCount() const;

    // Getters for Dijkstra and A* array/vector traversals
    const std::vector<FlightEdge>& getNeighbors(int internalId) const;
    const std::vector<Airport>& getAirports() const;
};

#endif // FLIGHTGRAPH_H
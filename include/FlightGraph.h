#ifndef FLIGHTGRAPH_H
#define FLIGHTGRAPH_H

#include <string>
#include <vector>
#include <unordered_map>

struct Airport {
    int internalId; 
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
    std::vector<Airport> airports; 
    std::vector<std::vector<FlightEdge>> adjList; 
    std::unordered_map<std::string, int> iataToInternalId;

public:
    int getOrAddAirport(const std::string& iata, double lat, double lon);
    void addFlight(int srcInternalId, int destInternalId, double distance, const std::string& airline);
    
    int getAirportCount() const;
    const std::vector<FlightEdge>& getNeighbors(int internalId) const;
    const std::vector<Airport>& getAirports() const;
};

#endif
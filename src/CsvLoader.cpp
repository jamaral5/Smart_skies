#include "CsvLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

void CsvLoader::loadFlightData(const std::string& filepath, FlightGraph& graph) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filepath << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string routeIdStr, srcIdStr, srcIata, destIdStr, destIata, distStr, latStr, lonStr, airline;

        std::getline(ss, routeIdStr, ',');
        std::getline(ss, srcIdStr, ',');
        std::getline(ss, srcIata, ',');
        std::getline(ss, destIdStr, ',');
        std::getline(ss, destIata, ',');
        std::getline(ss, distStr, ',');
        std::getline(ss, latStr, ',');
        std::getline(ss, lonStr, ',');
        std::getline(ss, airline, ',');

        try {
            int srcRealId = std::stoi(srcIdStr);
            int destRealId = std::stoi(destIdStr);
            double dist = std::stod(distStr);
            double lat = std::stod(latStr); 
            double lon = std::stod(lonStr);

            if (dist < 0.0) continue; // Enforce non-negative requirements seamlessly

            // The graph handles the translation and returns consecutive internal index values
            int srcInternalId = graph.getOrAddAirport(srcRealId, srcIata, lat, lon);
            int destInternalId = graph.getOrAddAirport(destRealId, destIata, 0.0, 0.0); 
            
            graph.addFlight(srcInternalId, destInternalId, dist, airline);
            
        } catch (const std::exception& e) {
            continue; 
        }
    }
}
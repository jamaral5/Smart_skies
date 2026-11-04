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
    std::getline(file, line); // Skip the header row

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
            int srcId = std::stoi(srcIdStr);
            int destId = std::stoi(destIdStr);
            double dist = std::stod(distStr);
            double lat = std::stod(latStr); 
            double lon = std::stod(lonStr);

            // Add source airport
            graph.addAirport(srcId, srcIata, lat, lon);
            // Temporarily add destination airport with 0.0 coords if it hasn't been added yet
            graph.addAirport(destId, destIata, 0.0, 0.0); 
            
            // Add directed edge
            graph.addFlight(srcId, destId, dist, airline);
            
        } catch (const std::exception& e) {
            continue; // Skip malformed rows seamlessly without crashing
        }
    }
}
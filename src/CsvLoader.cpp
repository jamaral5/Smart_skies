#include "CsvLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

std::vector<std::string> parseCsvRow(const std::string& line) {
    std::vector<std::string> cols;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, ',')) {
        cols.push_back(token);
    }
    return cols;
}

void CsvLoader::loadAirports(const std::string& filepath, FlightGraph& graph) {
    std::ifstream file(filepath);
    if (!file.is_open()) return;

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        auto cols = parseCsvRow(line);
        if (cols.size() < 7) continue;

        try {
            std::string iata = cols[0];
            double lat = std::stod(cols[5]);
            double lon = std::stod(cols[6]);
            graph.getOrAddAirport(iata, lat, lon);
        } catch (...) { continue; }
    }
}

void CsvLoader::loadFlights(const std::string& filepath, FlightGraph& graph, int maxRows) {
    std::ifstream file(filepath);
    if (!file.is_open()) return;

    std::string line;
    std::getline(file, line);

    int rowCount = 0;
    while (std::getline(file, line)) {
        if (maxRows > 0 && rowCount >= maxRows) break;
        
        auto cols = parseCsvRow(line);
        if (cols.size() < 18) continue;

        try {
            std::string airline = cols[4];
            std::string srcIata = cols[7];
            std::string destIata = cols[8];
            
            double distanceKm = std::stod(cols[17]) * 1.60934; 

            if (distanceKm < 0.0) continue;

            int srcInternalId = graph.getOrAddAirport(srcIata, 0.0, 0.0);
            int destInternalId = graph.getOrAddAirport(destIata, 0.0, 0.0);
            
            graph.addFlight(srcInternalId, destInternalId, distanceKm, airline);
            rowCount++;
        } catch (...) { continue; }
    }
}
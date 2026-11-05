#include "FlightGraph.h"
#include "CsvLoader.h"
#include "Dijkstra.h"
#include "AStar.h"
#include "RadiusSearch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <cctype>

using Clock = std::chrono::high_resolution_clock;

bool fileExists(const std::string& path) {
    std::ifstream file(path);
    return file.is_open();
}

std::string readLine() {
    std::string line;
    std::getline(std::cin, line);
    size_t start = line.find_first_not_of(" \t\r");
    size_t end = line.find_last_not_of(" \t\r");
    if (start == std::string::npos) return "";
    return line.substr(start, end - start + 1);
}

int askForAirport(const FlightGraph& graph, const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string code = readLine();
        for (char& c : code) {
            c = std::toupper(static_cast<unsigned char>(c));
        }
        int id = graph.getAirportId(code);
        if (id != -1) {
            return id;
        }
        std::cout << "Airport not found, try again\n";
    }
}

double askForNumber(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        try {
            double value = std::stod(readLine());
            if (value >= 0.0) return value;
        } catch (...) {}
        std::cout << "Please enter a positive number\n";
    }
}

void printMenu() {
    std::cout << "\n===== SMART SKIES: Flightpath Optimizer =====\n"
              << "1  Shortest route (Dijkstra)\n"
              << "2  Shortest route (A*)\n"
              << "3  Compare Dijkstra vs A* on one route\n"
              << "4  Airports within a radius\n"
              << "5  Run benchmark\n"
              << "0  Exit\n"
              << "> Enter choice: ";
}

int main() {
    FlightGraph graph;
    std::string flightsFile = "data/flights.csv";
    if (!fileExists(flightsFile)) {
        flightsFile = "data/flights_sample.csv";
    }
    if (!fileExists("data/airports.csv") || !fileExists(flightsFile)) {
        std::cout << "Could not find data files. Run this from the project folder.\n";
        return 1;
    }

    auto loadStart = Clock::now();
    CsvLoader::loadAirports("data/airports.csv", graph);
    CsvLoader::loadFlights(flightsFile, graph);
    auto loadEnd = Clock::now();
    double loadMs = std::chrono::duration<double, std::milli>(loadEnd - loadStart).count();

    std::cout << "Loaded " << graph.getRouteCount() << " routes, "
              << graph.getAirportCount() << " airports from " << flightsFile
              << " in " << std::fixed << std::setprecision(0) << loadMs << " ms\n";

    while (true) {
        printMenu();
        std::string choice = readLine();
        if (choice == "0") {
            std::cout << "Goodbye!\n";
            break;
        } else {
            std::cout << "Invalid choice, try again\n";
        }
    }

    return 0;
}
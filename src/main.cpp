#include "FlightGraph.h"
#include "CsvLoader.h"
#include "Dijkstra.h"
#include "AStar.h"
#include "RadiusSearch.h"
#include "Benchmark.h"
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
//prints a route w/airport codes
void printPath(const FlightGraph& graph, const std::vector<int>& path) {
    const auto& airports = graph.getAirports();
    for (size_t i = 0; i < path.size(); i++) {
        if (i > 0) std::cout << " -> ";
        std::cout << airports[path[i]].iata;
    }
    std::cout << "\n";
}

int countLayovers(const std::vector<int>& path) {
    return path.size() < 2 ? 0 : static_cast<int>(path.size()) - 2;
}

void runSingleRoute(const FlightGraph& graph, bool useAStar) {
    int start = askForAirport(graph, "Origin (IATA): ");
    int end = askForAirport(graph, "Destination (IATA): ");
    auto t0 = Clock::now();
    bool found;
    double distance;
    int explored;
    std::vector<int> path;

    if (useAStar) {
        AStarResult r = aStar(graph, start, end);
        found = r.found; distance = r.totalDistance; explored = r.airportsExplored; path = r.path;
    } else {
        DijkstraResult r = dijkstra(graph, start, end);
        found = r.found; distance = r.totalDistance; explored = r.airportsExplored; path = r.path;
    }
    double ms = std::chrono::duration<double, std::milli>(Clock::now() - t0).count();

    if (!found) {
        std::cout << "No route found between these airports\n";
        return;
    }

    std::cout << "\nRoute: ";
    printPath(graph, path);
    std::cout << std::fixed << std::setprecision(1)
              << "Total distance:    " << distance << " km\n"
              << "Layovers:          " << countLayovers(path) << "\n"
              << "Airports explored: " << explored << "\n"
              << std::setprecision(3)
              << "Run time:          " << ms << " ms\n";
}

void compareRoute(const FlightGraph& graph) {
    int start = askForAirport(graph, "Origin (IATA): ");
    int end = askForAirport(graph, "Destination (IATA): ");
    auto t0 = Clock::now();
    DijkstraResult d = dijkstra(graph, start, end);
    double dMs = std::chrono::duration<double, std::milli>(Clock::now() - t0).count();
    auto t1 = Clock::now();
    AStarResult a = aStar(graph, start, end);
    double aMs = std::chrono::duration<double, std::milli>(Clock::now() - t1).count();

    if (!d.found) {
        std::cout << "No route found between these airports\n";
        return;
    }

    std::cout << "\nDijkstra route: "; printPath(graph, d.path);
    std::cout << "A* route:       "; printPath(graph, a.path);
    std::cout << "\n" << std::left << std::setw(20) << ""
              << std::setw(14) << "Dijkstra" << "A*\n"
              << std::fixed << std::setprecision(1)
              << std::setw(20) << "Total distance" << std::setw(14) << d.totalDistance << a.totalDistance << " km\n"
              << std::setw(20) << "Layovers" << std::setw(14) << countLayovers(d.path) << countLayovers(a.path) << "\n"
              << std::setw(20) << "Airports explored" << std::setw(14) << d.airportsExplored << a.airportsExplored << "\n"
              << std::setprecision(3)
              << std::setw(20) << "Run time (ms)" << std::setw(14) << dMs << aMs << "\n"
              << std::right;
}

void runRadiusSearch(const FlightGraph& graph) {
    int center = askForAirport(graph, "Center airport (IATA): ");
    double radius = askForNumber("Radius (km): ");
    std::vector<RadiusResult> results = radiusSearch(graph, center, radius);
    const auto& airports = graph.getAirports();
    std::cout << "\nFound " << results.size() << " airports:\n";

    for (const RadiusResult& r : results) {
        std::cout << std::left << std::setw(6) << airports[r.airportId].iata
                  << std::setw(50) << airports[r.airportId].name
                  << std::right << std::fixed << std::setprecision(1)
                  << r.distanceKm << " km\n";
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
        if (choice == "1") {
            runSingleRoute(graph, false);
        } else if (choice == "2") {
            runSingleRoute(graph, true);
        } else if (choice == "3") {
            compareRoute(graph);
        } else if (choice == "4") {
            runRadiusSearch(graph);
        } else if (choice == "5") {
            runBenchmark(graph, 1000, "results/results.csv");
        } else if (choice == "0") {
            std::cout << "Goodbye!\n";
            break;
        } else {
            std::cout << "Invalid choice, try again\n";
        }
    }

    return 0;
}
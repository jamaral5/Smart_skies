#include "Benchmark.h"
#include "Dijkstra.h"
#include "AStar.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <random>
#include <chrono>

using Clock = std::chrono::high_resolution_clock;


struct ScaleResult {
    int percent;
    int edges;
    double dijkstraMs;
    double aStarMs;
    double dijkstraNodes;
    double aStarNodes;
    double dijkstraMemoryKb;
    double aStarMemoryKb;
};

FlightGraph buildScaledGraph(const FlightGraph& full, double fraction) {
    FlightGraph scaled;
    const auto& airports = full.getAirports();

    for (const Airport& a : airports) {
        scaled.getOrAddAirport(a.iata, a.latitude, a.longitude, a.name);
    }

    for (int i = 0; i < full.getAirportCount(); i++) {
        const auto& edges = full.getNeighbors(i);
        int keep = static_cast<int>(edges.size() * fraction + 0.5);
        for (int j = 0; j < keep; j++) {
            scaled.addFlight(i, edges[j].destInternalId, edges[j].distanceKm, edges[j].airlineCode);
        }
    }
    return scaled;
}

void runBenchmark(const FlightGraph& full, int queriesPerScale, const std::string& outputPath) {
    int airportCount = full.getAirportCount();
    if (airportCount < 2) {
        std::cout << "Not enough airports to benchmark\n";
        return;
    }

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> pick(0, airportCount - 1);
    std::vector<std::pair<int, int>> queries;
    while (static_cast<int>(queries.size()) < queriesPerScale) {
        int a = pick(rng);
        int b = pick(rng);
        if (a != b) {
            queries.push_back({a, b});
        }
    }

    std::vector<ScaleResult> results;
    int percents[] = {25, 50, 75, 100};

    for (int percent : percents) {
        FlightGraph graph = buildScaledGraph(full, percent / 100.0);
        double dTotalMs = 0, aTotalMs = 0;
        long long dTotalNodes = 0, aTotalNodes = 0;

        for (const auto& q : queries) {
            auto t0 = Clock::now();
            DijkstraResult d = dijkstra(graph, q.first, q.second);
            dTotalMs += std::chrono::duration<double, std::milli>(Clock::now() - t0).count();
            dTotalNodes += d.airportsExplored;
            auto t1 = Clock::now();
            AStarResult a = aStar(graph, q.first, q.second);
            aTotalMs += std::chrono::duration<double, std::milli>(Clock::now() - t1).count();
            aTotalNodes += a.airportsExplored;
        }

        double heapKb = graph.getRouteCount() * sizeof(double) * 2 / 1024.0;
        double dMemKb = airportCount * (sizeof(double) + sizeof(int) + sizeof(bool)) / 1024.0 + heapKb;
        double aMemKb = airportCount * (2 * sizeof(double) + sizeof(int) + sizeof(bool)) / 1024.0 + heapKb;
        int n = static_cast<int>(queries.size());

        results.push_back({percent, graph.getRouteCount(),
                           dTotalMs / n, aTotalMs / n,
                           static_cast<double>(dTotalNodes) / n,
                           static_cast<double>(aTotalNodes) / n,
                           dMemKb, aMemKb});
    }
//prints the table
    std::cout << "\nQueries per scale factor: " << queriesPerScale << "\n\n"
              << std::left << std::setw(8) << "Scale" << std::setw(9) << "Edges"
              << std::setw(14) << "Dijkstra ms" << std::setw(10) << "A* ms"
              << std::setw(13) << "Dijk nodes" << std::setw(10) << "A* nodes" << "\n";
    for (const ScaleResult& r : results) {
        std::cout << std::fixed
                  << std::setw(8) << (std::to_string(r.percent) + "%")
                  << std::setw(9) << r.edges
                  << std::setprecision(4) << std::setw(14) << r.dijkstraMs << std::setw(10) << r.aStarMs
                  << std::setprecision(1) << std::setw(13) << r.dijkstraNodes << std::setw(10) << r.aStarNodes << "\n";
    }
    const ScaleResult& last = results.back();
    std::cout << std::right << std::setprecision(1)
              << "\nEstimated peak memory: Dijkstra " << last.dijkstraMemoryKb
              << " KB   A* " << last.aStarMemoryKb << " KB\n";
}
#include "RadiusSearch.h"
#include "FlightGraph.h"
#include <iostream>
#include <stdexcept>

void check(bool condition, const char* message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

void testFindsNearby() {
    FlightGraph graph;
    int center = graph.getOrAddAirport("AAA", 30.0, -80.0);
    graph.getOrAddAirport("BBB", 30.0, -79.0); 
    graph.getOrAddAirport("CCC", 40.0, -80.0); 
    auto results = radiusSearch(graph, center, 200.0);
    check(results.size() == 1, "Should find exactly one airport");
    check(graph.getAirports()[results[0].airportId].iata == "BBB", "Should find BBB");
}

void testSortedByDistance() {
    FlightGraph graph;
    int center = graph.getOrAddAirport("AAA", 30.0, -80.0);
    graph.getOrAddAirport("FAR", 30.0, -78.0);
    graph.getOrAddAirport("NEAR", 30.0, -79.5);
    auto results = radiusSearch(graph, center, 500.0);
    check(results.size() == 2, "Should find two airports");
    check(results[0].distanceKm <= results[1].distanceKm, "Results should be sorted");
}

void testExcludesCenter() {
    FlightGraph graph;
    int center = graph.getOrAddAirport("AAA", 30.0, -80.0);
    auto results = radiusSearch(graph, center, 1000.0);
    check(results.empty(), "Center should not appear in results");
}

void testInvalidAirport() {
    FlightGraph graph;
    graph.getOrAddAirport("AAA", 30.0, -80.0);
    bool threw = false;

    try {
        radiusSearch(graph, 99, 100.0);
    } 
    catch (const std::out_of_range&) {
        threw = true;
    }
    
    check(threw, "Invalid airport should throw");
}

int main() {
    try {
        testFindsNearby();
        testSortedByDistance();
        testExcludesCenter();
        testInvalidAirport();
        std::cout << "All RadiusSearch tests passed\n";
        return 0;
    } catch (const std::exception& e) {
        std::cout << "Test failed: " << e.what() << "\n";
        return 1;
    }
}
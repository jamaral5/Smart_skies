
#include "Dijkstra.h"

#include <iostream>
#include <stdexcept>
#include <cmath>
#include <vector>
#include <string>

void checkDijkstra(bool condition, const char* message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

//compares decimal distances with a small tolerance
bool sameDistance(double a, double b) {
    return std::abs(a - b) < 0.000001;
}

//adds test airports
void addTestAirports(FlightGraph& graph, int count) {
    for (int i = 0; i < count; i++) {
        std::string code = "TEST" + std::to_string(i);
        graph.getOrAddAirport(code, 30.0, -80.0);
    }
}

//Test 1: checks a simple direct route
void testDirectRoute() {
    FlightGraph graph;
    addTestAirports(graph, 2);

    graph.addFlight(0, 1, 500.0, "TEST");

    DijkstraResult result = dijkstra(graph, 0, 1);

    checkDijkstra(result.found, "Direct route not found");
    checkDijkstra(sameDistance(result.totalDistance, 500.0),"Incorrect direct route distance");
    checkDijkstra(result.path == std::vector<int>({0, 1}),"Incorrect direct route path");
}

//Test 2: checks that the shortest route is selected
void testShortestRoute() {
    FlightGraph graph;
    addTestAirports(graph, 4);

    graph.addFlight(0, 1, 650.0, "TEST");
    graph.addFlight(1, 3, 3100.0, "TEST");
    graph.addFlight(0, 2, 1600.0, "TEST");
    graph.addFlight(2, 3, 2000.0, "TEST");
    graph.addFlight(0, 3, 4000.0, "TEST");

    DijkstraResult result = dijkstra(graph, 0, 3);

    checkDijkstra(result.found, "Shortest route not found");
    checkDijkstra(sameDistance(result.totalDistance, 3600.0),"Incorrect shortest route distance");
    checkDijkstra(result.path == std::vector<int>({0, 2, 3}),"Incorrect shortest route path");
}

//Test 3: checks an unreachable destination
void testUnreachableAirport() {
    FlightGraph graph;
    addTestAirports(graph, 3);

    graph.addFlight(0, 1, 100.0, "TEST");

    DijkstraResult result = dijkstra(graph, 0, 2);

    checkDijkstra(!result.found, "Unreachable route was found");
    checkDijkstra(result.path.empty(),"Unreachable route should have an empty path");
}

//Test 4: checks routes to the same airport
void testSameAirport() {
    FlightGraph graph;
    addTestAirports(graph, 3);

    DijkstraResult result = dijkstra(graph, 1, 1);

    checkDijkstra(result.found, "Same-airport route not found");
    checkDijkstra(sameDistance(result.totalDistance, 0.0),"Same-airport distance should be zero");
    checkDijkstra(result.path == std::vector<int>({1}),"Same-airport path incorrect");
}

//Test 5: checks that routes are directed
void testDirectedRoutes() {
    FlightGraph graph;
    addTestAirports(graph, 2);

    graph.addFlight(0, 1, 200.0, "TEST");

    DijkstraResult forward = dijkstra(graph, 0, 1);
    DijkstraResult backward = dijkstra(graph, 1, 0);

    checkDijkstra(forward.found, "Forward route not found");
    checkDijkstra(!backward.found,"Reverse route should not exist");
}

//Test 6: checks improved shortest distances
void testImprovedRoute() {
    FlightGraph graph;
    addTestAirports(graph, 3);

    graph.addFlight(0, 1, 500.0, "TEST");
    graph.addFlight(0, 2, 100.0, "TEST");
    graph.addFlight(2, 1, 200.0, "TEST");

    DijkstraResult result = dijkstra(graph, 0, 1);

    checkDijkstra(result.found, "Improved route not found");
    checkDijkstra(sameDistance(result.totalDistance, 300.0),"Improved route distance incorrect");
    checkDijkstra(result.path == std::vector<int>({0, 2, 1}),"Improved route path incorrect");
}

//Test 7: checks cycles in the graph
void testCycle() {
    FlightGraph graph;
    addTestAirports(graph, 3);

    graph.addFlight(0, 1, 100.0, "TEST");
    graph.addFlight(1, 0, 100.0, "TEST");
    graph.addFlight(1, 2, 200.0, "TEST");

    DijkstraResult result = dijkstra(graph, 0, 2);

    checkDijkstra(result.found, "Route through cycle not found");
    checkDijkstra(sameDistance(result.totalDistance, 300.0),"Cycle route distance incorrect");
    checkDijkstra(result.path == std::vector<int>({0, 1, 2}),"Cycle route path incorrect");
}

//Test 8: checks decimal distances
void testDecimalDistances() {
    FlightGraph graph;
    addTestAirports(graph, 3);

    graph.addFlight(0, 1, 125.5, "TEST");
    graph.addFlight(1, 2, 200.25, "TEST");
    graph.addFlight(0, 2, 400.0, "TEST");

    DijkstraResult result = dijkstra(graph, 0, 2);

    checkDijkstra(result.found, "Decimal route not found");
    checkDijkstra(sameDistance(result.totalDistance, 325.75),"Decimal distance incorrect");
}

//Test 9: checks invalid airport IDs
void testInvalidAirport() {
    FlightGraph graph;
    addTestAirports(graph, 3);

    bool exceptionThrown = false;

    try {
        dijkstra(graph, 0, 5);
    } catch (const std::out_of_range&) {
        exceptionThrown = true;
    }

    checkDijkstra(exceptionThrown,"Invalid airport should throw an exception");
}

//Test 10: checks the number of airports explored
void testAirportsExplored() {
    FlightGraph graph;
    addTestAirports(graph, 4);

    graph.addFlight(0, 1, 100.0, "TEST");
    graph.addFlight(1, 2, 100.0, "TEST");
    graph.addFlight(2, 3, 100.0, "TEST");

    DijkstraResult result = dijkstra(graph, 0, 3);

    checkDijkstra(result.found, "Route not found");
    checkDijkstra(result.airportsExplored == 4,"Incorrect number of airports explored");
}

//runs all dijkstra tests
int main() {
    try {
        testDirectRoute();
        testShortestRoute();
        testUnreachableAirport();
        testSameAirport();
        testDirectedRoutes();
        testImprovedRoute();
        testCycle();
        testDecimalDistances();
        testInvalidAirport();
        testAirportsExplored();

        std::cout << "All Dijkstra tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Dijkstra test failed: " << error.what() << std::endl;
        return 1;
    }
}

#include "AStar.h"
#include "Dijkstra.h"

#include <iostream>
#include <stdexcept>
#include <cmath>
#include <vector>

void checkAStar(bool condition, const char* message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

//compares decimal distances with a small tolerance
bool sameAStarDistance(double a, double b) {
    return std::abs(a - b) < 0.000001;
}

//Test 1: checks a simple direct flight
void testDirectFlight() {
    FlightGraph graph;

    int a = graph.getOrAddAirport("AAA", 30.0, -80.0);
    int b = graph.getOrAddAirport("BBB", 30.0, -79.0);

    graph.addFlight(a, b, 150.0, "TEST");

    AStarResult result = aStar(graph, a, b);

    checkAStar(result.found, "Direct flight not found");
    checkAStar(sameAStarDistance(result.totalDistance, 150.0),"Incorrect direct flight distance");
    checkAStar(result.path == std::vector<int>({a, b}),"Incorrect direct flight path");
}

//Test 2: compares a* with dijkstra
void testShortestRoute() {
    FlightGraph graph;

    int a = graph.getOrAddAirport("AAA", 30.0, -80.0);
    int b = graph.getOrAddAirport("BBB", 30.0, -79.0);
    int c = graph.getOrAddAirport("CCC", 31.0, -80.0);
    int d = graph.getOrAddAirport("DDD", 30.0, -78.0);

    graph.addFlight(a, b, 100.0, "TEST");
    graph.addFlight(b, d, 100.0, "TEST");
    graph.addFlight(a, c, 150.0, "TEST");
    graph.addFlight(c, d, 800.0, "TEST");
    graph.addFlight(a, d, 400.0, "TEST");

    AStarResult astarResult = aStar(graph, a, d);
    DijkstraResult dijkstraResult = dijkstra(graph, a, d);

    checkAStar(astarResult.found, "A* route not found");
    checkAStar(sameAStarDistance(astarResult.totalDistance, 200.0),"A* returned wrong distance");
    checkAStar(astarResult.path == std::vector<int>({a, b, d}),"A* returned wrong path");

    checkAStar(sameAStarDistance(astarResult.totalDistance, dijkstraResult.totalDistance),"A* and Dijkstra returned different distances");

    checkAStar(astarResult.airportsExplored <= dijkstraResult.airportsExplored,"A* explored more airports on this test graph");
}

//Test 3: checks an unreachable airport
void testUnreachableRoute() {
    FlightGraph graph;

    int a = graph.getOrAddAirport("AAA", 30.0, -80.0);
    int b = graph.getOrAddAirport("BBB", 30.0, -79.0);
    int c = graph.getOrAddAirport("CCC", 30.0, -78.0);

    graph.addFlight(a, b, 150.0, "TEST");

    AStarResult result = aStar(graph, a, c);

    checkAStar(!result.found, "Unreachable route was found");
    checkAStar(result.path.empty(),"Unreachable route should have an empty path");
}

//Test 4: checks a route to the same airport
void testSameAirport() {
    FlightGraph graph;

    int a = graph.getOrAddAirport("AAA", 30.0, -80.0);

    AStarResult result = aStar(graph, a, a);

    checkAStar(result.found, "Same-airport route not found");
    checkAStar(sameAStarDistance(result.totalDistance, 0.0),"Same-airport distance should be zero");
    checkAStar(result.path == std::vector<int>({a}),"Same-airport path incorrect");
}

//Test 5 checks that flights are directed
void testDirectedFlights() {
    FlightGraph graph;

    int a = graph.getOrAddAirport("AAA", 30.0, -80.0);
    int b = graph.getOrAddAirport("BBB", 30.0, -79.0);

    graph.addFlight(a, b, 150.0, "TEST");

    AStarResult forward = aStar(graph, a, b);
    AStarResult backward = aStar(graph, b, a);

    checkAStar(forward.found, "Forward route not found");
    checkAStar(!backward.found,"Reverse route should not exist");
}

//Test 6: checks that a better route can replace an old route
void testImprovedRoute() {
    FlightGraph graph;

    int a = graph.getOrAddAirport("AAA", 30.0, -80.0);
    int b = graph.getOrAddAirport("BBB", 30.0, -79.0);
    int c = graph.getOrAddAirport("CCC", 30.0, -78.0);

    graph.addFlight(a, c, 500.0, "TEST");
    graph.addFlight(a, b, 120.0, "TEST");
    graph.addFlight(b, c, 120.0, "TEST");

    AStarResult result = aStar(graph, a, c);

    checkAStar(result.found, "Improved route not found");
    checkAStar(sameAStarDistance(result.totalDistance, 240.0),"Improved route distance incorrect");
    checkAStar(result.path == std::vector<int>({a, b, c}),"Improved route path incorrect");
}

//Test 7: checks missing airport coordinates
void testMissingCoordinates() {
    FlightGraph graph;

    int a = graph.getOrAddAirport("AAA", 0.0, 0.0);
    int b = graph.getOrAddAirport("BBB", 0.0, 0.0);
    int c = graph.getOrAddAirport("CCC", 0.0, 0.0);

    graph.addFlight(a, b, 100.0, "TEST");
    graph.addFlight(b, c, 100.0, "TEST");
    graph.addFlight(a, c, 300.0, "TEST");

    AStarResult result = aStar(graph, a, c);

    checkAStar(result.found, "Route with missing coordinates not found");
    checkAStar(sameAStarDistance(result.totalDistance, 200.0),"Incorrect distance with missing coordinates");
}

//Test 8: checks zero-distance connections
void testZeroDistance() {
    FlightGraph graph;

    int a = graph.getOrAddAirport("AAA", 0.0, 0.0);
    int b = graph.getOrAddAirport("BBB", 0.0, 0.0);
    int c = graph.getOrAddAirport("CCC", 0.0, 0.0);

    graph.addFlight(a, b, 0.0, "TEST");
    graph.addFlight(b, c, 50.0, "TEST");
    graph.addFlight(a, c, 100.0, "TEST");

    AStarResult result = aStar(graph, a, c);

    checkAStar(result.found, "Zero-distance route not found");
    checkAStar(sameAStarDistance(result.totalDistance, 50.0),"Zero-distance route incorrect");
}

//Test 9: checks invalid airport IDs
void testInvalidAirport() {
    FlightGraph graph;

    graph.getOrAddAirport("AAA", 30.0, -80.0);

    bool exceptionThrown = false;

    try {
        aStar(graph, 0, 5);
    } catch (const std::out_of_range&) {
        exceptionThrown = true;
    }

    checkAStar(exceptionThrown,"Invalid airport should throw an exception");
}

//runs all a* tests
int main() {
    try {
        testDirectFlight();
        testShortestRoute();
        testUnreachableRoute();
        testSameAirport();
        testDirectedFlights();
        testImprovedRoute();
        testMissingCoordinates();
        testZeroDistance();
        testInvalidAirport();

        std::cout << "All AStar tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "AStar test failed: "
                  << error.what() << std::endl;
        return 1;
    }
}

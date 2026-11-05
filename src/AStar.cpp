
#include "AStar.h"
#include "MinHeap.h"
#include "Heuristic.h"

#include <vector>
#include <limits>
#include <stdexcept>
#include <cmath>

static double estimateDistance(const FlightGraph& graph, int airportID, int destinationID) {
    //estimates the remaining distance to the destination
    const std::vector<Airport>& airports = graph.getAirports();
    const Airport& current = airports[airportID];
    const Airport& destination = airports[destinationID];

    //uses zero when airport coordinates are missing or invalid
    if (!std::isfinite(current.latitude) ||
        !std::isfinite(current.longitude) ||
        !std::isfinite(destination.latitude) ||
        !std::isfinite(destination.longitude) ||
        current.latitude < -90.0 || current.latitude > 90.0 ||
        destination.latitude < -90.0 || destination.latitude > 90.0 ||
        current.longitude < -180.0 || current.longitude > 180.0 ||
        destination.longitude < -180.0 || destination.longitude > 180.0 ||
        (current.latitude == 0.0 && current.longitude == 0.0) ||
        (destination.latitude == 0.0 && destination.longitude == 0.0)) {
        return 0.0;
    }

    return Heuristic::calculate(airportID, destinationID, graph);
}

//finds the shortest route using a* search
AStarResult aStar(const FlightGraph& graph, int startAirport, int destinationAirport) {

    int airportCount = graph.getAirportCount();

    //checks that both airport IDs exist
    if (startAirport < 0 || startAirport >= airportCount || destinationAirport < 0 || destinationAirport >= airportCount) {
        throw std::out_of_range("Invalid airport ID");
    }

    AStarResult result;

    //shortest known distance from the starting airport
    std::vector<double> distances(airportCount,std::numeric_limits<double>::infinity());

    //best estimated total cost for each airport
    std::vector<double> bestPriority(airportCount,std::numeric_limits<double>::infinity());

    //previous airport along each shortest path
    std::vector<int> previous(airportCount, -1);

    //tracks which airports have been counted as explored
    std::vector<bool> explored(airportCount, false);

    MinHeap frontier;
    distances[startAirport] = 0.0;
    bestPriority[startAirport] = estimateDistance(graph, startAirport, destinationAirport);
    frontier.push(startAirport, bestPriority[startAirport]);

    //continues searching while reachable airports remain
    while (!frontier.empty()) {

        HeapNode current = frontier.pop();
        int currentAirport = current.airportID;

        //skips older entries with worse priorities
        if (current.priority > bestPriority[currentAirport]) {
            continue;
        }

        //counts each airport only once for benchmarking
        if (!explored[currentAirport]) {
            explored[currentAirport] = true;
            result.airportsExplored++;
        }

        //stops when the destination is reached
        if (currentAirport == destinationAirport) {
            result.found = true;
            result.totalDistance = distances[currentAirport];
            break;
        }

        const std::vector<FlightEdge>& neighbors = graph.getNeighbors(currentAirport);

        //examines each neighboring airport
        for (const FlightEdge& edge : neighbors) {

            int nextAirport = edge.destInternalId;

            if (nextAirport < 0 || nextAirport >= airportCount) {
                throw std::out_of_range("Invalid edge destination");
            }

            //requires valid nonnegative flight distances
            if (!std::isfinite(edge.distanceKm) || edge.distanceKm < 0.0) {
                throw std::invalid_argument("Invalid flight distance");
            }

            //calculates the distance traveled through this route
            double newDistance = distances[currentAirport] + edge.distanceKm;

            //updates the airport if a shorter route is discovered
            if (newDistance < distances[nextAirport]) {

                distances[nextAirport] = newDistance;
                previous[nextAirport] = currentAirport;

                //calculates the estimated total route cost
                double remainingDistance = estimateDistance(graph, nextAirport, destinationAirport);
                double priority = newDistance + remainingDistance;
                bestPriority[nextAirport] = priority;

                //inserts the improved route into the heap
                frontier.push(nextAirport, priority);
            }
        }
    }

    if (!result.found) {
        return result;
    }

    //reconstructs the path from destination to origin
    int airport = destinationAirport;

    while (airport != -1) {
        result.path.push_back(airport);
        airport = previous[airport];
    }

    //reverses the path to place the origin first
    std::vector<int> correctPath;

    for (int i = static_cast<int>(result.path.size()) - 1;
         i >= 0; i--) {
        correctPath.push_back(result.path[i]);
    }

    result.path = correctPath;

    return result;
}

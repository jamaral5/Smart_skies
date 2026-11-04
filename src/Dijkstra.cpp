
#include "Dijkstra.h"
#include "MinHeap.h"

#include <vector>
#include <limits>
#include <stdexcept>


DijkstraResult dijkstra(const FlightGraph& graph, int startAirport, int destinationAirport) {
    //finds the shortest route using dijkstra's algorithm
    int airportCount = graph.getAirportCount();

    //checks that both airport IDs exist
    if (startAirport < 0 || startAirport >= airportCount || destinationAirport < 0 || destinationAirport >= airportCount) {
        throw std::out_of_range("Invalid airport ID");
    }

    DijkstraResult result;

    //stores the shortest known distance to each airport
    std::vector<double> distances(airportCount, std::numeric_limits<double>::infinity()
    );

    //stores the previous airport along each shortest path
    std::vector<int> previous(airportCount, -1);

    //tracks airports that have already been explored
    std::vector<bool> visited(airportCount, false);

    //uses the custom min-heap to select the next airport
    MinHeap frontier;

    distances[startAirport] = 0.0;
    frontier.push(startAirport, 0.0);

    //continues searching while reachable airports remain
    while (!frontier.empty()) {

        HeapNode current = frontier.pop();
        int currentAirport = current.airportID;

        //skips outdated entries already processed
        if (visited[currentAirport]) {
            continue;
        }

        //marks the airport as explored
        visited[currentAirport] = true;
        result.airportsExplored++;

        //stops once the destination has been reached
        if (currentAirport == destinationAirport) {
            result.found = true;
            result.totalDistance = distances[currentAirport];
            break;
        }

        //retrieves outgoing connections from the graph
        //may need adjustment if anthony changes the graph interface
        const std::vector<FlightEdge>& neighbors = graph.getNeighbors(currentAirport);

        //checks each neighboring airport
        for (const FlightEdge& edge : neighbors) {

            int nextAirport = edge.destination;

            //rejects invalid airport references
            if (nextAirport < 0 || nextAirport >= airportCount) {
                throw std::out_of_range("Invalid edge destination");
            }

            //dijkstra requires nonnegative edge weights
            if (edge.distance < 0.0) {
                throw std::invalid_argument("Negative flight distance");
            }

            //skips airports whose shortest paths are already known
            if (visited[nextAirport]) {
                continue;
            }

            //calculates the distance through the current airport
            double newDistance =
                distances[currentAirport] + edge.distance;

            //updates the route if a shorter one is found
            if (newDistance < distances[nextAirport]) {
                distances[nextAirport] = newDistance;
                previous[nextAirport] = currentAirport;

                //inserts the improved route into the heap
                frontier.push(nextAirport, newDistance);
            }
        }
    }

    //returns an empty path if no route was found
    if (!result.found) {
        return result;
    }

    //reconstructs the route by following previous airports
    int airport = destinationAirport;

    while (airport != -1) {
        result.path.push_back(airport);
        airport = previous[airport];
    }

    //reverses the path so it starts at the origin
    std::vector<int> correctPath;

    for (int i = static_cast<int>(result.path.size()) - 1;
         i >= 0; i--) {
        correctPath.push_back(result.path[i]);
    }

    result.path = correctPath;

    return result;
}

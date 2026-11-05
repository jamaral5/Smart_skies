# Smart Skies: Flightpath Optimizer

A C++ console program that finds the shortest flight route between U.S. airports and compares two shortest-path algorithms, Dijkstra and A\* along with a custom binary min heap.

**Team:** Smart Skies
| Name | GitHub |
|---|---|
| Jake Amaral | jamaral5 |
| Anthony Vargas | AntVa0403 |
| Rylie Troendle | RTroendle |

## Features
1. **Shortest route (Dijkstra)**: finds the shortest route between two airports by distance
2. **Shortest route (A\*)**: same route but uses straight-line distance to guide the search
3. **Compare Dijkstra vs A\***: runs both on one route and shows distance, layovers, airports explored, and run time
4. **Airports within a radius**: lists every airport within x km of a chosen airport with closest first
5. **Benchmark**: times both algorithms on 1,000 random routes at 25-100% of the network and saves the results to `results/results.csv`

## Dataset
[2015 Flight Delays and Cancellations](https://www.kaggle.com/datasets/usdot/flight-delays) (U.S. Department of Transportation, via Kaggle)

- `data/airports.csv`: 322 US airports with coordinates
- `data/flights_sample.csv`: first 100,000 flights
- `data/flights.csv`: all 5.8 million flights (not included because too big for GitHub)

**To use the full dataset:** download `flights.csv` from the Kaggle link and put it in the `data/` folder.

## How to Build and Run

You need a C++17 compiler, and CMake is optional.

### With CMake
```bash
cmake -B build
cmake --build build
./build/Smart_skies
```

### With g++ directly
```bash
g++ -std=c++17 -O2 -Iinclude src/*.cpp -o smart_skies
./smart_skies
```

> **run the program from the main project folder (the one containing `data/`) so it can find the data files.**

## Example Usage
```
Loaded 4671 routes, 319 airports from data/flights.csv

===== SMART SKIES: Flightpath Optimizer =====
1  Shortest route (Dijkstra)
2  Shortest route (A*)
3  Compare Dijkstra vs A* on one route
4  Airports within a radius
5  Run benchmark
0  Exit
```
Try:
- **Option 3**, origin `MCO`, destination `LAX`
- **Option 4**, center `GNV`, radius `300`
- **Option 5** to regenerate `results/results.csv`

## Running the Tests
```bash
cmake --build build
cd build && ctest
```
Or build one test with g++, for example:
```bash
g++ -std=c++17 -Iinclude tests/test_RadiusSearch.cpp src/RadiusSearch.cpp src/FlightGraph.cpp src/Heuristic.cpp -o test_radius
./test_radius
```

## Project Structure
```
include/   header files
src/       source files (main.cpp, algorithms, graph, loader, benchmark)
tests/     unit tests for the min-heap, Dijkstra, A*, and radius search
data/      CSV data files
results/   benchmark output
```

## Data Structures and Algorithms
- **FlightGraph**: weighted, directed adjacency list (airports = vertices, routes = edges)
- **MinHeap**: binary min heap used as the priority queue
- **Dijkstra's algorithm**: O((V + E) log V)
- **A\* search**: O((V + E) log V), using the haversine distance as its heuristic

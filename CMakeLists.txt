cmake_minimum_required(VERSION 3.14)
project(Smart_skies)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(Smart_skies
        src/main.cpp
        src/AStar.cpp
        src/Benchmark.cpp
        src/CsvLoader.cpp
        src/Dijkstra.cpp
        src/FlightGraph.cpp
        src/Heuristic.cpp
        src/MinHeap.cpp
        src/RadiusSearch.cpp
)

enable_testing()

add_executable(test_MinHeap
        tests/test_MinHeap.cpp
        src/MinHeap.cpp
)

add_executable(test_Dijkstra
        tests/test_Dijkstra.cpp
        src/Dijkstra.cpp
        src/MinHeap.cpp
        src/FlightGraph.cpp
)

add_executable(test_AStar
        tests/test_AStar.cpp
        src/AStar.cpp
        src/Dijkstra.cpp
        src/MinHeap.cpp
        src/FlightGraph.cpp
        src/Heuristic.cpp
)

add_executable(test_RadiusSearch
        tests/test_RadiusSearch.cpp
        src/RadiusSearch.cpp
        src/FlightGraph.cpp
        src/Heuristic.cpp
)

add_test(NAME MinHeapTests COMMAND test_MinHeap)
add_test(NAME DijkstraTests COMMAND test_Dijkstra)
add_test(NAME AStarTests COMMAND test_AStar)
add_test(NAME RadiusSearchTests COMMAND test_RadiusSearch)

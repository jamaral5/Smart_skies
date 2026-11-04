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


add_executable(test_MinHeap
        tests/test_MinHeap.cpp
        src/MinHeap.cpp
)

target_include_directories(test_MinHeap PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

enable_testing()
add_test(NAME MinHeapTests COMMAND test_MinHeap)

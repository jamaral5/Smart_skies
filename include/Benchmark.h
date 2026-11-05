#pragma once
#include <string>
#include "FlightGraph.h"

FlightGraph buildScaledGraph(const FlightGraph& full, double fraction);
void runBenchmark(const FlightGraph& full, int queriesPerScale, const std::string& outputPath);
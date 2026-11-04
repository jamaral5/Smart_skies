#ifndef CSVLOADER_H
#define CSVLOADER_H

#include <string>
#include "FlightGraph.h"

class CsvLoader {
public:
    static void loadAirports(const std::string& filepath, FlightGraph& graph);
    static void loadFlights(const std::string& filepath, FlightGraph& graph, int maxRows = -1);
};

#endif
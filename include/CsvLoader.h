#ifndef CSVLOADER_H
#define CSVLOADER_H

#include <string>
#include "FlightGraph.h"

class CsvLoader {
public:
    static void loadFlightData(const std::string& filepath, FlightGraph& graph);
};

#endif
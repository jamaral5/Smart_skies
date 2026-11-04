


//Some notes for integration with Dijkstra.h/Dijkstra.cpp implementations:
//If you need to deviate from these proposed integrations, let me know and I (or you) can modify the files in question to ensure they integrate properly.


//Airport IDs must be consecutive integers from 0 through getAirportCount() - 1. Real airport IDs or IATA codes can be mapped to these internal indices by the loader.

//getNeighbors() must return the outgoing flights for the requested airport.

//addEdge() must add a directed connection, not automatically create a return flight.

//Every edge's distance must be a finite, nonnegative value in kilometers.
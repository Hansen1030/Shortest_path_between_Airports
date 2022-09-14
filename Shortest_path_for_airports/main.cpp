#include "airports.h"
#include "airports_graph.h"
#include <iostream>

int main() {
    Airports airports;
    airports.readFromFile("airports data.csv");
    AirportsGraph graph(airports);
    graph.makeMatrix("routes.csv");
    vector<string> output = graph.findShortestPath("JFK", "PKU");
    for (unsigned i = 0; i < output.size(); i++) {
        std::cout << output[i] << " " << std::endl;
    }
    vector<string> out = graph.findCentralAirport();
    std::cout << "The most central/important airport(s) is(are) " << std::endl;
    for (unsigned i = 0; i < output.size(); i++) {
        std::cout << out[i] << std::endl;
    }
    std::cout << "Therefore, this(these) airport(s) should have the highest priorities in security protection."
            << std::endl;
}
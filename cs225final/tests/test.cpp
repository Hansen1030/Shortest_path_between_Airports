#include "../cs225/catch/catch.hpp"
#include "../airports.h"
#include "../airports_graph.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

using std::string;
using std::vector;
using std::ifstream;
using std::stringstream;
using std::stod;

/**
 * the result vector should be identical to the solution
 * 
 * @param solutionFileName the file name of solution
 * @param result result vector after called readFromFile
 * 
 * @return true if two sources are identical
 */
bool verifierForRead(const string& solutionFileName, const vector<Airports::AirportEle>& result) {
    unsigned count;
    count = 0;
    string line;
    ifstream infile(solutionFileName);
    if (infile.is_open()) {
        while (getline(infile, line)) {
            vector<string> strings;
            stringstream s_stream(line);
            while (s_stream.good()) {
                string substr;
                getline(s_stream, substr, ',');
                strings.push_back(substr);
            }
            const Airports::AirportEle& curr = result[count++];
            if (strings[0] != curr.airport || strings[1] != curr.airportID || stod(strings[2]) != curr.latitude
                    || stod(strings[3]) != curr.longitude || stod(strings[4]) != curr.altitude) {
                return false;
            }
        }
    }
    return true;
}

/**
 * check if all the vertices have been visited
 * 
 * @param isVisited the vector that stores whether vertices are visited or not
 * 
 * @return true if all vertices have been visited
 */
bool verifierForDFSVertices(const vector<bool>& isVisited) {
    for (bool b : isVisited) {
        if (!b) {
            return b;
        }
    }
    return true;
}

/**
 * check if all edges are traversed
 * 
 * @param edges the 2-d vector that stores edge traversal
 * @param adjMatrix the adjacency matrix
 * 
 * @return true if all existed edges have been traversed
 */
bool verifierForDFSEdges(const vector<vector<double>>& edges, const vector<vector<double>>& adjMatrix) {
    unsigned width = adjMatrix.size();
    unsigned height = adjMatrix[0].size();
    for (unsigned i = 0; i < width; ++i) {
        for (unsigned j = 0; j < height; ++j) {
            if (adjMatrix[i][j] == 0 && edges[i][j] == 0) {
                continue;
            } else if (adjMatrix[i][j] > 0 && edges[i][j] > 0) {
                continue;
            } else {
                return false;
            }
        }
    }
    return true;
}

/**
 * print out the path given a vector of strings
 * 
 * @param path the returned vector from findShortestPath
 * 
 * @return the shortest path in string
 */
string printPath(const vector<string>& path) {
    string result;
    for (const string& s : path) {
        result += s;
    }
    return result;
}

////////////////////////////////////////////////////////////////////////
////////////////////     Simple weighted graph     /////////////////////
////////////////////////////////////////////////////////////////////////

/**
 * 
 * (longtitude/degrees)
 * 7                            ---j----k
 *                             /
 * 6                       ---i
 *                        /
 * 5 l---             ---f----h
 *       \           /        |
 * 4      b---   ---c         |
 *            \ /             |
 * 3        ---a---           |
 *         /       \          |
 * 2 m----e         d         |
 *        |          \        |
 * 1      |           ---g    |
 *        ---------------------
 * 0 n
 *   0    1    2    3    4    5    6    7 (latitude/degrees)
 * 
 * PS: If there's a path between two vertices, it means it can go both ways in this example.
 * And, altitudes for all vertices are zero.
 */

TEST_CASE("Test Airports::readFromFile for complete and good data", "[weight=1][valgrind]") {
    Airports airports;
    airports.readFromFile("tests/test case 1 airports.csv");
    REQUIRE( verifierForRead("tests/sol_simple_read.csv", airports.getAirports()) );
}

TEST_CASE("Test Airports::readFromFile for bad data", "[weight=1][valgrind]") {
    Airports airports;
    airports.readFromFile("tests/test case 2 airports.csv");

    // discard the last two airports' information, since the name of number 15 airport contains comma
    // and number 16 airport is missing its 3-letter ID.
    REQUIRE( airports.getAirports().size() == 14 );

    REQUIRE( verifierForRead("tests/sol_simple_read.csv", airports.getAirports()) );
}

TEST_CASE("Test AirportsGraph::dfsTraversal", "[weight=2][valgrind") {
    Airports airports;
    airports.readFromFile("tests/test case 1 airports.csv");
    AirportsGraph graph(airports);
    graph.makeMatrix("tests/test case 1 routes.csv");
    graph.DFS("AAA");
    REQUIRE( verifierForDFSVertices(graph.getVertices()) );
    REQUIRE( verifierForDFSEdges(graph.getEdges(), graph.getAdjMatrix()) );
}

TEST_CASE("Test AirportsGraph::findShortestPath", "[weight=4][valgrind]") {
    Airports airports;
    airports.readFromFile("tests/test case 1 airports.csv");
    AirportsGraph graph(airports);
    graph.makeMatrix("tests/test case 1 routes.csv");

    vector<string> pathAB = graph.findShortestPath("AAA", "AAB"); // a and b
    REQUIRE( printPath(pathAB) == "ab" );

    vector<string> pathLK = graph.findShortestPath("AAL", "AAK"); // l and k
    REQUIRE( printPath(pathLK) == "lbacfijk" );
    
    vector<string> pathEH = graph.findShortestPath("AAE", "AAH"); // e and h
    REQUIRE( printPath(pathEH) == "eh");

    vector<string> pathLN = graph.findShortestPath("AAL", "DIS"); // l and n
    REQUIRE( printPath(pathLN) == "can not reach");
}

TEST_CASE("Test AirportsGraph::findCentralAirport", "[weight=2][valgrind") {
    Airports airports;
    airports.readFromFile("tests/test case 1 airports.csv");
    AirportsGraph graph(airports);
    graph.makeMatrix("tests/test case 1 routes.csv");

    vector<string> result = graph.findCentralAirport();
    REQUIRE( result.size() == 1 );
    REQUIRE( result[0] == "a" );
}
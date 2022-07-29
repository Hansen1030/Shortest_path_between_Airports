#pragma once

#include "airports.h"
#include <iostream>
#include <list>
#include <vector>
#include  <stack>

class AirportsGraph {
    public:
        AirportsGraph(const Airports& airports);
        /**
         * Make the adjcency Matrix by this file. 
         * If two airports do not have route or they are the same, 
         * the double should be 0. 
         * Otherwise, the double is the distance of these two airports.
         * 
         * @param filename the name of file to read from
         */ 
        void makeMatrix(const string& filename);
        vector<string> findShortestPath(const string& airport1, const string& airport2);
        vector<string> findCentralAirport();
        // dfs of adjmatrix
        void DFS(const string& airport);
        const vector<vector<double>>& getEdges() const;
        const vector<bool>& getVertices() const;
        const vector<vector<double>>& getAdjMatrix() const;

    private:
        /**
         * Calculate the distance from a source airport to a destination aiport, given 
         * their latitudes, longitudes, and altitudes (in feet). The steps are 
         * 1. Get the current radius, given the altitude; 
         * 2. Transform spherical coordinates to cartesian coordinates; 
         * 3. Use Euclidean formula to calculate the distance.
         * 
         * @param idx_1 index of the source airport in the vector airports_ of class Airports
         * @param idx_2 index of the destination airport in the vector airports_ of class Airports
         */
        double calculateDistance(unsigned idx_1, unsigned idx_2);
        void dfsTraversal(unsigned vertex, vector<bool>& visited_);
        double inf; // infinity
        vector<vector<double> > adjMatrix_;
        vector<vector<vector<double> > > spm;// shortest path matrix. 
        unsigned numAirports_;
        const Airports *airports_;
        vector<bool> visited_;
        // if an airport is visited
        vector<vector<double>> edgeexist_;
        // if edge exist between airports
        // 0, no , 1 discovery edge, 2 back edge
        

};
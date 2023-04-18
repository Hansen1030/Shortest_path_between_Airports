#pragma once

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>

using std::vector;
using std::map;
using std::string;
using std::ifstream;
using std::stringstream;
using std::stod;

class Airports {
    public:
        struct AirportEle {
            string airport;
            string airportID;
            double latitude;
            double longitude;
            double altitude;
        };
        Airports();
        void readFromFile(const string& filename);
        const vector<AirportEle>& getAirports() const;
        const map<string, unsigned>& getAirportsIdx() const;
    private:
        void print();
        vector<AirportEle> airports_;
        map<string, unsigned> airportsIdx_; // use airport id to search
};
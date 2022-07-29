#include "airports.h"

Airports::Airports() {}

void Airports::readFromFile(const string& filename) {

    // if it's not empty, make it empty
    if (!airports_.empty()) {
        airports_.clear();
    }
    if (!airportsIdx_.empty()) {
        airportsIdx_.clear();
    }

    // assign data into the vector and map
    string line;
    ifstream infile(filename);
    if (infile.is_open()) {
        int i = 0;
        while (getline(infile, line)) {
            vector<string> tool;
            stringstream stream(line);
            while(stream.good()) {
                string part;
                getline(stream, part, ',');
                tool.push_back(part);
            }

            // bad file
            if (tool[4].length() != 5) {
                continue;
            }

            AirportEle tmp;

            // Airport name
            string name = tool[1];
            
            tmp.airport = name.substr(1, name.size() - 2);

            // Airport ID
            string id = tool[4];
            
            tmp.airportID = id.substr(1, 3);

            //latitude
            double lat = stod(tool[6]);
            tmp.latitude = lat;

            //longitude
            double lon = stod(tool[7]);
            tmp.longitude = lon;

            //altitude
            double alt = stod(tool[8]);
            tmp.altitude = alt;

            airports_.push_back(tmp);

            airportsIdx_[id.substr(1, 3)] = i;
            i++;
        }
    }

    std::cout << "The file " << filename << " has been processed." << std::endl;
    print();
}

const vector<Airports::AirportEle>& Airports::getAirports() const {
    return airports_;
}

const map<string, unsigned>& Airports::getAirportsIdx() const {
    return airportsIdx_;
}

void Airports::print() {
    unsigned size = airports_.size();
    std::cout << "We have following airports:" << std::endl;
    for (unsigned i = 0; i < 10 && i < size; ++i) {
        if (i == 0) {
            std::cout << "The 1st airport is " << airports_[i].airport << ", and its 3-letter IATA code (ID) is "
            << airports_[i].airportID << ". Its latitude is " << airports_[i].latitude << " degrees, its longitude is "
            << airports_[i].longitude << " degrees, and its alitutde is " << airports_[i].altitude << " feet."
            << std::endl;
        } else if (i == 1) {
            std::cout << "The 2nd airport is " << airports_[i].airport << ", and its 3-letter IATA code (ID) is "
            << airports_[i].airportID << ". Its latitude is " << airports_[i].latitude << " degrees, its longitude is "
            << airports_[i].longitude << " degrees, and its alitutde is " << airports_[i].altitude << " feet."
            << std::endl;
        } else if (i == 2) {
            std::cout << "The 3rd airport is " << airports_[i].airport << ", and its 3-letter IATA code (ID) is "
            << airports_[i].airportID << ". Its latitude is " << airports_[i].latitude << " degrees, its longitude is "
            << airports_[i].longitude << " degrees, and its alitutde is " << airports_[i].altitude << " feet."
            << std::endl;
        } else {
            std::cout << "The " << i + 1 <<"th airport is " << airports_[i].airport << ", and its 3-letter IATA code (ID) is "
            << airports_[i].airportID << ". Its latitude is " << airports_[i].latitude << " degrees, its longitude is "
            << airports_[i].longitude << " degrees, and its alitutde is " << airports_[i].altitude << " feet."
            << std::endl;
        }
    }
    if (size > 10) {
        std::cout << "And other " << size - 10 << " airports." << std::endl;
    }
}
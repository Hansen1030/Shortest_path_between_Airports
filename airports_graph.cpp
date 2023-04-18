#include "airports_graph.h"

#include <cmath>

using namespace std;

AirportsGraph::AirportsGraph(const Airports& airports) {
    airports_ = &airports;
    numAirports_ = airports.getAirports().size();
    adjMatrix_.resize(numAirports_);
    visited_.resize(numAirports_);
    spm.resize(numAirports_);
    edgeexist_.resize(numAirports_);
    inf = 1000000000;
    for (unsigned i = 0; i < numAirports_; ++i) {
        adjMatrix_[i].resize(numAirports_, 0);
        edgeexist_[i].resize(numAirports_, 0);
        spm[i].resize(numAirports_);
        for (unsigned j = 0; j < numAirports_; ++j) {
            spm[i][j].resize(3);
            if (i == j) {
                spm[i][j][0] = 0;
            } else {
                spm[i][j][0] = inf; 
            }
            spm[i][j][1] = i;
            spm[i][j][2] = j;
        }
        visited_[i] = false;
    }
}

void AirportsGraph::makeMatrix(const string& filename) {
    // if the adjacency matrix is not empty, clear it and initialize it
    if (!adjMatrix_.empty()) {
        adjMatrix_.clear();
        spm.clear();
        adjMatrix_.resize(numAirports_);
        spm.resize(numAirports_);
        for (unsigned i = 0; i < numAirports_; ++i) {
            adjMatrix_[i].resize(numAirports_);
            spm[i].resize(numAirports_);
            for (unsigned j = 0; j < numAirports_; ++j) {
                adjMatrix_[i][j] = 0;
                spm[i][j].resize(3);
                if (i == j) {
                    spm[i][j][0] = 0;
                } else {
                    spm[i][j][0] = inf; 
                }
                spm[i][j][1] = i;
                spm[i][j][2] = j;
            }
        }
    }
    
    const map<string, unsigned>& airportsIdx = airports_->getAirportsIdx();

    // parse the file line by line
    string line;
    ifstream infile(filename);
    if (infile.is_open()) {
        while (getline(infile, line)) {

            // parse the line by comma
            vector<string> parsingStrings;
            stringstream s_stream(line);
            while (s_stream.good()) {
                string substr;
                getline(s_stream, substr, ',');
                parsingStrings.push_back(substr);
            }

            // if not an airport is not registered in Airports class, then discard the route
            if (airportsIdx.find(parsingStrings[2]) == airportsIdx.end()
                    || airportsIdx.find(parsingStrings[4]) == airportsIdx.end()) {
                continue;
            }

            // calculate the distance between two airports
            unsigned idx_1 = airportsIdx.at(parsingStrings[2]);
            unsigned idx_2 = airportsIdx.at(parsingStrings[4]);
            adjMatrix_[idx_1][idx_2] = calculateDistance(idx_1, idx_2);
            spm[idx_1][idx_2][0] = calculateDistance(idx_1, idx_2);
        }
    }

    std::cout << "All routes have been processed." << std::endl;

    // the shortest path matrix will be done here
    // Floyd_Warshall
    for (unsigned i = 0; i < numAirports_; i++) {
        for (unsigned j = 0; j < numAirports_; j++) {
            for (unsigned k = 0; k < numAirports_; k++) {
                if (spm[j][k][0] > spm[j][i][0] + spm[i][k][0]) {
                    double tmp = spm[j][i][0] + spm[i][k][0];
                    spm[j][k] = spm[j][i];
                    spm[j][k][0] = tmp;
                    for (unsigned pos = 2; pos < spm[i][k].size(); pos++) {
                        spm[j][k].push_back(spm[i][k][pos]);
                    }
                }
            }
        }
        std::cout << "Remaining steps are "<< numAirports_ - i << std::endl;
    }

    std::cout << "The file " << filename << " has been processed." << std::endl;
}

double AirportsGraph::calculateDistance(unsigned idx_1, unsigned idx_2) {
    double distance, lat_1, lat_2, long_1, long_2, alt_1, alt_2, r_1, r_2,
            x_1, x_2, y_1, y_2, z_1, z_2;
    const double radius = 6378.1 * 1000; // the earth's radius (in meters)
    const double ftToMeter = 0.3048; // 1 ft = 0.3048 m
    const double pi = 3.14159265358979323846; // the constant pi

    Airports::AirportEle airports1 = airports_->getAirports().at(idx_1);
    Airports::AirportEle airports2 = airports_->getAirports().at(idx_2);

    lat_1 = airports1.latitude * pi / 180;
    long_1 = airports1.longitude * pi / 180;
    alt_1 = airports1.altitude;
    lat_2 = airports2.latitude * pi / 180;
    long_2 = airports2.longitude * pi / 180;
    alt_2 = airports2.altitude;

    // get the current radius
    r_1 = radius + alt_1 * ftToMeter;
    r_2 = radius + alt_2 * ftToMeter;

    // transform spherical coordinates to cartesian coordinates
    double sin1, sin2;
    sin1 = sin(long_1);
    sin2 = sin(long_2);
    x_1 = r_1 * sin1 * cos(lat_1);
    y_1 = r_1 * sin1 * sin(lat_1);
    z_1 = r_1 * cos(long_1);
    x_2 = r_2 * sin2 * cos(lat_2);
    y_2 = r_2 * sin2 * sin(lat_2);
    z_2 = r_2 * cos(long_2);

    // calculate the euclidean distance
    distance = sqrt((x_1 - x_2) * (x_1 - x_2) + (y_1 - y_2) * (y_1 - y_2)
        + (z_1 - z_2) * (z_1 - z_2));
    
    return distance;
}

vector<string> AirportsGraph::findShortestPath(const string& airport1, const string& airport2) {
    vector<string> output;
    unsigned start = airports_->getAirportsIdx().at(airport1);
    unsigned end = airports_->getAirportsIdx().at(airport2);
    if (spm[start][end][0] >= inf) {
        output.push_back("can not reach");
        return output;
    }
    const vector<Airports::AirportEle> &v = airports_->getAirports();
    for (unsigned i = 1; i < spm[start][end].size(); i++) {
        string name = v[spm[start][end][i]].airport;
        output.push_back(name);
    }
    return output;
}

void AirportsGraph::DFS(const string& airport) {
    unsigned vertex;
    vertex = airports_->getAirportsIdx().at(airport);
    dfsTraversal(vertex, visited_);
    for (unsigned i = 0; i < numAirports_; ++i) {
        if (!visited_[i]) {
            dfsTraversal(i, visited_);
        }
    }
}

void AirportsGraph::dfsTraversal(unsigned vertex, vector<bool>& visited_) {
    //set up
    // vector<string> result;

    stack<unsigned> stack_;
    stack_.push(vertex);
    unsigned cur_ = vertex;
    visited_[cur_] = true;


    while (!stack_.empty()) {
        cur_ = stack_.top();
        stack_.pop();
        // result.push_back("a");

        for(unsigned i = 0; i < adjMatrix_[cur_].size(); i++) {
            if (i != cur_ && adjMatrix_[cur_][i] > 0 && (!visited_[i])) {
                    visited_[i] = true;
                    stack_.push(i);
                    edgeexist_[cur_][i] = 1;  //discovery
            } else if (i != cur_ && adjMatrix_[cur_][i] > 0 && (visited_[i])) {
                    edgeexist_[cur_][i] = 2;  // back 
            } else {
                edgeexist_[cur_][i] = 0;  // no  edge
            }
        }
        

    }
    // return result;

}

//centrality based on the number of shortest pathes passed through a node
//say the dfs vector is x
vector <string> AirportsGraph::findCentralAirport(){
    vector<string> result; //Object of return

    //to deal with edge cases:
    if (spm[0].size() <= 0) {
        //cout << "smp wrong size" << endl;
        //result.push_back("Central Airport does not exist"); can add in but size might confuse users
        return result;
    }

    vector<Airports::AirportEle> vector;
    vector = airports_->getAirports();
    //create hashmap with the keys as the result of dfs of the data, 
    //and values initialized as 0 and will later be the number of times that it appeared in the shortest path that is stored in spm.
    map <unsigned, unsigned> y;
    unsigned largest = 0;

    unsigned matrixSize = spm[0].size();
    //cout << "matirx size: " << matrixSize << endl;
    unsigned tmp = 0;
    while (matrixSize > 0) {
        y.insert (std::pair<unsigned, unsigned>(tmp, 0));
        tmp++;
        matrixSize--;
    }

    //cout << "reached here" << endl;

    matrixSize = spm[0].size();
    //cout << "matrix size: " << matrixSize << endl;
    //loop through the upper triangle, increment the value of the key
    //cout << "smp size:" << spm[0].size() << endl;
    for (unsigned i = 0; i < matrixSize - 1; i++){
        //cout << "i: " << i << endl;
        for (unsigned j = i + 1; j < matrixSize; j++) {
            //cout << "j: " << j << endl;
            //cout << "ij size: "<<spm[i][j].size() << endl;
            if (spm[i][j].size() > 1){
                for (unsigned k = 1; k < spm[i][j].size(); k++){
                    //cout << "k: " << k << endl;
                    //cout << "before: " << y[spm[i][j][k]] << endl;
                    y[spm[i][j][k]]++;
                    //check the largest connect possible
                    if (y[spm[i][j][k]] > largest) {largest = y[spm[i][j][k]];}
                    //cout << "after: " << y[spm[i][j][k]] << endl;
                }
            }
        }
    }
    //find the airport that has the largest value in the hashmap, and return the representive index.
  //cout << "sp values: " << endl;
    // for (unsigned i = 0; i < y.size(); i++) {
    //     if (y[i] == largest) {
    //         //cout << y[i] << endl;
    //         largest = y[i];
    //     }
    // }
    //cout << "largest: "<< largest << endl;
    //cout << "result: " << endl;
    for (unsigned i  = 0; i < y.size(); i++) {
        if (y[i] == largest) {
            //cout << vector[i].airport << endl;
            result.push_back(vector[i].airport);
        }
    }
    return result; //returns a vector that contains the airports with the most appearance on all shortest paths
}

const vector<vector<double> >& AirportsGraph::getEdges() const {
    return edgeexist_;
}

const vector<bool>& AirportsGraph::getVertices() const {
    return visited_;
}

const vector<vector<double> >& AirportsGraph::getAdjMatrix() const {
    return adjMatrix_;
}
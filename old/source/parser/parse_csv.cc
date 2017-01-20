#include <cstdio>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <set>
using namespace std;

#include "../base.h"
#include "parse_csv.h"

double DistanceBetweenLatLngs(pair<double, double> ll1,
                              pair<double, double> ll2) {
  double lat1 = ll1.first;
  double lng1 = ll1.second;
  double lat2 = ll2.first;
  double lng2 = ll2.second;
  // Convert to radians...
  const double kDegToRad = 3.1415926535897932384626433 / 180;
  lat1 *= kDegToRad;
  lng1 *= kDegToRad;
  lat2 *= kDegToRad;
  lng2 *= kDegToRad;
  // https://en.wikipedia.org/wiki/Haversine_formula
  const double s1 = sin(0.5 * (lat2 - lat1));
  const double s2 = sin(0.5 * (lng2 - lng1));
  const double d = 2 * asin(sqrt(s1 * s1 + cos(lat1) * cos(lat2) * s2 * s2));
  // https://en.wikipedia.org/wiki/Earth_radius#Mean_radius
  const double kMeanEarthRadiusMeters = 6371008.8;
  return kMeanEarthRadiusMeters * d;
}

vector<Road> Parser::ParseRoadCsv(const string& filename) {
  ifstream is(filename);
  string line;
  vector<Road> roads;
  // Main loop: parse the file, line by line.
  while (getline(is, line)) {
    // Split the line in CSV fields and store them in "fields".
    vector<string> fields;
    {
      string field;
      istringstream is(line);
      while (getline(is, field, ',')) fields.push_back(field);
    }
    // Parse the fields into the "Road" struct.
    int numpoints = stoi(fields[5]);
    CHECK_EQ(2 * numpoints + 6, fields.size()) << line;
    Road road;
    // Parse the polyline...
    for (int i = 0; i < numpoints; ++i) {
      double lat, lng;
      CHECK_EQ(sscanf(fields[2 * i + 6].c_str(), "%lf", &lat), 1);
      CHECK_EQ(sscanf(fields[2 * i + 7].c_str(), "%lf", &lng), 1);
      road.latlngs.push_back(make_pair(lat, lng));
    }
    // Parse the 'one way' and 'speed'.
    int one_way_int = -1;
    sscanf(fields[4].c_str(), "%d", &one_way_int);
    road.one_way = one_way_int == 1;
    sscanf(fields[2].c_str(), "%d", &road.speed_kmh);
    // Store the road.
    roads.push_back(road);
  }

  return roads;
}

RoadNetwork Parser::ConvertRoadDataToGraph(const vector<Road>& roads) {
  RoadNetwork network;

  // First pass: determine which lat/lng are in fact nodes.
  map<pair<double, double>, int> latlng_count;
  set<pair<double, double>> latlng_nodes;
  for (const Road& road : roads) {
    for (const pair<double, double>& ll : road.latlngs) {
      if (++latlng_count[ll] > 1) latlng_nodes.insert(ll);
    }
    latlng_nodes.insert(road.latlngs[0]);
    latlng_nodes.insert(road.latlngs.back());
  }

  // Populate the map between lat/lng and node index.
  int num_nodes = 0;
  for (const pair<double, double>& ll : latlng_nodes) {
    network.latlng_to_node[ll] = num_nodes++;
  }

  // Go over the roads again, and now that we know which
  // lat/lngs are nodes, we can build the arcs.
  for (const Road& road : roads) {
    // Scan the lat/lngs. At any iteration, "i" is the index of a lat/lng node,
    // and 'j' is searching for the next lat/lng node.
    int i = 0;
    while (i < road.latlngs.size() - 1) {
      int j = i + 1;
      while (latlng_nodes.count(road.latlngs[j]) == 0) j++;
      // Now 'j' points to the next lat/lng node. Compute the overall
      // sub-polyline length between i and j.
      double length = 0;
      for (int k = i; k < j; ++k) {
        length += DistanceBetweenLatLngs(road.latlngs[k],
                                         road.latlngs[k + 1]);
      }
      const double duration = length * 3.6 / road.speed_kmh;
      // We're ready to add the arc, with its duration!
      network.graph.AddArc(network.latlng_to_node[road.latlngs[i]],
                           network.latlng_to_node[road.latlngs[j]]);
      network.arc_durations.push_back(duration);
      if (!road.one_way) {
        network.graph.AddArc(network.latlng_to_node[road.latlngs[j]],
                             network.latlng_to_node[road.latlngs[i]]);
        network.arc_durations.push_back(duration);
      }
      // Next iteration: i moves to j, so that we look for the next node.
      i = j;
    }
  }

  return network;
}

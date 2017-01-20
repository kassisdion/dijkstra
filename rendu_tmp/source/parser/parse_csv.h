#ifndef PARSE_CSV_H_
#define PARSE_CSV_H_

#include <map>
#include <string>
#include <utility>
#include <vector>
#include "../graph/graph.h"
using namespace std;

double DistanceBetweenLatLngs(pair<double, double> ll1,
                              pair<double, double> ll2);

struct Road {
  bool one_way;
  int speed_kmh;
  vector<pair<double, double>> latlngs;
};
vector<Road> ParseRoadCsv(const string& filename);

struct RoadNetwork {
  Graph graph;
  vector<double> arc_durations;
  map<pair<double, double>, int> latlng_to_node;
};

class Parser {
 public:
  RoadNetwork ConvertRoadDataToGraph(const vector<Road>& roads);
  vector<Road> ParseRoadCsv(const string& filename);
};

#endif  // PARSE_CSV_H_

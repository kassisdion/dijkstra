#include "graph/graph.h"
#include "base.h"
#include "parser/parse_csv.h"
#include "dijkstra/dijkstra.h"

#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

using namespace std;

int main(int argc, char *argv[], char *envp[]) {

  if (argc < 2) {
    cerr << "No file\n";
  }
  //Parse args
  CHECK_EQ(argc, 2);
  char *fileName = argv[1];

  //Parse file
  Parser parser;
  vector<Road> roadData = parser.ParseRoadCsv(fileName);

  //Build graph
  RoadNetwork roadNetwork = parser.ConvertRoadDataToGraph(roadData);

  printf("%d\n%d\n%.6f\n",
  	 roadNetwork.graph.NumNodes(),
  	 roadNetwork.graph.NumArcs(),
  	 accumulate(roadNetwork.arc_durations.begin(), roadNetwork.arc_durations.end(), 0.0d));

  //Init djikstra
  Dijkstra dijkstra(&roadNetwork.graph, &roadNetwork.arc_durations);
  
  // Get User Input
  string input = "";
  while (getline(cin, input)) {
    pair<double, double> from;
    pair<double, double> to;
    
    size_t pos = 0;
    string token;
    string token2;
    
    // From
    if ((pos = input.find(",")) == string::npos) {
      cout << "INVALID\n";
      continue;
    }
    token = input.substr(0, pos);
    input.erase(0, pos + 1);


    if ((pos = input.find("->")) == string::npos) {
      cout << "INVALID\n";
      continue;
    }
    token2 = input.substr(0, pos);
    input.erase(0, pos + 2);
    
    from = make_pair(atof(token.c_str()), atof(token2.c_str()));

    // TO
    if ((pos = input.find(",")) == string::npos) {
      cout << "INVALID\n";
      continue;
    }
    token = input.substr(0, pos);
    input.erase(0, pos + 1);
    
    token2 = input;
    
    to = make_pair(atof(token.c_str()), atof(token2.c_str()));
    
    //printf("%.6f - %.6f -- %.6f - %.6f\n", from.first, from.second, to.first, to.second);
    
    map<pair<double, double>, int>::iterator it_from;
    map<pair<double, double>, int>::iterator it_to;
    map<pair<double, double>, int>::iterator it_end;
    
    it_from = roadNetwork.latlng_to_node.find(from);
    it_to   = roadNetwork.latlng_to_node.find(to);
    it_end  = roadNetwork.latlng_to_node.end();

    if (it_end == it_from || it_end == it_to) {
      cout << "INVALID\n";
      continue;
    }

    // launch Dijkstra
    vector<int> target;
    target.push_back(it_to->second);
    dijkstra.RunUntilAllTargetsAreReached(it_from->second, target);

    dijkstra.ArcPathFromSourceTo(it_to->second);
    
  }
  
  return 0;
}

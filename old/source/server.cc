#include "graph/graph.h"
#include "base.h"
#include "parser/parse_csv.h"

#include <ctime>
#include <iostream>
#include <fstream>

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

  printf("%s: %d nodes / %d arcs / %.6f seconds \n",
  	 fileName,
  	 roadNetwork.graph.NumNodes(),
  	 roadNetwork.graph.NumArcs(),
  	 accumulate(roadNetwork.arc_durations.begin(), roadNetwork.arc_durations.end(), 0.0f));
  
  return 0;
}

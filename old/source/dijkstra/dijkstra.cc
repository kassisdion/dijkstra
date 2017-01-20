#include <cstdio>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <set>
using namespace std;

#include "dijkstra.h"

Dijkstra::Dijkstra(const Graph* graph, const vector<double>* arc_lengths) {
  this->m_graph = graph;
  this->m_arc_lengths = arc_lengths;
}

const Graph & Dijkstra::GetGraph() const {
  return *(this->m_graph);
}

void Dijkstra::RunUntilAllTargetsAreReached(int source, const vector<int>& targets) {
  fprintf(stderr, "RununtilAllTargetsAreReached\n");
  fprintf(stderr, "source=%d\n", source);
  for(int i = 0; i< targets.size(); i++) {
    fprintf(stderr, "target[%d] = %d", i, targets[i]);
  }


  priority_queue<DijkstraState> pq;

  //Init
  DijkstraState initialState = {source, 0}; 
  pq.push(initialState);
  this->m_distances[source] = 0;
  this->m_parent_arcs[source] = -2;

  while (!pq.empty()) {
    //Get the smaller distance
    DijkstraState state = pq.top();
    vector<int> arcs = this->m_graph->OutgoingArcs(state.node);
    
    for (int i = 0; i < arcs.size(); i++) {
      int arcId = arcs[i];
    }

    pq.pop();
  }  
}

const vector<int>& Dijkstra::ReachedNodes() const {
  fprintf(stderr, "ReachedNodes\n");  
  return this->m_reached_node;
}

const vector<double>& Dijkstra::Distances() const {
  fprintf(stderr, "Distances\n");  
  return this->m_distances;
}

const vector<int>& Dijkstra::ParentArcs() const {
  fprintf(stderr, "ParentArc\n");  
  return this->m_parent_arcs;
}

vector<int> Dijkstra::ArcPathFromSourceTo(int node) const {
  fprintf(stderr, "ArcPathFromSourceTo: node=%d\n", node);
  
  vector<int> path;
  int current;

  try {
    current = this->m_parent_arcs[current];

    fprintf(stderr, "ArcPathFromSourceTo > current=%d\n", current);
    
    path.push_back(current);
  } catch (const std::out_of_range& oor) {
    fprintf(stderr, "ArcPathFromSourceTo > no more parents\n");    
  }

  return path;
}

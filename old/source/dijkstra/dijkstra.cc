#include <cstdio>
#include <limits>
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

  this->m_reached_node = vector<int>(graph->NumNodes());

  this->m_distances = vector<double>(graph->NumArcs());
  for (int i = 0; i < this->m_distances.size(); i++) {
    this->m_distances[i] = numeric_limits<double>::max();
  }
  
  this->m_parent_arcs = vector<int>(graph->NumArcs());
  for (int i = 0; i < this->m_parent_arcs.size(); i++) {
    this->m_parent_arcs[i] = -1;
  }
}

const Graph & Dijkstra::GetGraph() const {
  return *(this->m_graph);
}

void Dijkstra::RunUntilAllTargetsAreReached(int source, const vector<int>& targets) {
  fprintf(stderr, "RununtilAllTargetsAreReached\n");
  fprintf(stderr, "run > source=%d\n", source);
  for(int i = 0; i< targets.size(); i++) {
    fprintf(stderr, "target[%d] = %d\n", i, targets[i]);
  }
  fprintf(stderr, "\n");
  
  //Init
  fprintf(stderr, "run > init\n\n");

  priority_queue<DijkstraState> pq;
  DijkstraState initialState = {source, 0}; 
  pq.push(initialState);
  this->m_distances[source] = 0;
  this->m_parent_arcs[source] = -2;

  fprintf(stderr, "run > starting...\n\n");
  while (!pq.empty()) {
    //Get the smaller distance
    DijkstraState current = pq.top();
    vector<int> arcs = this->m_graph->OutgoingArcs(current.node);

    //Loop over arc
    for (int i = 0; i < arcs.size(); i++) {
      fprintf(stderr, "run > parsing arc:\n");
      
      int arcId = arcs[i];
      int targetNodeId = this->m_graph->Head(arcId);
      double arcLength = this->m_arc_lengths->at(arcId);     
      fprintf(stderr, "run > ardId=%d | targetNodeId=%d | arcLength=%.6f\n", arcId, targetNodeId, arcLength);

      double distanceToTarget = this->m_distances[current.node] + arcLength;
      fprintf(stderr, "run > distance to target= %.6f\n", distanceToTarget);

      if (this->m_distances[targetNodeId] > distanceToTarget) {
	fprintf(stderr, "run > add target\n");
	DijkstraState targetState = {targetNodeId, distanceToTarget};
	pq.push(targetState);
	this->m_distances[targetNodeId] = distanceToTarget;
	this->m_parent_arcs[targetNodeId] = targetNodeId;
      } else {
	fprintf(stderr, "run > don't add target\n");
      }
      
      fprintf(stderr, "\n");
    }

    pq.pop();
  }

  fprintf(stderr, "run > end...\n");
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

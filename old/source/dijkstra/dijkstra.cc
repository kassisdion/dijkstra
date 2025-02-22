#include <cstdio>
#include <limits>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <set>
#include <stdio.h>
#include <stdarg.h>

using namespace std;

#include "dijkstra.h"

void log(const char *format, ...) {

  bool debug = false;

  if (debug) {  
    va_list vargs;
    va_start(vargs, format);
    vprintf(format, vargs);
    va_end(vargs);
  }
}

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
  log("RununtilAllTargetsAreReached\n");
  log("run > source=%d\n", source);
  for(int i = 0; i< targets.size(); i++) {
    log("target[%d] = %d\n", i, targets[i]);
  }
  log("\n");
  
  //Init
  log("run > init\n\n");

  priority_queue<DijkstraState> pq;
  DijkstraState initialState = {source, 0}; 
  pq.push(initialState);
  this->m_distances[source] = 0;
  this->m_parent_arcs[source] = -2;

  log("run > starting...\n\n");
  while (!pq.empty()) {
    //Get the smaller distance
    DijkstraState current = pq.top();
    vector<int> arcs = this->m_graph->OutgoingArcs(current.node);

    //Loop over arc
    for (int i = 0; i < arcs.size(); i++) {
      log("run > parsing arc:\n");
      
      int arcId = arcs[i];
      int targetNodeId = this->m_graph->Head(arcId);
      double arcLength = this->m_arc_lengths->at(arcId);     
      log("run > ardId=%d | targetNodeId=%d | arcLength=%.6f\n", arcId, targetNodeId, arcLength);

      double distanceToTarget = this->m_distances[current.node] + arcLength;
      log("run > distance to target= %.6f\n", distanceToTarget);

      if (this->m_distances[targetNodeId] > distanceToTarget) {
	log("run > add target\n");
	DijkstraState targetState = {targetNodeId, distanceToTarget};
	pq.push(targetState);
	this->m_distances[targetNodeId] = distanceToTarget;
	this->m_parent_arcs[targetNodeId] = arcId;
      } else {
	log("run > don't add target\n");
      }
      
      log("\n");
    }

    pq.pop();
  }

  log("run > end...\n");
}

const vector<int>& Dijkstra::ReachedNodes() const {
  log("ReachedNodes\n");  
  return this->m_reached_node;
}

const vector<double>& Dijkstra::Distances() const {
  log("Distances\n");  
  return this->m_distances;
}

const vector<int>& Dijkstra::ParentArcs() const {
  log("ParentArc\n");  
  return this->m_parent_arcs;
}

vector<int> Dijkstra::ArcPathFromSourceTo(int node) const {
  log("ArcPathFromSourceTo: node=%d\n", node);
  
  vector<int> path;
  int current = node;

  try {
    current = this->m_parent_arcs[current];

    log("ArcPathFromSourceTo > current=%d\n", current);
    
    path.push_back(current);
  } catch (const std::out_of_range& oor) {
    log("ArcPathFromSourceTo > no more parents\n");    
  }

  return path;
}

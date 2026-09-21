#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <Arduino.h>

// =================================================================
// 1. DIRECTION & TURN DEFINITIONS (8-WAY / 45-DEGREE RESOLUTION)
// =================================================================
enum Direction : uint8_t {
  DIR_N  = 0, // 0°   (North / Up)
  DIR_NE = 1, // 45°  (North-East)
  DIR_E  = 2, // 90°  (East / Right)
  DIR_SE = 3, // 135° (South-East)
  DIR_S  = 4, // 180° (South / Down)
  DIR_SW = 5, // 225° (South-West)
  DIR_W  = 6, // 270° (West / Left)
  DIR_NW = 7  // 315° (North-West)
};

enum TurnAction : int8_t {
  ACTION_STRAIGHT  = 0,
  ACTION_RIGHT_45  = 1,
  ACTION_RIGHT_90  = 2,
  ACTION_RIGHT_135 = 3,
  ACTION_UTURN_180 = 4,
  ACTION_LEFT_135  = -3,
  ACTION_LEFT_90   = -2,
  ACTION_LEFT_45   = -1
};

// =================================================================
// 2. GRAPH DATA STRUCTURES
// =================================================================
constexpr int MAX_NODES = 20;
constexpr int MAX_EDGES_PER_NODE = 8;
constexpr int INF = 9999;

struct Edge {
  int targetNode;
  int weight;
  Direction heading; // Heading required from source node to target
};

struct Node {
  int edgeCount;
  Edge edges[MAX_EDGES_PER_NODE];
};

struct GraphContext {
  Node nodes[MAX_NODES];
  int currentNode;
  Direction currentHeading;
};

// Global pathfinder instance
static GraphContext g_graph;

// =================================================================
// 3. GRAPH BUILDER & MATH UTILITIES
// =================================================================

// Clears all nodes in graph
inline void clearGraph() {
  for (int i = 0; i < MAX_NODES; i++) {
    g_graph.nodes[i].edgeCount = 0;
  }
}

// Adds bidirectional edge between u and v
inline void addEdge(int u, int v, int weight, Direction headingUtoV) {
  if (u >= MAX_NODES || v >= MAX_NODES) return;

  // Edge u -> v
  Node &nu = g_graph.nodes[u];
  if (nu.edgeCount < MAX_EDGES_PER_NODE) {
    nu.edges[nu.edgeCount++] = { v, weight, headingUtoV };
  }

  // Edge v -> u (Opposite direction = (heading + 4) % 8)
  Node &nv = g_graph.nodes[v];
  Direction headingVtoU = static_cast<Direction>((headingUtoV + 4) % 8);
  if (nv.edgeCount < MAX_EDGES_PER_NODE) {
    nv.edges[nv.edgeCount++] = { u, weight, headingVtoU };
  }
}

// Retrieves heading required to travel between two directly adjacent nodes
inline Direction getHeadingTo(int fromNode, int toNode) {
  const Node &n = g_graph.nodes[fromNode];
  for (int i = 0; i < n.edgeCount; i++) {
    if (n.edges[i].targetNode == toNode) {
      return n.edges[i].heading;
    }
  }
  return DIR_N; // Fallback
}

// Calculates required turn action from current heading to target heading
inline TurnAction calculateTurn(Direction fromHeading, Direction toHeading) {
  int diff = ((int)toHeading - (int)fromHeading + 8) % 8;
  switch (diff) {
    case 0: return ACTION_STRAIGHT;
    case 1: return ACTION_RIGHT_45;
    case 2: return ACTION_RIGHT_90;
    case 3: return ACTION_RIGHT_135;
    case 4: return ACTION_UTURN_180;
    case 5: return ACTION_LEFT_135;
    case 6: return ACTION_LEFT_90;
    case 7: return ACTION_LEFT_45;
    default: return ACTION_STRAIGHT;
  }
}

// =================================================================
// 4. DIJKSTRA SHORTEST PATH ALGORITHM
// =================================================================
inline bool dijkstra(int startNode, int goalNode, int pathOut[], int &pathLength) {
  int dist[MAX_NODES];
  int parent[MAX_NODES];
  bool visited[MAX_NODES];

  for (int i = 0; i < MAX_NODES; i++) {
    dist[i] = INF;
    visited[i] = false;
    parent[i] = -1;
  }

  dist[startNode] = 0;

  for (int i = 0; i < MAX_NODES; i++) {
    int u = -1;
    int min_d = INF;

    for (int j = 0; j < MAX_NODES; j++) {
      if (!visited[j] && dist[j] < min_d) {
        min_d = dist[j];
        u = j;
      }
    }

    if (u == -1 || u == goalNode) break;
    visited[u] = true;

    const Node &nodeU = g_graph.nodes[u];
    for (int e = 0; e < nodeU.edgeCount; e++) {
      int v = nodeU.edges[e].targetNode;
      int weight = nodeU.edges[e].weight;
      if (!visited[v] && dist[u] + weight < dist[v]) {
        dist[v] = dist[u] + weight;
        parent[v] = u;
      }
    }
  }

  if (dist[goalNode] == INF) {
    pathLength = 0;
    return false; // No path found
  }

  // Reconstruct path
  int temp[MAX_NODES];
  int count = 0;
  int curr = goalNode;

  while (curr != -1) {
    temp[count++] = curr;
    curr = parent[curr];
  }

  pathLength = count;
  for (int i = 0; i < count; i++) {
    pathOut[i] = temp[count - 1 - i];
  }
  return true;
}

// =================================================================
// 5. ARENA MAP TOPOLOGY (Arena from Image)
// =================================================================
/*
   NODES MAPPED:
   [0] Station Box 1 (Top Left)
   [1] Station Box 2 (Upper Mid Left)
   [2] Station Box 3 (Lower Mid Left)
   [3] Station Box 4 (Bottom Left)
   [4..7] Outer Left Spine Junctions
   [8..10] Inner Left Spine Junctions
   [11] Central 8-Way Star Hub
   [12] Top Mid Box
   [13] Bottom Mid Box
   [14] Top Right Box
   [15] Bottom Right Box
   [16] Right Middle Junction
*/
inline void initArenaMap() {
  clearGraph();

  // Left Boxes -> Outer Left Spine
  addEdge(0, 4, 10, DIR_E);
  addEdge(1, 5, 10, DIR_E);
  addEdge(2, 6, 10, DIR_E);
  addEdge(3, 7, 10, DIR_E);

  // Outer Left Spine (Vertical)
  addEdge(4, 5, 10, DIR_S);
  addEdge(5, 6, 10, DIR_S);
  addEdge(6, 7, 10, DIR_S);

  // Cross links to Inner Left Spine
  addEdge(4, 8, 15, DIR_E);
  addEdge(6, 9, 15, DIR_E);
  addEdge(7, 10, 15, DIR_E);

  // Inner Left Spine (Vertical)
  addEdge(8, 9, 15, DIR_S);
  addEdge(9, 10, 15, DIR_S);

  // 8-WAY CENTRAL STAR HUB (Node 11)
  addEdge(9,  11, 20, DIR_E);  // Straight West to Center
  addEdge(8,  11, 25, DIR_SE); // Diagonal Top-Left to Center
  addEdge(10, 11, 25, DIR_NE); // Diagonal Bottom-Left to Center
  addEdge(12, 11, 15, DIR_S);  // Top Box down to Center
  addEdge(13, 11, 15, DIR_N);  // Bottom Box up to Center

  // Right Side Stations & Spine
  addEdge(11, 14, 25, DIR_NE); // Diagonal Center to Top-Right Box
  addEdge(11, 15, 25, DIR_SE); // Diagonal Center to Bottom-Right Box
  addEdge(11, 16, 20, DIR_E);  // Center to Right Spine Junction
  addEdge(14, 16, 15, DIR_S);
  addEdge(15, 16, 15, DIR_N);
}

#endif // PATHFINDER_H
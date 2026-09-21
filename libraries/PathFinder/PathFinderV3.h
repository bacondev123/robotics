#ifndef PATHFINDER_RAW_ENGINE_H
#define PATHFINDER_RAW_ENGINE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// =================================================================
// 1. DIRECTION & NAVIGATION ENUMS
// =================================================================
enum Direction : uint8_t {
  DIR_N       = 0, // 0°   (North / +Y)
  DIR_NE      = 1, // 45°  (North-East)
  DIR_E       = 2, // 90°  (East / +X)
  DIR_SE      = 3, // 135° (South-East)
  DIR_S       = 4, // 180° (South / -Y)
  DIR_SW      = 5, // 225° (South-West)
  DIR_W       = 6, // 270° (West / -X)
  DIR_NW      = 7, // 315° (North-West)
  DIR_INVALID = 8
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
// 2. DATA STRUCTURES
// =================================================================
struct Edge {
  uint8_t targetNode;
  uint16_t weight;      // Distance / travel cost in any unit (mm, cm, ticks)
  Direction heading;
  bool enabled;
};

template <uint8_t MAX_EDGES>
struct Node {
  uint8_t edgeCount;
  bool enabled;
  Edge edges[MAX_EDGES];
};

struct PathStep {
  uint8_t fromNode;
  uint8_t toNode;
  Direction targetHeading;
  TurnAction turn;
  int16_t turnAngleDeg; // -180 to +180 deg
  uint16_t distance;    // Weight/distance of this specific segment
};

template <uint8_t MAX_NODES>
struct NavigationPlan {
  uint8_t nodeCount;
  uint8_t nodes[MAX_NODES];
  uint8_t stepCount;
  PathStep steps[MAX_NODES > 0 ? (MAX_NODES - 1) : 1];
  uint32_t totalDistance;
  bool isValid;
};

// =================================================================
// 3. GENERIC RAW PATHFINDER ENGINE
// =================================================================
template <uint8_t MAX_NODES = 32, uint8_t MAX_EDGES = 8>
class PathfinderEngine {
private:
  static constexpr uint16_t INF_DIST = 0xFFFF;
  static constexpr uint8_t  NO_PARENT = 0xFF;

  Node<MAX_EDGES> m_nodes[MAX_NODES];

public:
  using PlanType = NavigationPlan<MAX_NODES>;

  PathfinderEngine() {
    clear();
  }

  // Clear all graph data
  void clear() {
    for (uint8_t i = 0; i < MAX_NODES; ++i) {
      m_nodes[i].edgeCount = 0;
      m_nodes[i].enabled = true;
    }
  }

  // Connects two arbitrary nodes with user-defined distance and heading
  bool addEdge(uint8_t u, uint8_t v, uint16_t weight, Direction headingUtoV, bool bidirectional = true) {
    if (u >= MAX_NODES || v >= MAX_NODES || u == v) return false;

    if (!insertEdgeInternal(u, v, weight, headingUtoV)) return false;

    if (bidirectional) {
      Direction headingVtoU = static_cast<Direction>((headingUtoV + 4) % 8);
      if (!insertEdgeInternal(v, u, weight, headingVtoU)) return false;
    }
    return true;
  }

  // Obstacle avoidance: enable/disable edges or nodes dynamically at runtime
  bool setEdgeEnabled(uint8_t u, uint8_t v, bool enabled) {
    if (u >= MAX_NODES || v >= MAX_NODES) return false;
    for (uint8_t i = 0; i < m_nodes[u].edgeCount; ++i) {
      if (m_nodes[u].edges[i].targetNode == v) {
        m_nodes[u].edges[i].enabled = enabled;
        return true;
      }
    }
    return false;
  }

  void setNodeEnabled(uint8_t node, bool enabled) {
    if (node < MAX_NODES) {
      m_nodes[node].enabled = enabled;
    }
  }

  Direction getHeading(uint8_t fromNode, uint8_t toNode) const {
    if (fromNode >= MAX_NODES || toNode >= MAX_NODES) return DIR_INVALID;
    const auto &n = m_nodes[fromNode];
    for (uint8_t i = 0; i < n.edgeCount; ++i) {
      if (n.edges[i].targetNode == toNode) {
        return n.edges[i].heading;
      }
    }
    return DIR_INVALID;
  }

  static TurnAction calculateTurn(Direction fromHeading, Direction toHeading) {
    if (fromHeading >= DIR_INVALID || toHeading >= DIR_INVALID) return ACTION_STRAIGHT;
    int8_t diff = ((int8_t)toHeading - (int8_t)fromHeading + 8) % 8;
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

  static int16_t turnActionToDegrees(TurnAction turn) {
    switch (turn) {
      case ACTION_STRAIGHT:  return 0;
      case ACTION_RIGHT_45:  return 45;
      case ACTION_RIGHT_90:  return 90;
      case ACTION_RIGHT_135: return 135;
      case ACTION_UTURN_180: return 180;
      case ACTION_LEFT_135:  return -135;
      case ACTION_LEFT_90:   return -90;
      case ACTION_LEFT_45:   return -45;
      default: return 0;
    }
  }

  // Core Dijkstra Path Planning
  bool planPath(uint8_t startNode, uint8_t goalNode, Direction startHeading, PlanType &planOut) {
    planOut.isValid = false;
    planOut.nodeCount = 0;
    planOut.stepCount = 0;
    planOut.totalDistance = 0;

    if (startNode >= MAX_NODES || goalNode >= MAX_NODES) return false;
    if (!m_nodes[startNode].enabled || !m_nodes[goalNode].enabled) return false;

    if (startNode == goalNode) {
      planOut.nodes[0] = startNode;
      planOut.nodeCount = 1;
      planOut.isValid = true;
      return true;
    }

    uint16_t dist[MAX_NODES];
    uint8_t  parent[MAX_NODES];
    bool     visited[MAX_NODES];

    for (uint8_t i = 0; i < MAX_NODES; ++i) {
      dist[i] = INF_DIST;
      parent[i] = NO_PARENT;
      visited[i] = false;
    }

    dist[startNode] = 0;

    for (uint8_t count = 0; count < MAX_NODES; ++count) {
      uint8_t u = NO_PARENT;
      uint16_t min_d = INF_DIST;

      for (uint8_t i = 0; i < MAX_NODES; ++i) {
        if (!visited[i] && m_nodes[i].enabled && dist[i] < min_d) {
          min_d = dist[i];
          u = i;
        }
      }

      if (u == NO_PARENT || u == goalNode) break;
      visited[u] = true;

      const auto &nodeU = m_nodes[u];
      for (uint8_t e = 0; e < nodeU.edgeCount; ++e) {
        const Edge &edge = nodeU.edges[e];
        uint8_t v = edge.targetNode;

        if (edge.enabled && !visited[v] && m_nodes[v].enabled) {
          uint16_t newDist = dist[u] + edge.weight;
          if (newDist < dist[v]) {
            dist[v] = newDist;
            parent[v] = u;
          }
        }
      }
    }

    if (dist[goalNode] == INF_DIST) return false;

    // Backtrack path
    uint8_t tempPath[MAX_NODES];
    uint8_t pathLen = 0;
    uint8_t curr = goalNode;

    while (curr != NO_PARENT && pathLen < MAX_NODES) {
      tempPath[pathLen++] = curr;
      curr = parent[curr];
    }

    planOut.nodeCount = pathLen;
    for (uint8_t i = 0; i < pathLen; ++i) {
      planOut.nodes[i] = tempPath[pathLen - 1 - i];
    }
    planOut.totalDistance = dist[goalNode];
    planOut.stepCount = (pathLen > 0) ? (pathLen - 1) : 0;

    Direction currentHeading = startHeading;
    for (uint8_t i = 0; i < planOut.stepCount; ++i) {
      uint8_t from = planOut.nodes[i];
      uint8_t to   = planOut.nodes[i + 1];

      Direction nextHeading = getHeading(from, to);
      TurnAction turn = calculateTurn(currentHeading, nextHeading);

      planOut.steps[i].fromNode = from;
      planOut.steps[i].toNode = to;
      planOut.steps[i].targetHeading = nextHeading;
      planOut.steps[i].turn = turn;
      planOut.steps[i].turnAngleDeg = turnActionToDegrees(turn);
      planOut.steps[i].distance = getEdgeWeight(from, to);

      currentHeading = nextHeading;
    }

    planOut.isValid = true;
    return true;
  }

private:
  bool insertEdgeInternal(uint8_t u, uint8_t v, uint16_t weight, Direction heading) {
    auto &node = m_nodes[u];
    for (uint8_t i = 0; i < node.edgeCount; ++i) {
      if (node.edges[i].targetNode == v) {
        node.edges[i].weight = weight;
        node.edges[i].heading = heading;
        node.edges[i].enabled = true;
        return true;
      }
    }
    if (node.edgeCount < MAX_EDGES) {
      node.edges[node.edgeCount++] = { v, weight, heading, true };
      return true;
    }
    return false;
  }

  uint16_t getEdgeWeight(uint8_t u, uint8_t v) const {
    const auto &node = m_nodes[u];
    for (uint8_t i = 0; i < node.edgeCount; ++i) {
      if (node.edges[i].targetNode == v) return node.edges[i].weight;
    }
    return 0;
  }
};

// Default alias: 32 max nodes, 8 max edges per node
using Pathfinder = PathfinderEngine<32, 8>;

#endif // PATHFINDER_RAW_ENGINE_H
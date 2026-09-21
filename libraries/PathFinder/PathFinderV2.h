#ifndef PATHFINDER_ENGINE_H
#define PATHFINDER_ENGINE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// =================================================================
// 1. CONFIGURATION & LIMITS
// =================================================================
#ifndef PATHFINDER_MAX_NODES
#define PATHFINDER_MAX_NODES 24
#endif

#ifndef PATHFINDER_MAX_EDGES_PER_NODE
#define PATHFINDER_MAX_EDGES_PER_NODE 8
#endif

constexpr uint16_t PATH_INF = 0xFFFF; // Max weight / unreachable sentinel
constexpr uint8_t  INVALID_NODE = 0xFF;

// =================================================================
// 2. DIRECTION & NAVIGATION ENUMS
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
// 3. NAVIGATION DATA STRUCTURES
// =================================================================

struct Edge {
  uint8_t targetNode;
  uint16_t weight;
  Direction heading;
  bool enabled; // Allows dynamic blocking (e.g. obstacles detected)
};

struct Node {
  uint8_t edgeCount;
  bool enabled;
  Edge edges[PATHFINDER_MAX_EDGES_PER_NODE];
};

// Step-by-step navigation instructions for the robot's motion controller
struct PathStep {
  uint8_t fromNode;
  uint8_t toNode;
  Direction targetHeading;
  TurnAction turn;
  int16_t turnAngleDeg; // Relative turn angle in degrees (-180 to +180)
  uint16_t distance;    // Weight/Distance of this segment
};

// Full navigation route
struct NavigationPlan {
  uint8_t nodeCount;
  uint8_t nodes[PATHFINDER_MAX_NODES];
  uint8_t stepCount;
  PathStep steps[PATHFINDER_MAX_NODES - 1];
  uint32_t totalDistance;
  bool isValid;
};

// =================================================================
// 4. PATHFINDER ENGINE CLASS
// =================================================================
class Pathfinder {
private:
  Node m_nodes[PATHFINDER_MAX_NODES];

public:
  Pathfinder() {
    clear();
  }

  // Resets the entire graph
  void clear() {
    for (uint8_t i = 0; i < PATHFINDER_MAX_NODES; ++i) {
      m_nodes[i].edgeCount = 0;
      m_nodes[i].enabled = true;
    }
  }

  // Adds an edge (directed or bidirectional)
  bool addEdge(uint8_t u, uint8_t v, uint16_t weight, Direction headingUtoV, bool bidirectional = true) {
    if (u >= PATHFINDER_MAX_NODES || v >= PATHFINDER_MAX_NODES || u == v) return false;

    // Add Edge u -> v
    if (!insertEdgeInternal(u, v, weight, headingUtoV)) return false;

    // Add Edge v -> u if bidirectional
    if (bidirectional) {
      Direction headingVtoU = static_cast<Direction>((headingUtoV + 4) % 8);
      if (!insertEdgeInternal(v, u, weight, headingVtoU)) return false;
    }
    return true;
  }

  // Temporarily enable/disable an edge (Obstacle avoidance)
  bool setEdgeEnabled(uint8_t u, uint8_t v, bool enabled) {
    if (u >= PATHFINDER_MAX_NODES || v >= PATHFINDER_MAX_NODES) return false;
    bool found = false;

    for (uint8_t i = 0; i < m_nodes[u].edgeCount; ++i) {
      if (m_nodes[u].edges[i].targetNode == v) {
        m_nodes[u].edges[i].enabled = enabled;
        found = true;
        break;
      }
    }
    return found;
  }

  // Temporarily enable/disable a whole node (e.g. blocked junction)
  void setNodeEnabled(uint8_t node, bool enabled) {
    if (node < PATHFINDER_MAX_NODES) {
      m_nodes[node].enabled = enabled;
    }
  }

  // Get heading required to move directly from u to v
  Direction getHeading(uint8_t fromNode, uint8_t toNode) const {
    if (fromNode >= PATHFINDER_MAX_NODES || toNode >= PATHFINDER_MAX_NODES) return DIR_INVALID;
    
    const Node &n = m_nodes[fromNode];
    for (uint8_t i = 0; i < n.edgeCount; ++i) {
      if (n.edges[i].targetNode == toNode) {
        return n.edges[i].heading;
      }
    }
    return DIR_INVALID;
  }

  // Calculate relative turn action between two absolute directions
  static TurnAction calculateTurn(Direction fromHeading, Direction toHeading) {
    if (fromHeading >= DIR_INVALID || toHeading >= DIR_INVALID) return ACTION_STRAIGHT;

    int8_t diff = static_cast<int8_t>(toHeading) - static_cast<int8_t>(fromHeading);
    diff = (diff + 8) % 8;

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

  // Convert turn action into signed degrees (-180° to +180°) for PID/motion control
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

  // Convert absolute direction into compass degrees (0° North, 90° East, ...)
  static int16_t directionToDegrees(Direction dir) {
    if (dir >= DIR_INVALID) return 0;
    return static_cast<int16_t>(dir) * 45;
  }

  // =================================================================
  // 5. CORE PATHFINDING (Dijkstra)
  // =================================================================

  // High-level navigation planner: outputs complete route with turn angles & distances
  bool planPath(uint8_t startNode, uint8_t goalNode, Direction startHeading, NavigationPlan &planOut) {
    planOut.isValid = false;
    planOut.nodeCount = 0;
    planOut.stepCount = 0;
    planOut.totalDistance = 0;

    if (startNode >= PATHFINDER_MAX_NODES || goalNode >= PATHFINDER_MAX_NODES) return false;
    if (!m_nodes[startNode].enabled || !m_nodes[goalNode].enabled) return false;

    // Fast return if start == goal
    if (startNode == goalNode) {
      planOut.nodes[0] = startNode;
      planOut.nodeCount = 1;
      planOut.isValid = true;
      return true;
    }

    uint16_t dist[PATHFINDER_MAX_NODES];
    uint8_t  parent[PATHFINDER_MAX_NODES];
    bool     visited[PATHFINDER_MAX_NODES];

    for (uint8_t i = 0; i < PATHFINDER_MAX_NODES; ++i) {
      dist[i] = PATH_INF;
      parent[i] = INVALID_NODE;
      visited[i] = false;
    }

    dist[startNode] = 0;

    // Dijkstra relaxation loop
    for (uint8_t count = 0; count < PATHFINDER_MAX_NODES; ++count) {
      uint8_t u = INVALID_NODE;
      uint16_t min_d = PATH_INF;

      for (uint8_t i = 0; i < PATHFINDER_MAX_NODES; ++i) {
        if (!visited[i] && m_nodes[i].enabled && dist[i] < min_d) {
          min_d = dist[i];
          u = i;
        }
      }

      if (u == INVALID_NODE || u == goalNode) break;
      visited[u] = true;

      const Node &nodeU = m_nodes[u];
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

    if (dist[goalNode] == PATH_INF) return false; // Unreachable

    // Trace path back from goal
    uint8_t tempPath[PATHFINDER_MAX_NODES];
    uint8_t pathLength = 0;
    uint8_t curr = goalNode;

    while (curr != INVALID_NODE && pathLength < PATHFINDER_MAX_NODES) {
      tempPath[pathLength++] = curr;
      curr = parent[curr];
    }

    // Populate plan nodes in forward order
    planOut.nodeCount = pathLength;
    for (uint8_t i = 0; i < pathLength; ++i) {
      planOut.nodes[i] = tempPath[pathLength - 1 - i];
    }
    planOut.totalDistance = dist[goalNode];

    // Generate motion instructions (steps)
    planOut.stepCount = (pathLength > 0) ? (pathLength - 1) : 0;
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

      currentHeading = nextHeading; // Advance robot's expected orientation
    }

    planOut.isValid = true;
    return true;
  }

  // Simple array overload (legacy/minimal use)
  bool findPath(uint8_t startNode, uint8_t goalNode, uint8_t pathOut[], uint8_t &pathLength) {
    NavigationPlan plan;
    if (!planPath(startNode, goalNode, DIR_N, plan)) {
      pathLength = 0;
      return false;
    }
    pathLength = plan.nodeCount;
    for (uint8_t i = 0; i < plan.nodeCount; ++i) {
      pathOut[i] = plan.nodes[i];
    }
    return true;
  }

private:
  bool insertEdgeInternal(uint8_t u, uint8_t v, uint16_t weight, Direction heading) {
    Node &node = m_nodes[u];
    
    // Check if edge already exists; update it if so
    for (uint8_t i = 0; i < node.edgeCount; ++i) {
      if (node.edges[i].targetNode == v) {
        node.edges[i].weight = weight;
        node.edges[i].heading = heading;
        node.edges[i].enabled = true;
        return true;
      }
    }

    // Add new edge if capacity allows
    if (node.edgeCount < PATHFINDER_MAX_EDGES_PER_NODE) {
      node.edges[node.edgeCount++] = { v, weight, heading, true };
      return true;
    }
    return false; // Exceeded maximum edges per node
  }

  uint16_t getEdgeWeight(uint8_t u, uint8_t v) const {
    const Node &node = m_nodes[u];
    for (uint8_t i = 0; i < node.edgeCount; ++i) {
      if (node.edges[i].targetNode == v) {
        return node.edges[i].weight;
      }
    }
    return 0;
  }
};

// =================================================================
// 6. ARENA TOPOLOGY LOADER
// =================================================================
inline void loadStandardArena(Pathfinder &engine) {
  engine.clear();

  // Left Boxes -> Outer Left Spine
  engine.addEdge(0, 4, 10, DIR_E);
  engine.addEdge(1, 5, 10, DIR_E);
  engine.addEdge(2, 6, 10, DIR_E);
  engine.addEdge(3, 7, 10, DIR_E);

  // Outer Left Spine (Vertical)
  engine.addEdge(4, 5, 10, DIR_S);
  engine.addEdge(5, 6, 10, DIR_S);
  engine.addEdge(6, 7, 10, DIR_S);

  // Cross links to Inner Left Spine
  engine.addEdge(4, 8, 15, DIR_E);
  engine.addEdge(6, 9, 15, DIR_E);
  engine.addEdge(7, 10, 15, DIR_E);

  // Inner Left Spine (Vertical)
  engine.addEdge(8, 9, 15, DIR_S);
  engine.addEdge(9, 10, 15, DIR_S);

  // Central 8-Way Star Hub (Node 11)
  engine.addEdge(9,  11, 20, DIR_E);
  engine.addEdge(8,  11, 25, DIR_SE);
  engine.addEdge(10, 11, 25, DIR_NE);
  engine.addEdge(12, 11, 15, DIR_S);
  engine.addEdge(13, 11, 15, DIR_N);

  // Right Side Stations & Spine
  engine.addEdge(11, 14, 25, DIR_NE);
  engine.addEdge(11, 15, 25, DIR_SE);
  engine.addEdge(11, 16, 20, DIR_E);
  engine.addEdge(14, 16, 15, DIR_S);
  engine.addEdge(15, 16, 15, DIR_N);
}

#endif // PATHFINDER_ENGINE_H
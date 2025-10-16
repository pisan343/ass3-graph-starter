/**
 * Testing BST - Binary Search Tree functions
 *
 * @author Yusuf Pisan
 * @date 19 Oct 2019
 */

#include "graph.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

/**
 * Trying to avoid global variables,
 * by creating a singleton class with our visitor functions
 * stringstream SS contains the output from visitor
 */
class Tester {
public:
  Tester() = delete;
  // insert output to SStream rather than cout, so we can test it
  static stringstream SStream;
  static string getSs() { return SStream.str(); }
  static void resetSs() { SStream.str(string()); }
  // visitor function used for DFS and BFS
  static void labelVisitor(const string &Label) { SStream << Label; }
  // visitor function used for edges for minimum spanning tree
  static void edgeVisitor(const string &FromVertex, const string &ToVertex,
                          int Weight) {
    SStream << "[" << FromVertex << ToVertex << " " << Weight << "]";
  }
};

// initialize the static variable
// NOLINTNEXTLINE
stringstream Tester::SStream;

// convert a map to a string so we can compare it
template <typename K, typename L>
static string map2string(const map<K, L> &Map2String) {
  stringstream Out;
  for (auto &Pair : Map2String) {
    Out << "[" << Pair.first << ":" << Pair.second << "]";
  }
  return Out.str();
}

void testGraphBasic() {
  Graph Graph;
  assert(Graph.add("a") && "add vertex a");
  assert(Graph.add("b") && "add vertex b");
  assert(Graph.add("c") && "add vertex c");
  assert(Graph.add("d") && "add vertex d");
  assert(Graph.add("e") && "add vertex e");
  assert(!Graph.add("b") && "b added twice");
  assert(Graph.connect("a", "b", 10) && "connect a b");
  assert(!Graph.connect("a", "b", 50) && "duplicate connect a b");
  assert(!Graph.connect("a", "a", 1) && "connect a to itself");
  Graph.connect("a", "d", 40);
  Graph.connect("a", "c", 20);
  assert((Graph.verticesSize() == 5) && "graph number of vertices");
  assert((Graph.edgesSize() == 3) && "graph number of edges");
  assert((Graph.neighborsSize("a") == 3) && "vertex number of edges");
  assert((Graph.neighborsSize("c") == 0) && "no outgoing edges c");
  assert((Graph.neighborsSize("xxx") == -1) && "no edges for xxx");
  assert(!Graph.contains("xxx") && "xxx not in graph");
  assert(Graph.contains("a") && "a in graph");

  // check that they are sorted based on edge end label
  assert(Graph.getEdgesAsString("a") == "b(10),c(20),d(40)");
  // disconnect non-existent edge/vertex
  assert(!Graph.disconnect("a", "e") && "disconnecting non-existent vertex");
  assert((Graph.edgesSize() == 3) && "disconnected nonexisting");
  assert(Graph.disconnect("a", "c") && "a-c disconnect");
  assert((Graph.edgesSize() == 2) && "number of edges after disconnect");
  assert((Graph.neighborsSize("a") == 2) && "a has 2 edges");
  assert(Graph.getEdgesAsString("a") == "b(10),d(40)" &&
         "removing middle edge");
}

void testGraph0DFS() {
  cout << "testGraph0DFS" << endl;
  Graph Graph;
  if (!Graph.readFile("graph0.txt")) {
    return;
  }
  assert(Graph.contains("A") && "a in graph");
  assert(Graph.contains("B") && "b in graph");
  assert(Graph.contains("C") && "c in graph");
  assert(Graph.getEdgesAsString("A") == "B(1),C(8)");
  assert(Graph.getEdgesAsString("B") == "C(3)");
  assert(Graph.getEdgesAsString("C").empty());

  Tester::resetSs();
  Graph.dfs("A", Tester::labelVisitor);
  assert(Tester::getSs() == "ABC" && "starting from A");

  Tester::resetSs();
  Graph.dfs("B", Tester::labelVisitor);
  assert(Tester::getSs() == "BC" && "starting from B");

  Tester::resetSs();
  Graph.dfs("C", Tester::labelVisitor);
  assert(Tester::getSs() == "C" && "starting from C");

  Tester::resetSs();
  Graph.dfs("X", Tester::labelVisitor);
  assert(Tester::getSs().empty() && "starting from X");
}

void testGraph0BFS() {
  cout << "testGraph0BFS" << endl;
  Graph Graph;
  if (!Graph.readFile("graph0.txt")) {
    return;
  }
  Tester::resetSs();
  Graph.bfs("A", Tester::labelVisitor);
  assert(Tester::getSs() == "ABC" && "starting from A");

  Tester::resetSs();
  Graph.dfs("B", Tester::labelVisitor);
  assert(Tester::getSs() == "BC" && "starting from B");

  Tester::resetSs();
  Graph.dfs("C", Tester::labelVisitor);
  assert(Tester::getSs() == "C" && "starting from C");

  Tester::resetSs();
  Graph.dfs("X", Tester::labelVisitor);
  assert(Tester::getSs().empty() && "starting from X");
}

void testGraph0Dijkstra() {
  cout << "testGraph0Dijkstra" << endl;
  Graph Graph;
  if (!Graph.readFile("graph0.txt")) {
    return;
  }
  map<string, int> Weights;
  map<string, string> Previous;
  tie(Weights, Previous) = Graph.dijkstra("A");
  // cout << "Dijkstra(A) weights is " << map2string(weights) << endl;
  assert(map2string(Weights) == "[B:1][C:4]" && "Dijkstra(A) weights");
  // cout << "Dijkstra(A) previous is " << map2string(previous) << endl;
  assert(map2string(Previous) == "[B:A][C:B]" && "Dijkstra(A) previous");

  tie(Weights, Previous) = Graph.dijkstra("B");
  assert(map2string(Weights) == "[C:3]" && "Dijkstra(B) weights");
  assert(map2string(Previous) == "[C:B]" && "Dijkstra(B) previous");

  tie(Weights, Previous) = Graph.dijkstra("X");
  assert(map2string(Weights).empty() && "Dijkstra(X) weights");
  assert(map2string(Previous).empty() && "Dijkstra(X) previous");
}

void testGraph0NotDirected() {
  cout << "testGraph0NotDirected" << endl;
  bool IsDirectional = false;
  Graph Graph(IsDirectional);
  if (!Graph.readFile("graph0.txt")) {
    return;
  }

  Tester::resetSs();
  Graph.bfs("A", Tester::labelVisitor);
  assert(Tester::getSs() == "ABC" && "starting from A");

  Tester::resetSs();
  Graph.dfs("B", Tester::labelVisitor);
  assert(Tester::getSs() == "BAC" && "starting from B");

  Tester::resetSs();
  Graph.dfs("C", Tester::labelVisitor);
  assert(Tester::getSs() == "CAB" && "starting from C");

  Tester::resetSs();
  Graph.dfs("X", Tester::labelVisitor);
  assert(Tester::getSs().empty() && "starting from X");

  map<string, int> Weights;
  map<string, string> Previous;
  tie(Weights, Previous) = Graph.dijkstra("A");
  // cout << "Dijkstra(A) weights is " << map2string(weights) << endl;
  assert(map2string(Weights) == "[B:1][C:4]" && "Dijkstra(A) weights");
  // cout << "Dijkstra(A) previous is " << map2string(previous) << endl;
  assert(map2string(Previous) == "[B:A][C:B]" && "Dijkstra(A) previous");

  tie(Weights, Previous) = Graph.dijkstra("B");
  assert(map2string(Weights) == "[A:1][C:3]" && "Dijkstra(B) weights");
  assert(map2string(Previous) == "[A:B][C:B]" && "Dijkstra(B) previous");

  tie(Weights, Previous) = Graph.dijkstra("X");
  assert(map2string(Weights).empty() && "Dijkstra(C) weights");
  assert(map2string(Previous).empty() && "Dijkstra(C) previous");

  Tester::resetSs();
  int MstLength = Graph.mst("A", Tester::edgeVisitor);
  assert(MstLength == 4 && "mst A is 4");
  assert(Tester::getSs() == "[AB 1][BC 3]" && "mst A is [AB 1][BC 3]");

  Tester::resetSs();
  MstLength = Graph.mst("B", Tester::edgeVisitor);
  assert(MstLength == 4 && "mst 4 is 4");
  assert(Tester::getSs() == "[BA 1][BC 3]");

  Tester::resetSs();
  MstLength = Graph.mst("C", Tester::edgeVisitor);
  assert(MstLength == 4 && "mst C is 4");
  assert(Tester::getSs() == "[CB 3][BA 1]");

  Tester::resetSs();
  MstLength = Graph.mst("X", Tester::edgeVisitor);
  assert(MstLength == -1 && "mst X is -1");
  assert(Tester::getSs().empty() && "mst for vertex not found");
}

void testGraph1() {
  cout << "testGraph1" << endl;
  Graph Graph;
  if (!Graph.readFile("graph1.txt")) {
    return;
  }
  Tester::resetSs();
  Graph.dfs("A", Tester::labelVisitor);
  assert(Tester::getSs() == "ABCDEFGH" && "dfs starting from A");

  Tester::resetSs();
  Graph.bfs("A", Tester::labelVisitor);
  assert(Tester::getSs() == "ABHCGDEF" && "bfs starting from A");

  Tester::resetSs();
  Graph.dfs("B", Tester::labelVisitor);
  assert(Tester::getSs() == "BCDEFG" && "dfs starting from B");

  Tester::resetSs();
  Graph.bfs("B", Tester::labelVisitor);
  assert(Tester::getSs() == "BCDEFG" && "bfs starting from B");

  map<string, int> Weights;
  map<string, string> Previous;
  auto Pair = Graph.dijkstra("A");
  Weights = Pair.first;
  Previous = Pair.second;
  assert(map2string(Weights) == "[B:1][C:2][D:3][E:4][F:5][G:4][H:3]" &&
         "Dijkstra(B) weights");
  assert(map2string(Previous) == "[B:A][C:B][D:C][E:D][F:E][G:H][H:A]" &&
         "Dijkstra(B) previous");
}

void testAll() {
  testGraphBasic();
  testGraph0DFS();
  testGraph0BFS();
  testGraph0Dijkstra();
  testGraph0NotDirected();
  testGraph1();
}

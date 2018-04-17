// Graphs.cpp
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

struct Node {
    // constructing the graph might take a little bit longer but algorithms on the graph will be faster
    std::vector<Node*> children; 
    std::string name; 
}; 

struct Graph {
    // cant resize this
    std::vector<Node> nodes; 
}; 

// NOTE: remember that a graph might be stored like the following
typedef uint16_t NodeKey; 
std::unordered_map<NodeKey, std::tuple<std::string, std::unordered_set<NodeKey>>> alternateGraph; 

//------------------------------------------------------------------------------------
// Name: FromAdjacencyMatrix 
// Desc:
//------------------------------------------------------------------------------------
void FromAdjacencyMatrix(Graph& graph, uint8_t* adjacencyMatrix, std::vector<std::string> values, uint16_t nodeCount) {
    graph.nodes.resize(nodeCount); 

    for (auto i = 0; i < values.size(); i++) {
        graph.nodes[i].name = values[i]; 
    }

    // cant really do better than O(n^2) here i think? 
    // we need to iterate through the whole matrix
    for (auto i = 0; i < nodeCount; i++) {        
        for (auto j = 0; j < nodeCount; j++) {
            if (j == i) continue; // i-th node cant connect to itself

            auto element = adjacencyMatrix[i * nodeCount + j]; 

            if (element) {
                auto& childNode = graph.nodes[j];  
                graph.nodes[i].children.push_back(&childNode); 
            }
        }
    }
}

// Chapter 4 
// Breadth First Search
// O(n) memory (i think? worst case we store a pointer to each node...)
// O(n) complexity 
//
bool BreadthFirstSearch(const Graph& graph, const Node* start = nullptr, const Node* find = nullptr) {
    std::deque<const Node*> queue; 
    std::unordered_set<const Node*> visited; 

    if (start) {
        // assuming start is in graph.nodes
        auto result = std::find_if(graph.nodes.begin(), graph.nodes.end(), [start] (const Node& n) {return &n == start; } ); 
        
        if (result != graph.nodes.end()) {    
            queue.push_back(start); 
            visited.insert(start); 
        } else {
            return false; 
        }

    } else {

        visited.insert(&graph.nodes[0]);
        queue.push_back(&graph.nodes[0]); 
    }

    while (!queue.empty()) {
        auto top = queue.front(); 
        queue.pop_front();
        std::cout << top->name << " "; 

        for (auto child : top->children) {
            if (visited.find(child) == visited.end()) {
                visited.insert(child);  

                if (find && (child == find)) {
                    std::cout << child->name << "\n";
                    return true; 
                }

                queue.push_back(child); 
            }
        }
    }

    std::cout << "\n";

    return false; 
}

// Depth first search
// possible use is in simulations for example where 
//
void DepthFirstSearch(const Graph& graph) {
    if (graph.nodes.size() == 0) { return; }
    
    std::unordered_set<const Node*> visitedNodes; 
    std::vector<std::tuple<const Node*, unsigned int>> stack; 
    stack.push_back(&graph.nodes[0]); 

    while (!stack.empty()) {
        auto& top = stack.back(); 
        auto& visited = std::get<1>(top); 
        auto currentNode = std::get<0>(top); 

        if (visited == 0) {
            std::cout << currentNode->name << " "; 
        }

        if (visited < currentNode->children.size()) {
            for (auto node : currentNode->children) {
                visited++; 
                if (visitedNodes.find(node) == visitedNodes.end()) {
                    visitedNodes.insert(node); 
                    stack.push_back(std::tuple<const Node*, unsigned int>(node, 0)); 
                    break; 
                }
            }
        } else {
            stack.pop_back(); 
        }
    }

    std::cout << "\n"; 
}

// 4.1
// Route between nodes
// Given a directed graph, design and algorithm to find out whether there is a route between two nodes
//
bool RouteBetweenNodes(Graph& graph, const Node* node1, const Node* node2) {
    return BreadthFirstSearch(graph, node1, node2);
}

//------------------------------------------------------------------------------------
// Name: main 
// Desc:
//------------------------------------------------------------------------------------
int main() {

    uint8_t adjacencyMatrix[6][6] = 
    {
        {0, 1, 0, 0, 1, 1}, // 0 A
        {0, 0, 0, 1, 1, 0}, // 1 B
        {0, 1, 0, 0, 0, 0}, // 2 C
        {0, 0, 1, 0, 1, 0}, // 3 D
        {0, 0, 0, 0, 0, 0}, // 4 E
        {0, 0, 0, 0, 0, 0}  // 5 F
    }; 

    std::vector<std::string> values = { "0", "1", "2", "3", "4", "5" };

    Graph graph;
    FromAdjacencyMatrix(graph, (uint8_t*)adjacencyMatrix, values, 6); 

    BreadthFirstSearch(graph);

    auto result = BreadthFirstSearch(graph, &graph.nodes[2], &graph.nodes[4]);
    std::cout << result << "\n";

    DepthFirstSearch(graph);

    return 0; 
}




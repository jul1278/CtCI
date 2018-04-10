// Graphs.cpp
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include <unordered_set>

struct Node {
    // constructing the graph might take a little bit longer but algorithms on the graph will be faster
    std::vector<Node*> children; 
    std::string name; 
}; 

struct Graph {
    // cant resize this
    std::vector<Node> nodes; 
}; 

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
bool BreadthFirstSearch(const Graph& graph, const Node* start = nullptr, const Node* find = nullptr) {
    std::deque<const Node*> queue; 
    std::unordered_set<const Node*> visited; 

    if (start) {
        // assuming start is in graph.nodes
        auto result = std::find_if(graph.nodes.begin(), graph.nodes.end(), [start] (const Node& n) {return &n == start; } ); 
        
        if (result != graph.nodes.end()) {    
            queue.push_back(start); 
            visited.insert(start); 
        }

    } else {

        visited.insert(&graph.nodes[0]);
        // TODO: 
        // reserve space in the queue
        queue.push_back(&graph.nodes[0]); 
    }

    while (!queue.empty()) {
        auto top = queue.front(); 
        queue.pop_front();

        std::cout << top->name << " "; 
        for (auto child : top->children) {
            if (visited.find(child) == visited.end()) {
                visited.insert(child);  

                if (child == find) {
                    std::cout << child->name << "\n";
                    return true; 
                }

                // visit 
                queue.push_back(child); 
            }
        }
    }

    std::cout << "\n";

    return false; 
}

// 4.1
// Route between nodes
// Given a directed graph, design and algorithm to find out whether there is a route between two nodes
//
bool RouteBetweenNodes(Graph& graph, const Node* node1, const Node* node2) {
    
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

    std::vector<std::string> values = { "0","1","2","3","4","5" };

    Graph graph;
    FromAdjacencyMatrix(graph, (uint8_t*)adjacencyMatrix, values, 6); 

    BreadthFirstSearch(graph);

    auto result = BreadthFirstSearch(graph, &graph.nodes[2], &graph.nodes[4]);
    std::cout << "\n" << result << "\n";

    return 0; 
}




// Trees.cpp
#include <memory>
#include <iostream>
#include <stack>
#include <tuple>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <math.h>
#include <cmath>

// BinaryNode
struct BinaryNode {
    int value;

    BinaryNode* left;
    BinaryNode* right;

    bool HasChildren() const { return this->left || this->right; }

    ~BinaryNode() {
        delete left;
        delete right; 
    }
};

// BinaryChildNode
struct BinaryChildNode {
    BinaryChildNode* parent;  

    int value;

    BinaryChildNode* left;
    BinaryChildNode* right;

    bool HasChildren() { return this->left || this->right; }
    
    ~BinaryChildNode() {
        delete left;
        delete right; 
    }
};

enum TupleId { NODE = 0, LEFT_VISITED = 1, RIGHT_VISITED = 2, DEPTH = 3 };

typedef std::tuple<const BinaryNode*, bool, bool> TraversalFrameState; 
typedef std::tuple<const BinaryNode*, bool, bool, unsigned int> DepthTraversalFrameState; 

//--------------------------------------------------------------------------------------------------------------
// Name: BinaryInsert 
// Desc: insert into binary tree, not recursive, but unfortunately does allocate on the stack.  
// O(log n) complexity assuming the tree is fairly well balanced
// O(1) memory
//
// Better solution would have a binary tree wrapper class or something which store a pool of nodes 
// so we dont have to new up anything 
// also consider using std::unique_ptr instead of raw pointers
//--------------------------------------------------------------------------------------------------------------
void BinaryInsert(int value, BinaryNode& node) {
    auto nodePtr = &node;

    while (nodePtr) {
        // if value > node.value then 
        if (value == nodePtr->value) {
            return; 
        }

        if (value > nodePtr->value) {
            
            if (nodePtr->right == nullptr) {
                nodePtr->right = new BinaryNode {value, nullptr, nullptr}; 
                return; 
            } 
            
            nodePtr = nodePtr->right;  
        } else { // value < node.value
            
            if (nodePtr->left == nullptr) {
                nodePtr->left = new BinaryNode {value, nullptr, nullptr }; 
                return; 
            }

            nodePtr = nodePtr->left; 
        }
    }
}

//---------------------------------------------------------------------------------------
// Name: VisitInOrder 
// Desc: Visit in order non-recursively 
// O(n) complexity (pretty sure pushing frames on the back of the vector is O(1) as long as it doesnt resize
// O(n) memory as well 
//---------------------------------------------------------------------------------------
void VisitInOrder(BinaryNode& rootNode, unsigned int size = 0) {

    // depends on implementation but std::get accessing tuples slower than accessing a member of a struct
    // use a vector instead of a stack so we only call new when we need to resize
    // remember to think about how STL containers actually work internally, when might they be calling new?
    std::vector<TraversalFrameState> nodes; 

    // Reserve space for the vector here but we have no idea how big the tree is
    if (size > 0) {
        nodes.reserve(size); 
    }

    auto currentNode = &rootNode; 
    nodes.push_back(TraversalFrameState(currentNode, false, false)); 

    while(nodes.size() > 0) {

        // capture the tuple by reference!!
        auto& stackTop = nodes.back(); 
        auto currentNode = std::get<NODE>(stackTop);

        auto leftVisited = std::get<LEFT_VISITED>(stackTop);
        auto rightVisited = std::get<RIGHT_VISITED>(stackTop); 

        if (!currentNode->right) {
            //std::get<RIGHT_VISITED>(stackTop) = true;
            rightVisited = true; 
            //std::cout << " right visited "; 
        }
        if (!currentNode->left) {
            leftVisited = true; 
            // std::cout << " left visited: " << leftVisited << " "; 
        }

        // node has no children
        if (!currentNode->left && !currentNode->right) {
            std::cout << currentNode->value << " "; 
            nodes.pop_back(); 
        } else {

            if (currentNode->left) {
                if (!leftVisited) {
                    // visit left node
                    auto nextNode = currentNode->left; 
                    std::get<LEFT_VISITED>(stackTop) = true; 
                    
                    if (nextNode->HasChildren()) {
                        nodes.push_back(TraversalFrameState(nextNode, false, false)); 
                    } else {
                        std::cout << nextNode->value << " "; 
                    }
                    
                    continue; 
                }
            } 

            // we've visted left and NOT right
            if (!rightVisited && leftVisited) {
                // visit the current node
                std::cout << currentNode->value << " "; 
            } 

            if (currentNode->right) {
                if (!rightVisited) {
                    // vist right node
                    auto nextNode = currentNode->right; 
                    std::get<RIGHT_VISITED>(stackTop) = true;
                    
                    if (nextNode->HasChildren()) {
                        nodes.push_back(TraversalFrameState(nextNode, false, false)); 
                    } else {
                        std::cout << nextNode->value << " "; 
                    }
                    
                    continue; 
                } 
            } 

            if (leftVisited && rightVisited) {
                nodes.pop_back(); 
            }
        }
    }

    std::cout << "\n"; 
}

//----------------------------------------------------------------------------------------------------------------
// Name: PreOrderVisit
// Desc: 
// O(n) complexity and memory
//----------------------------------------------------------------------------------------------------------------
void PreOrderVisit(BinaryNode& rootNode, unsigned int size = 0) {

    // depends on implementation but std::get accessing tuples slower than accessing a member of a struct
    // use a vector instead of a stack so we only call new when we need to resize
    std::vector<TraversalFrameState> nodes;
    if (size > 0) {
        nodes.reserve(size); 
    }  

    auto currentNode = &rootNode; 
    nodes.push_back(TraversalFrameState(currentNode, false, false)); 

    while(nodes.size() > 0) {
        // capture the tuple by reference!!
        auto& stackTop = nodes.back(); 

        auto currentNode = std::get<NODE>(stackTop);
        auto leftVisited = std::get<LEFT_VISITED>(stackTop);
        auto rightVisited = std::get<RIGHT_VISITED>(stackTop); 

        // visit the node straight away
        if (!leftVisited) { std::cout << currentNode->value << " "; }
        
        // if we've visited the left node then dont visit me
        if (!currentNode->left && !currentNode->right) {
            nodes.pop_back();
        } else { 

            if (!leftVisited) {
                if (currentNode->left) {
                    auto nextNode = currentNode->left; 
                    std::get<LEFT_VISITED>(stackTop) = true; 
                    
                    if (nextNode->HasChildren()) {
                        nodes.push_back(TraversalFrameState(nextNode, false, false)); 
                    } else {
                        std::cout << nextNode->value << " ";
                    }

                } else {
                    std::get<LEFT_VISITED>(stackTop) = true; 
                }

            } else if (!rightVisited) {
                if (currentNode->right) {
                    auto nextNode = currentNode->right; 
                    std::get<RIGHT_VISITED>(stackTop) = true; 
                    
                    if (nextNode->HasChildren()) {
                        nodes.push_back(TraversalFrameState(nextNode, false, false)); 
                    } else {
                        std::cout << nextNode->value << " ";
                    }

                } else {
                    std::get<RIGHT_VISITED>(stackTop) = true; 
                }

                std::get<RIGHT_VISITED>(stackTop) = true; 
            }

            if (leftVisited && rightVisited) {
                nodes.pop_back();
            }
        }
    }

    std::cout << "\n"; 
}

//----------------------------------------------------------------------------------------------------------------
// Name: PostOrderVisit
// Desc: 
// O(n) complexity and memory
//----------------------------------------------------------------------------------------------------------------
void PostOrderVisit(BinaryNode& rootNode, unsigned int size = 0) {

    // depends on implementation but std::get accessing tuples slower than accessing a member of a struct
    // use a vector instead of a stack so we only call new when we need to resize
    std::vector<TraversalFrameState> nodes;
    if (size > 0) {
        nodes.reserve(size); 
    }  

    auto currentNode = &rootNode; 
    nodes.push_back(TraversalFrameState(currentNode, false, false)); 

    while(nodes.size() > 0) {
        // capture the tuple by reference!!
        auto& stackTop = nodes.back(); 

        auto currentNode = std::get<NODE>(stackTop);
        auto leftVisited = std::get<LEFT_VISITED>(stackTop);
        auto rightVisited = std::get<RIGHT_VISITED>(stackTop); 

        if (!currentNode->left && !currentNode->right) { 
            std::cout << currentNode->value << " ";
            nodes.pop_back();     
        } else {

            if (!currentNode->left) { std::get<LEFT_VISITED>(stackTop) = true; }
            if (!currentNode->right) { std::get<RIGHT_VISITED>(stackTop) = true; }

            // visit left
            // neither left or right visited
            if (!rightVisited && !leftVisited && currentNode->left) {
                auto nextNode = currentNode->left; 
                std::get<LEFT_VISITED>(stackTop) = true; 

                // look ahead here and dont bother pushing on the vector if node has no children
                if (nextNode->HasChildren()) {
                    nodes.push_back(TraversalFrameState(nextNode, false, false)); 
                } else {
                    std::cout << nextNode->value << " ";     
                }
            }

            // visit right 
            // left and not right 
            else if (leftVisited && !rightVisited && currentNode->right) { // visit right
                auto nextNode = currentNode->right; 
                std::get<RIGHT_VISITED>(stackTop) = true; 

                // look ahead here and dont bother pushing on the vector if node has no children
                if (nextNode->HasChildren()) {
                    nodes.push_back(TraversalFrameState(nextNode, false, false)); 
                } else {
                    std::cout << nextNode->value << " ";     
                }
            }

            // visited both
            else if (leftVisited && rightVisited) { 
                std::cout << currentNode->value << " "; 
                nodes.pop_back(); 
            }
        }
    }

    std::cout << "\n"; 
}

// 4.2 
// Given a sorted (increasing order) array with unique integer elements, write an algorithm 
// to create a binary search tree with minimal height. 
//
//
//
/*
/*

{2, 4, 6, 8, 10, 20}

            8
          /   \
        4      10
      /  \       \
     2    6       20


            6
           / \
          4   10
         /    / \
        2    8   20


first call
array: 2,4,6,8,10,20 arrayLen: 6 
middleIndex = 3
node value = 8

second call left:
array: 2,4,6 arraylen = 3
middleIndex = 1
node value = 4
     
    third call left:
    array: 2, arrayLen = 1

    third call right: 
    array: 6, arrayLen = 1

second call right
array: 8, 10, 20 arrayLen = 3
middleIndex = 1
node value = 10

*/  

// {2, 4, 6, 8, 10, 20}; 

// {10, 20}

void MinimalTreeRecursive(BinaryNode& binaryNode, uint32_t* array, uint32_t arrayLen) {
    auto middleIndex = arrayLen / 2;  // NOTE: should we be doing (arrayLen - 1)/2 ???? 

    if (arrayLen <= 1) { return; }

    if (middleIndex > 1) {

        binaryNode.value = array[middleIndex];

        binaryNode.left = new BinaryNode();
        binaryNode.right = new BinaryNode(); 
        MinimalTreeRecursive(*binaryNode.left, array, middleIndex); 

        uint32_t subArrayLen = arrayLen - middleIndex - 1; 

        MinimalTreeRecursive(*binaryNode.right, array + middleIndex + 1, subArrayLen); // middleIndex /* - 1 */);

    } else {

        if (arrayLen == 3) {
            binaryNode.value = array[middleIndex];

            binaryNode.right = new BinaryNode(); 
            binaryNode.right->value = array[arrayLen - 1]; 

            binaryNode.left = new BinaryNode(); 
            binaryNode.left->value = *array; 
             
        } else if (arrayLen == 2) {
            binaryNode.value = *array;

            binaryNode.right = new BinaryNode();
            binaryNode.right->value = array[arrayLen - 1];            
        }
    }
} 

// 4.3 List Of Depths
// Given a binary tree, design an algorithm which creates a linked list of all the nodes at each depth 
// this one returns a vector but its pretty simple to use a linked list instead
std::vector<const BinaryNode*> ListOfDepth(const BinaryNode& tree, const unsigned int depth) {
    
    std::vector<const BinaryNode*> nodesAtDepth; 
    std::vector<DepthTraversalFrameState> nodes;
    unsigned int currentDepth = 0; 

    nodes.push_back(DepthTraversalFrameState(&tree, false, false, currentDepth)); 

    while (!nodes.empty()) {
        // get the top
        auto& frameState = nodes.back(); 

        auto currentNode = std::get<NODE>(frameState);
        auto leftVisited = std::get<LEFT_VISITED>(frameState); 
        auto rightVisited = std::get<RIGHT_VISITED>(frameState); 
        auto currentDepth = std::get<DEPTH>(frameState); 

        // DEBUG
        // std::cout << currentNode->value << " " << currentDepth << " " << leftVisited << " " << rightVisited << " "; 

        if ( (leftVisited && rightVisited) || (currentDepth > depth)) {
            nodes.pop_back();
            // DEBUG
            // std::cout << "pop"; 
        } else {
            if (currentDepth == depth) {
                // DEBUG
                // std::cout << "added node"; 
                nodesAtDepth.push_back(currentNode); 
                nodes.pop_back(); 
            } else if (currentDepth < depth) {
                
                if (!currentNode->left && !currentNode->right) {
                    nodes.pop_back(); 
                }

                if (!leftVisited) {
                    std::get<LEFT_VISITED>(frameState) = true;
                    if (currentNode->left) { 
                        nodes.push_back(DepthTraversalFrameState(currentNode->left, false, false, currentDepth + 1)); 
                    }                      
                }
                else if (!rightVisited) {
                    std::get<RIGHT_VISITED>(frameState) = true; 
                    if (currentNode->right) {
                        nodes.push_back(DepthTraversalFrameState(currentNode->right, false, false, currentDepth + 1));
                    }
                }
            } 
        }

        // DEBUG to 'step through'
        // std::cin.get(); 
    }

    return nodesAtDepth; 
}

//---------------------------------------------------------------------------------
// Name: Depth
// Desc: 
//---------------------------------------------------------------------------------
unsigned int Depth(const BinaryNode& parent) {
    
    std::vector<DepthTraversalFrameState> nodes;
    unsigned int depth = 0;

    nodes.push_back(DepthTraversalFrameState(&parent, false, false, 0)); 

    // get node off stack
    // push left and right child on stack and increment left depth counter
    // compare depth counters to largest seen depth counters
    // pop stack if node has no children
    // when stack is empty compare left and right counters

    while(!nodes.empty()) {

        auto& frame = nodes.back(); 
        auto currentNode = std::get<NODE>(frame); 
        auto currentDepth = std::get<DEPTH>(frame); 
        auto& leftVisited = std::get<LEFT_VISITED>(frame);
        auto& rightVisited = std::get<RIGHT_VISITED>(frame); 

        //std::cout << currentDepth << " "; 
        
        if (currentDepth > depth) {
            depth = currentDepth; 
        }

        if ( (!currentNode->left && !currentNode->right) 
            || (leftVisited && rightVisited)) {
            nodes.pop_back(); 
        } else {

            if (!currentNode->left) {
                //std::cout << "L "; 
                std::get<LEFT_VISITED>(frame) = true; 
            } 

            if (!currentNode->right) {
                //std::cout << "R "; 
                std::get<RIGHT_VISITED>(frame) = true;
            }

            if (!leftVisited) {
                std::get<LEFT_VISITED>(frame) = true; 
                // DEBUG
                //std::cout << "push back left "; 
                nodes.push_back(DepthTraversalFrameState(currentNode->left, false, false, currentDepth + 1)); 
            } 
            else if (!rightVisited) {
                std::get<RIGHT_VISITED>(frame) = true; 
                // DEBUG
                //std::cout << "push back right "; 
                nodes.push_back(DepthTraversalFrameState(currentNode->right, false, false, currentDepth + 1));
            }
        }
    }

    return depth;
}

// 4.4 Check Balanced
// Implement a function to check if a binary tree is balanced 
// For the purposes of this question, a balanced tree is considered
// to be a binary tree such that the heights of the two subtrees of any node 
// never differ by more than one
//
bool IsBalanced(BinaryNode& rootNode) {

    int leftDepth = 0; 
    int rightDepth = 0;  

    if (rootNode.left) {
        leftDepth = Depth(*rootNode.left);
    }

    if (rootNode.right) {
        rightDepth = Depth(*rootNode.right);
    }

    return (abs(leftDepth - rightDepth) <= 1); 
}

// Is a valid binary search tree? 
// all nodes to the left <= n and all nodes to the right are > 
bool IsBST_Rec(const BinaryNode& root) {
    
    auto result = true; 
    
    if (root.left != nullptr && root.left->value <= root.value) {
        result |= IsBST_Rec(root); 
    }

    if (root.right != nullptr && root.right->value > root.value) {
        result |= IsBST_Rec(root); 
    }

    return result; 
}

// Is a valid binary search tree? 
// all nodes to the left <= n and all nodes to the right are > 
bool IsValidBST(const BinaryNode& root) {

    typedef std::tuple<const BinaryNode*, bool, bool> stackFrame; 

    std::vector<stackFrame> stack; 

    stack.push_back(stackFrame(&root, false, false));

    while (!stack.empty()) {
        auto currentNode = std::get<0>(stack.back()); 
        auto& leftVisited = std::get<1>(stack.back()); 
        auto& rightVisited = std::get<2>(stack.back()); 

        // DEBUG
        //std::cout << currentNode->value << std::endl; 

        if (!leftVisited) {
            if (currentNode->left != nullptr){
                
                if(currentNode->left->value <= currentNode->value) {
                    std::get<1>(stack.back()) = true; 
                    stack.push_back(stackFrame(currentNode->left, false, false));
                } else {

                    return false; 
                }
            } else {

                leftVisited = true; 
            }
        }
        else if (!rightVisited) {
            if (currentNode->right != nullptr) {
                if (currentNode->right->value > currentNode->value) {
                    std::get<2>(stack.back()) = true; 
                    stack.push_back(stackFrame(currentNode->right, false, false));           
                }
            } else {

                rightVisited = true; 
            }
        }
        else if (rightVisited && leftVisited) 
        {
            stack.pop_back(); 
        }
    }

    return true; 

    // while stack in not empty 
    // {
    //  if left node not visited    
    //      check left node value less than or equal to current node value
    //          push left node on stack                
    //      else 
    //          return false 
    //
    //  if right node not visited
    //      check right node value greater than current node value
    //          push right node on stack
    //      else
    //          return false 
    //
    //  if left and right are visited then pop
    // }
}

// Successor
// return the leftmost node of the righhand subtree
BinaryChildNode& Successor(BinaryChildNode& node) {

    // if node has no right child
    //   return parent

    if (node.right == nullptr) {
        return *node.parent; 
    }

    while (node.left != nullptr) {
        node = *node.left;
    }

    return node; 

    // loop
    // if node has left child 
    //      node = left child
    // else
    //      return node
}

// 4.7 Build Order
// You are given a list of projects and a list of dependencies (which is a list of pairs of projects where the second project is dependent on the first project).
// All the projects dependencies must be built before the project is. Find a build order that will allow the projects to be built, if there is no valid build order
// return an error.
void BuildOrder() {
    // Example
    //
    // projects: a, b, c, d, e, f
    // dependencies: (a,d), (f,b, (b,d), (f,a), (d,c)

    // output: f, e, a, b, d, c

    std::list<std::string> projects = {"a", "b", "c", "d", "e", "f"};

    std::list<std::tuple<std::string, std::string>> dependencies = 
    {
        std::tuple<std::string, std::string>("a", "d"),
        std::tuple<std::string, std::string>("f", "b"),
        std::tuple<std::string, std::string>("b", "d"),
        std::tuple<std::string, std::string>("f", "a"),
        std::tuple<std::string, std::string>("d", "c")
    }; 

    // build a map "name"-> project
    
    // first need to find a project that has no dependencies and other projects depend on it
    //
    // everything in the dependencies pairs list either depends on something or is depended upon 
    //
    // map string -> int 
    // count the number of dependencies each project has
    // project with zero dependencies is where we can start

    std::unordered_set<std::string> projectSet;

    // Project -> who is directly dependent on me
    std::unordered_map<std::string, std::vector<std::string>> dependencyMap; 
    
    // Projects which are dependent on another project
    std::unordered_set<std::string> dependencyCounter; 

    for (auto pair : dependencies) {
        auto a = std::get<0>(pair); 
        auto b = std::get<1>(pair); 

        dependencyMap[a].push_back(b);  
        dependencyCounter.insert(b);

        projectSet.insert(a); 
        projectSet.insert(b); 
    }

    std::string startDependency = ""; 

    for (auto pair : dependencies) {
        auto project = std::get<0>(pair); 

        if (dependencyCounter.find(project) == dependencyCounter.end()) {
            startDependency = project;
            break; 
        }
    }

    // DEBUG
    // for(auto pair : dependencyMap) {
    //     auto project = std::get<0>(pair);
    //     auto dependencies = std::get<1>(pair); 

    //     std::cout << project.c_str() << ": "; 
    //     for(auto dependency : dependencies) {
    //         std::cout << dependency.c_str() << ", "; 
    //     }

    //     std::cout << "\n"; 
    // }

    if (startDependency == "") {
        std::cout << "Error!";
        return; 
    }
    
    std::vector<std::string> buildOrder; 

    // Assuming we found a project that isnt dependent on anything where other projects are dependent on it... 
    std::cout << "Build order: "; 

    // first build all the projects with no dependencies/depended upon
    for (auto project : projects) {
        if (projectSet.find(project) == projectSet.end()) {
            // std::cout << project.c_str() << ", "; 
            buildOrder.push_back(project); 
        }
    }

    // stack
    // push most depenent project on the stack
    
    std::unordered_set<std::string> visitedProjects; 
    std::list<std::string> stack; 
    stack.push_back(startDependency); 

    while(!stack.empty()) {

        auto project = stack.back(); 
        stack.pop_back();

        if (visitedProjects.find(project) == visitedProjects.end()) {
            //std::cout << project.c_str() << ", "; 
            buildOrder.push_back(project); 
            
            visitedProjects.insert(project); 
            auto result = dependencyMap.find(project); 

            if (result != dependencyMap.end()) {
                auto dependencies = dependencyMap[project]; 
                for(auto dependency : dependencies) {
                    if (visitedProjects.find(dependency) != visitedProjects.end()) {
                        // Error Stop
                        std::cout << "Error! "; 
                        return; 

                    } else {

                        stack.push_front(dependency); 
                    }
                }
            }
            
        }

        // get top project off the stack
        // if not in visited hashset
        //  print
        //  if any of my dependencies are in visited hashset
        //      error
        //  else
        //      push in visited hashset
    }

    for(auto i = 0; i < buildOrder.size();  i++) {
        
        std::cout << buildOrder[i].c_str();

        if (i < buildOrder.size() - 1) {
            std::cout << ", "; 
        }      
    }
} 

// 4.8 First Common Ancestor
// Design an algorithm and write code to find the first common ancestor of two nodes
// in a binary search tree. Avoid storing additional nodes in a data structure
// NOTE: this is not neccessarily a binary search tree
//
//
//         5
//     4      3
//   2   1   6  7
//                8
//
//
//
BinaryChildNode& FirstCommonAncestor(BinaryChildNode& nodeA, BinaryChildNode& nodeB) {

    // starting from nodeB and nodeB walk both up the tree until we find the root node of the tree. 

    // store the root node

    // start from nodeA and nodeB walk up the tree until both 'walkers' reach a child of the stored node
    // if both walkers have reach the same child 
    //      store child node and repeat
    // else 
    //      stored node is the most common ancestor

    BinaryChildNode* storedNode = nullptr; 
    BinaryChildNode* walkerA = &nodeA;
    BinaryChildNode* walkerB = &nodeB;
    
    while (walkerA->parent != nullptr) {
        walkerA = walkerA->parent;
    }

    storedNode = walkerA;

    walkerA = &nodeA;
    walkerB = &nodeB;

    bool foundCommonNode = false; 

    // DEBUG
    if (storedNode != nullptr) {
        std::cout << "Found topmost parent node: " << storedNode->value << "\n";
    }

    std::cout << "Node A: " << walkerA->value << "\n";
    std::cout << "Node B: " << walkerB->value << "\n";

    // DEBUG
    // return *storedNode;

    // NOTE: ideally should walk B back up to the root to in order to prove they both actually do share common ancestors

    while (!foundCommonNode) {
        bool walkerAStop = false; 
        bool walkerBStop = false; 

        // DEBUG
        // std::cout << "Walking\n";

        // walk 
        while (!walkerAStop && !walkerBStop) {
            
            // DEBUG
            // std::cout << "...\n";

            if (!walkerAStop) {
                if ( (storedNode->left != nullptr && storedNode->left == walkerA) 
                || (storedNode->right != nullptr && storedNode->right == walkerA)) {
                    walkerAStop = true; 
                } else {
                    
                    if (walkerA->parent != nullptr) {
                        std::cout << "A: " << walkerA->value << " -> ";  
                        walkerA = walkerA->parent; 
                        std::cout << walkerA->value << "\n"; 
                    } else {
                        walkerAStop = true; 
                    }
                } 
            }
            
            if (!walkerBStop) {
                if ( (storedNode->left != nullptr && storedNode->left == walkerB) 
                || (storedNode->right != nullptr && storedNode->right == walkerB)) {
                    walkerBStop = true; 
                } else {

                    if (walkerB->parent != nullptr) {

                        std::cout << "B: " << walkerB->value << " -> ";  
                        walkerB = walkerB->parent; 
                        std::cout << walkerB->value << "\n"; 
                    } else {
                        walkerBStop = true; 
                    }
                }
            }
        }

        // if the nodes are the same then we need to iterate again
        if (walkerA == walkerB) {
            storedNode = walkerA; 

            walkerA = &nodeA; 
            walkerB = &nodeB; 

        } else {
            foundCommonNode = true; 
        }
    }    

    // DEBUG
    if (storedNode != nullptr) {
        std::cout << "Found lowest common ancestor node: " << storedNode->value << "\n";
    }

    return *storedNode; 
}

// 4.9 
// A binary search tree was created by traversing through an array from left to right and inserting each element
// Given a BST with distinct elements, print all possible arrays that could have led to this tree. 
// 
// Example 
//
//   2
// 1   3
//
// Answer: {2, 1, 3}, {2, 3, 1}


// NOTE: not tested very and recursive so not a very good solution
// come back and do a non-recursive solution
std::list<std::list<int>> Sequences(const BinaryNode& node) {
    std::list<std::list<int>> resultList; 

    if (!node.HasChildren()) {
        std::list<int> lst; 
        lst.push_back(node.value); 
        resultList.push_back(lst); 

        return resultList; 
    }

    std::list<std::list<int>> leftSeq; 
    std::list<std::list<int>> rightSeq;
    
    if (node.left != nullptr) {
        leftSeq = Sequences(*node.left); 
    }

    if (node.right != nullptr) {
        rightSeq = Sequences(*node.right); 
    }   

    auto value = node.value; 

    if (!leftSeq.empty()) {

        for(auto& llst : leftSeq) {
            if (!rightSeq.empty()) {
                for(auto& rlst : rightSeq) {
                    std::list<int> lst;

                    lst.push_back(value); 
                    
                    auto func = [&lst] (int n) {lst.push_back(n); }; 

                    std::for_each(llst.begin(), llst.end(), func);
                    std::for_each(rlst.begin(), rlst.end(), func); 

                    resultList.push_back(lst); 
                }
            }
        }
    }

    if (!rightSeq.empty()) {

        
        for(auto& rlst : rightSeq) {
            if (!leftSeq.empty()) {
                for(auto& llst : leftSeq) {
                    std::list<int> lst;

                    lst.push_back(value); 
                    
                    auto func = [&lst] (int n) {lst.push_back(n); }; 

                    std::for_each(rlst.begin(), rlst.end(), func); 
                    std::for_each(llst.begin(), llst.end(), func);

                    resultList.push_back(lst); 
                }
            }
        }
    }

    return resultList;     
}

std::list<std::list<int>> Sequences(const BinaryNode& node) {

    std::list<std::list<int>> result;
    std::unordered_set<const BinaryNode*, std::list<std::list<int>>> sequences; 

    std::vector<const BinaryNode*> stack; 

    stack.push_back(&node); 

    while(!stack.empty()) {

        // get top of stack node

        // check if sequences contains left 

        // cjhec

    }
}

/*

------ EXAMPLE 1 ------ 

return []

------ EXAMPLE 2 ------

        4 
  2          6
1   3      5   7

Answer: 
LEFT NODE SEQ.
{A, B} = {{2, 1, 3}, {2, 3, 1}}

    PARENT SEQ:
        {{2}}

    LEFT SEQ:
        {{1}}

    RIGHT SEQ:
        {{3}}

    result list
    for each list in parent list:
        
        for each llst in left seq:
            
            for each rlst in right seq:
                result_list push back ( list + llist + rlist ) 

RIGHT NODE SEQ. 
{C, D} = {{6, 5, 7}, {6, 7, 5}}
    
now we need to generate all combinations of these where we pick one from each set

AC, AD, BC, BD, CA, CB, DA, DB

{4, 2, 1, 3, 6, 5, 7}, 
{4, 2, 1, 3, 6, 7, 5},
{4, 2, 3, 1, 6, 5, 7},
{4, 2, 3, 1, 6, 7, 5}, 
{4, 6, 5, 7, 2, 1, 3},
{4, 6, 5, 7, 2, 3, 1},
{4, 6, 7, 5, 2, 1, 3},
{4, 6, 7, 5, 2, 3, 1}

return [[current node, left node permutations, right node permutations], [current node, right node permutations, left node permutations]]

    [[4, left node permutations, right node permutations], [4, right node permutations, left node permutations]]

    right node permutations = [[6, left node perm., right node perm.], [6, right node perm., left node perm.]]
        left node perm. = [[5]]
        right node perm. = [[7], [7]]

        result = [[6, 5, 7], [6, 7, 5]]

    left node permutations = [[2, left node permutations, right node permutations], [2, right node permutations, left node permutations]]
        left node perm. = [[1]]
        right node perm. = [[3]]

        result = [[2, 1, 3], [2, 3, 1]]

        foreach(leftPermutation) {
            foreach(rightPermutation) {
                add to list [4, left perm. , right perm]
            }
        }

        foreach(rightPermutation) {
            foreach(leftPermutation) {
                add to list [4, right perm. , left perm]
            }
        }

        [
            [4, left node perm1, right node perm 1],
            [4, left node perm2, right node perm 1],
            [4, left node perm1, right node perm 2],
            [4, left node perm2, right node perm 2],

            [4, right node perm1, left node perm 1],
            [4, right node perm2, left node perm 1],
            [4, right node perm1, left node perm 2],
            [4, right node perm2, left node perm 2],
        ]
*/

// std::list<std::list<int>> Sequences(const BinaryNode& node) {
    
//     std::list<std::list<int>> emptyList; 
//     return Sequences(node, emptyList); 
// }

/*

------ EXAMPLE 2 ------

        4 
  2          6
1   3      5   7


*/

void BSTSequences() {
    BinaryNode node1;
    BinaryNode node2;
    BinaryNode node3;
    BinaryNode node4;
    BinaryNode node5;
    BinaryNode node6;
    BinaryNode node7; 

    node1.value = 1;
    node2.value = 2;
    node3.value = 3; 
    node4.value = 4;
    node5.value = 5;  
    node6.value = 6; 
    node7.value = 7; 

    node4.left = &node2; 
    node4.right = &node6; 
    
    node6.left = &node5; 
    node6.right = &node7;

    node5.left = nullptr;
    node5.right = nullptr;

    node7.left = nullptr;
    node7.right = nullptr;        

    node2.left = &node1;
    node2.right = &node3;

    node1.left = nullptr;
    node1.right = nullptr;
    
    node3.left = nullptr;
    node3.right = nullptr; 

    auto result = Sequences(node4); 

    for(auto& list : result) {
        for(auto& l : list) {
            std::cout << l << ", "; 
        }

        std::cout << "\n";
    }
}


// PrintTree
//
void PrintTree(BinaryNode& root) {

    unsigned int minNodeSeparation = 3; 
    auto depth = 6;//Depth(root); 

    auto bottomWidth = (unsigned int) std::ceil(std::pow(2, depth)) * minNodeSeparation; 

    // make bottomWidth even
    if ((bottomWidth % 2) == 1) {
        bottomWidth++; 
    }

    std::list<unsigned int> queue; 
    queue.push_back(0); 

    for (auto l = 0; l < depth; l++) {
        std::list<unsigned int> nextQueue;
        nextQueue.push_back(0);
        
        auto sep = (unsigned int) pow(2, l + 1); 
        auto next = 0; 

        for (auto i = 0; i < bottomWidth; i++) {
            auto f = (bottomWidth/sep); 
            if ((queue.front() + f) == i && !queue.empty()) {
                queue.pop_front(); 
                std::cout << "N"; 

                nextQueue.push_back(i); 

            } else {
                std::cout << " "; 
            }
        }

        std::cout << "\n"; 

        // TODO: fix this
        // have pointers to queues and swap them 
        queue.clear();
        for(auto n : nextQueue) {
            queue.push_back(n); 
        }
    }
}

void PrintTree2(BinaryNode& root) {
    unsigned int minNodeSeparation = 2; 
    auto depth = 4;//Depth(root); 

    auto bottomWidth = (unsigned int) std::ceil(std::pow(2, depth)) * minNodeSeparation; 

    // make bottomWidth even
    if ((bottomWidth % 2) == 1) {
        bottomWidth++; 
    }

    std::list<int> points; 
    std::list<BinaryNode*> nodes; 

    auto currentSeparation = bottomWidth / 2;
    points.push_back(currentSeparation); 

    nodes.push_back(&root); 

    for (auto l = 0; l < depth; l++) {

        std::list<int> nextPoints; 
        std::list<std::tuple<int, bool>> linkPoints; 
        std::list<BinaryNode*> nextNodes; 

        // DEBUG
        // for(auto n : points) {
        //     std::cout << n << " "; 
        // }

        // std::cout << "\n"; 

        auto accumulator = 0; 

        for (auto i = 0; i < bottomWidth; i++) {
            if (i == points.front()) {

                auto node = nodes.front(); 
                auto offset = 0;
                
                if (node != nullptr) {
                    auto str = std::to_string(node->value);  
                    
                    std::cout << str;   
                    nextNodes.push_front(nodes.front()->left);

                    if (str.size() > 1) {
                        // DEBUG
                        // std::cout << "c"; 
                        offset += str.size() - 1; 
                    }

                    if (nodes.front()->left != nullptr) {
                        auto num = offset + i - (currentSeparation / 4); 
                        linkPoints.push_back(std::tuple<int, bool>(num, true));
                    }

                    nextNodes.push_front(nodes.front()->right);
                    
                    if (nodes.front()->right != nullptr) {
                        auto num = offset + i + (currentSeparation / 4); 
                        linkPoints.push_back(std::tuple<int, bool>(num, false));
                    }
                    
                } else {
                    std::cout << " "; // actually print the value at nodes.front()

                    nextNodes.push_front(nullptr);
                    nextNodes.push_front(nullptr);
                }
                
                points.pop_front(); 
                
                nextPoints.push_back(offset + i - (currentSeparation / 2));
                nextPoints.push_back(offset + i + (currentSeparation / 2)); 

                nodes.pop_front(); 
                
            } else {
                std::cout << " "; 
            }
        }

        std::cout << "\n";

        // // debug
        // for (auto& pair : linkPoints) {
        //     auto n = std::get<0>(pair); 
        //     std::cout << n << " "; 
        // }

        // std::cout << "\n"; 
        
        for (auto i = 0; i < bottomWidth; i++) {
            auto front = linkPoints.front(); 
            auto num = std::get<0>(front); 
            auto left = std::get<1>(front); 

            if (num == i) {
                
                if (left) {
                    std::cout << "/";
                    left = false;
                } else {
                    std::cout << "\\";
                    left = true; 
                }
                
                linkPoints.pop_front(); 
            } else {
                std::cout << " "; 
            }

        }

        linkPoints.clear(); 

        currentSeparation = currentSeparation / 2;

        std::cout << "\n"; 
        
        points.clear(); 
        
        for (auto n : nextPoints) {
            points.push_back(n); 
        }

        for (auto n : nextNodes) {
            nodes.push_front(n);
        }
    }
}

//-------------------------------------------------------------------------------- 
// Name:
// Desc: 
//--------------------------------------------------------------------------------
int main() {
    BinaryNode node {10, nullptr, nullptr}; 

    BinaryInsert(12, node); 
    BinaryInsert(15, node); 
    BinaryInsert(8, node); 
    BinaryInsert(9, node); 
    BinaryInsert(4, node); 

    // tree should look like
    /*
             10
            /  \
          8      12
        /  \       \
       4    9       15

    // */

    // // 15 12 10 9 8 4
    // VisitInOrder(node); 

    // // 10 8 4 9 12 15
    // PreOrderVisit(node);

    // // 4 9 8 15 12 10
    // PostOrderVisit(node);

    // /*
    //         8
    //       /   \
    //     4      10
    //   /  \       \
    //  2    6       20
    // */
    // uint32_t array[] = {2, 4, 6, 8, 10, 20}; 
    // BinaryNode root; 
    // MinimalTreeRecursive(root, array, 6); 

    // VisitInOrder(root); 
    // PreOrderVisit(root); 
    // PostOrderVisit(root); 

    /*
        8
       / \   
      4   20
     / \
    2   6
    */   

      

    //uint32_t array2[] = {2, 4, 6, 8, 20}; 
    uint32_t array2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}; 
    BinaryNode root2; 
    MinimalTreeRecursive(root2, array2, 15); 

    // // VisitInOrder(root2); 
    // // PreOrderVisit(root2);  // 4 2 8 6 20
    // // PostOrderVisit(root2); 

    // // std::cout << root2.value << "\n";
    // // std::cout << root2.left->value << "\n"; 
    // // std::cout << root2.right->value << "\n"; 

    // /*
    //    6
    //  /   \
    // 2     8
    //  \      \
    //   4      20
    // */
    
    std::cout << IsBalanced(root2); 


    // auto nodesAtDepth = ListOfDepth(root2, 2); 

    // for (auto node : nodesAtDepth) {
    //     std::cout << node->value << " "; 
    // }



    std::cout << "\n";
    std::cout << "Is valid BST: " << IsValidBST(root2) << "\n"; 
    
    std::cout << "\n";
    PrintTree2(root2); 

    std::cout << "\n";

    PrintTree2(node); 

    std::cout << "\n";

    // Build a tree lol
    // Parent, Value, Left, Right
    //
    //         5
    //     4      3
    //   2   1   6  7
    //                8

    BinaryChildNode node1; node1.value = 1;
    BinaryChildNode node2; node2.value = 2;
    BinaryChildNode node3; node3.value = 3;
    BinaryChildNode node4; node4.value = 4;
    BinaryChildNode node5; node5.value = 5;  
    BinaryChildNode node6; node6.value = 6;
    BinaryChildNode node7; node7.value = 7;
    BinaryChildNode node8; node8.value = 8;

    node8.parent = &node7; 
    node8.left = nullptr;
    node8.right = nullptr; 

    node7.parent = &node3;
    node7.right = &node8;
    node7.left = nullptr; 

    node6.parent = &node3; 
    node6.left = nullptr;
    node6.right = nullptr;
    
    node3.left = &node6; 
    node3.right = &node7; 
    node3.parent = &node5; 
    
    node4.parent = &node5; 
    node4.left = &node2; 
    node4.right = &node1; 
    
    node5.right = &node3; 
    node5.left = &node4; 
    node5.parent = nullptr; 
    
    node2.parent = &node4; 
    node2.left = nullptr; 
    node2.right = nullptr; 

    node1.parent = &node4; 
    node1.left = nullptr; 
    node1.right = nullptr; 

    FirstCommonAncestor(node8, node1); 

    std::cout << "\n";

    BSTSequences(); 
    
    std::cout << "\nDone!"; 

    // BuildOrder(); 



    return 0; 
}
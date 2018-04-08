// TreesAndGraphs.cpp
#include <memory>
#include <iostream>
#include <stack>
#include <tuple>
#include <vector>

struct BinaryNode {
    int value;

    BinaryNode* left;
    BinaryNode* right;

    bool HasChildren() { return this->left || this->right;}

    ~BinaryNode() {
        delete left;
        delete right; 
    }
};

enum TupleId { NODE = 0, LEFT_VISITED = 1, RIGHT_VISITED = 2 };

typedef std::tuple<BinaryNode*, bool, bool> TraversalFrameState; 

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

        if (!currentNode->right) {std::get<RIGHT_VISITED>(stackTop) = true;}
        if (!currentNode->left) {std::get<LEFT_VISITED>(stackTop) = true;}

        // node has no children
        if (!currentNode->left && !currentNode->right) {
            std::cout << currentNode->value << " "; 
            nodes.pop_back(); 
        } else {
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
            // we've visted right and NOT left
            if (rightVisited && !leftVisited) {
                // visit the current node
                std::cout << currentNode->value << " "; 
            } 

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
                std::get<1>(stackTop) = true; 

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
                std::get<2>(stackTop) = true; 

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

//-------------------------------------------------------------------------------- 
// Name: BalanceBinaryTree
// Desc: 
//--------------------------------------------------------------------------------
void BalanceBinaryTree(BinaryNode& rootNode) {
    // make a list of all the values O(n)
    // sort the list O(n log n)
    // find the middle value O(1)
    // build tree O(n log n)
}

bool IsBalanced(BinaryNode& rootNode) {
    // both sub trees have +-1 nodes???
    return false; 
}

bool IsComplete(BinaryNode& rootNode) {
    return false; 
}

bool IsFull(BinaryNode& rootNode) {
    return false; 
}

bool IsPerfect(BinaryNode& rootNode) {
    // both subtrees have same number of nodes and levels
    return false;
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

    */

    // 15 12 10 9 8 4
    VisitInOrder(node); 

    // 10 8 4 9 12 15
    PreOrderVisit(node);

    // 4 9 8 15 12 10
    PostOrderVisit(node);

    return 0; 
}
// Trees.cpp
#include <memory>
#include <iostream>
#include <stack>
#include <tuple>
#include <vector>
#include <list>
#include <math.h>

// BinaryNode
struct BinaryNode {
    int value;

    BinaryNode* left;
    BinaryNode* right;

    bool HasChildren() { return this->left || this->right; }

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

//-------------------------------------------------------------------------------- 
// Name:
// Desc: 
//--------------------------------------------------------------------------------
int main() {
    // BinaryNode node {10, nullptr, nullptr}; 

    // BinaryInsert(12, node); 
    // BinaryInsert(15, node); 
    // BinaryInsert(8, node); 
    // BinaryInsert(9, node); 
    // BinaryInsert(4, node); 

    // // tree should look like
    // /*
    //          10
    //         /  \
    //       8      12
    //     /  \       \
    //    4    9       15

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

      

    uint32_t array2[] = {2, 4, 6, 8, 20}; 
    BinaryNode root2; 
    MinimalTreeRecursive(root2, array2, 5); 

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
    PrintTree(root2); 

    return 0; 
}
// Misc questions
// mostly taken from interviews that I've actually done

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include <iterator>

// Print pyramid numbers
// 1
// 2*3
// 4*5*6
// 7*8*9*10
// 7*8*9*10
// 4*5*6
// 2*3
// 1

// Kinda tempting to solve this with one loop but remember that nesting another loop in this case doesnt mean you're doing
// any more operations its's still O(width!) operations 
//
// Could possibly combine loops into one and have an ascending/descending state
void PrintPyramid(unsigned int width) {
    
    unsigned int counter = 1;

    for(auto w = 0; w <= width; w++) {

        for (auto i = 0; i < w; i++) { 
            std::cout << counter; 
            if (counter > 1 && i < (w - 1)) {
                std::cout << "*";
            }

            counter++;
        }

        std::cout << "\n"; 
    }

    for(auto w = width; w > 0; w--) {
        counter -= w; 
        for (auto i = 0; i < w; i++) { 
            std::cout << counter; 
            if (counter > 1 && i < (w-1)) {
                std::cout << "*";
            }

            counter++;
        }
        
        counter -= w; 
        std::cout << "\n"; 
    }

    std::cout << "\n";
}

// complicated with one loop
// actually more lines of code than the first and way more branching so this is probably worse
void PrintPyramid2(unsigned int width) {
    
    unsigned int counter = 1;
    bool ascending = true; 
    unsigned int w = 0;

    while ((ascending && w <= width) 
    || (!ascending && w > 0)) {
    
        if (ascending) {
            for (auto i = 0; i < w; i++) { 
                std::cout << counter; 
                if (counter > 1 && i < (w - 1)) {
                    std::cout << "*";
                }

                counter++;
            }
        } 
        
        if (w == width) { ascending = false; std::cout << "\n"; }

        if (!ascending) {
            counter -= w; 
            for (auto i = 0; i < w; i++) { 
                std::cout << counter; 
                if (counter > 1 && i < (w-1)) {
                    std::cout << "*";
                }

                counter++;
            }
            
            counter -= w; 
        }

        std::cout << "\n"; 

        
        if (ascending) { w++; } 
        else { w--; }
    }

    std::cout << "\n";
}

void PrintPyramid3(unsigned int width) {
    
    std::list<std::string> pyramid;
    unsigned int counter = 1;

    for(auto w = 0; w <= width; w++) {
        std::string line; 
        for (auto i = 0; i < w; i++) { 
            line += std::to_string(counter); 
            if (counter > 1 && i < (w - 1)) {
                line += "*";
            }

            counter++;
        }

        pyramid.push_back(line); 
    }   

    // NOTE: reverse iterating uses ++it!!!
    for (auto it = pyramid.rbegin(); it != pyramid.rend(); ++it) {
        pyramid.push_back(*it); 
    }

    for (auto s : pyramid) {
        std::cout << s << "\n"; 
    }
}

//
//
//
//
std::string LongestSubstring(const std::string& strA, const std::string& strB) {
    std::unordered_map<char, std::vector<unsigned int>> characterMap; 

    for (auto i = 0; i < strB.length(); i++) {
        auto c = strB[i]; 
        characterMap[c].push_back(i); 
    }

    auto startIndex = 0;
    auto longestSubStringLen = 0;

    for (auto j = 0; j < strA.length(); j++) {
        auto c = strA[j]; 

        // debug
        // std::cout << c << ": "; 

        // find c in our hashmap
        if (characterMap.find(c) != characterMap.end()) {
            
            auto indices = characterMap[c]; 
            for (auto i : indices) {
                // debug
                // std::cout << i << " ";
                auto currentSubStringLength = 0;        
                for (auto k = 0; (k + j) < strA.length() && (k + i) < strB.length(); k++) {

                    if (strA[j + k] == strB[i + k]) {
                        currentSubStringLength++; 
                        
                        if (currentSubStringLength > longestSubStringLen) {
                            longestSubStringLen = currentSubStringLength; 
                            startIndex = j; 
                        }
                    } else {
                        break; 
                    }
                }
            }
        }
    }

    return strA.substr(startIndex, longestSubStringLen); 
}

//------------------------------------------------------------------------------------------------------------
// Name: main
//------------------------------------------------------------------------------------------------------------
int main() {
    std::cout << "PrintPyramid\n";
    PrintPyramid(4); 

    std::cout << "PrintPyramid2\n";
    PrintPyramid2(4);

    std::cout << "PrintPyramid3\n";
    PrintPyramid3(4);

    std::cout << "LongestSubstring\n"; 
    std::cout << "abc dbc\n";
    std::cout << LongestSubstring("abc", "dbc") << "\n"; 

    std::cout << "function fun\n"; 
    std::cout << LongestSubstring("function", "fun") << "\n"; 

    std::cout << "abcdefghijkl dfsdfdcvdfdefghsdasr\n"; 
    std::cout << LongestSubstring("abcdefghijkl", "dfsdfdcvdfdefghsdasr") << "\n"; 

    return 0; 
}


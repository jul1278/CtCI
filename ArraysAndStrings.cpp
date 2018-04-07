#include <string>
#include <unordered_set>
#include <iostream>

// ArraysAndStrings.cpp

// 1.1 
// Implement an algorithm to determine if a string has all unique characters. What if you cannot use additional data structures?

// using hashset
bool IsUnique1(const std::string& str) {

    if (str.length() == 0) { return true; }

    std::unordered_set<char> set; 

    for (auto c : str) {
        if (set.find(c) != set.end()) {return false;}
        else {set.insert(c);}
    }

    return true; 
}
// not using any data structures 
// Do we need to handle a character being '\0'?
bool IsUnique2(const std::string& str) {
    if (str.length() == 0) { return true; }

    char table[0xff]; 
    memset(table, 0, sizeof(table));

    for (auto c : str) {
        if (table[c]) {return false;} 
        else {table[c] = c;}
    }

    return true; 
}

// 1.2 
// Given two strings, write an algorithm to decide if one is a permuation of the other
// if the strings have the same characters and the same frequency of each character then they are permutations
bool CheckPermutation(const std::string& a, const std::string& b) {

    if (a.length() != b.length()) { return false; }

    // O(n) solution
    // memory O(1)

    // NOTE: need to use larger int type if the string is really big
    // NOTE: only works with ASCII characters, I'd assume
    char tableA[0xff]; memset(tableA, 0, sizeof(tableA)); 
    char tableB[0xff]; memset(tableB, 0, sizeof(tableB));

    for (auto c : a) {
        tableA[c]++; 
    }

    for(auto c : b) {
        tableB[c]++;
    }

    return memcmp(tableA, tableB, 0xff) == 0; 
}

// 1.3
// Write a method to efficiently replace all spaces in a string with '%20' You may assume that the string has sufficient space
// at the end to hold the additional characters and that you are given the "true" length of the string (what?). 
void URLify(std::string& str) {

    if (str.empty()) {
        return; 
    }

    const std::string spaceCode = "%20"; 
    auto codeLen = spaceCode.length(); 

    for (auto i = 0; i < str.length(); i++) {
        if (str[i] == ' ') {
            str.insert(i, spaceCode); 
            i += codeLen; 
        }
    }
}

// 1.4
// Given a string, write a function to check if it is a permutation of a palindrome. 
bool IsPermutationOfPalindrome(const std::string& str) {
    // lets say that a string of length 1 or 0 is not a palindrome
    if (str.length() < 2) { return false; }

    // if the string has an odd number of characters there must be an even frequency of each character except for one character
    // if the string length is even then there must be an even number of all characters

    // this is O(n) solution 
    // memory O(1)

    // NOTE: need bigger int for really big strings
    char table[0xff]; memset(table, 0, sizeof(table)); 

    for (auto c : str) {
        if (c == ' ') {continue;}
        table[c]++;
    }

    if (str.length() % 2) {
        unsigned int oddFreq = 0; 
        for (auto c : table) {
            if (c % 2) { oddFreq++; }
            if (oddFreq > 1) {return false;}
        }
    } else {
        for (auto c : table) {
            if (c % 2) {return false;}
        }
    }

    return true; 
}

//------------------------------------------------------------------------------------------------------
// Name:
// Desc: 
//------------------------------------------------------------------------------------------------------
int main() {

//------------------------------------------------------------------------------------------------------

    std::string unique1 = "abcdefg";
    std::string unique2 = "abcdafg";
    std::string unique3 = "aaaaaaa";

    std::cout << " ---- IsUnique1() ---- \n"; 

    if (IsUnique1(unique1)) {
        std::cout << unique1 << " is unique\n"; 
    } else {
        std::cout << unique1 << " is not unique\n"; 
    }

    if (IsUnique1(unique2)) {
        std::cout << unique2 << " is unique\n"; 
    } else {
        std::cout << unique2 << " is not unique\n"; 
    }

    if (IsUnique1(unique3)) {
        std::cout << unique3 << " is unique\n"; 
    } else {
        std::cout << unique3 << " is not unique\n"; 
    }

//------------------------------------------------------------------------------------------------------
    std::cout << " ---- IsUnique2() ---- \n"; 

    if (IsUnique2(unique1)) {
        std::cout << unique1 << " is unique\n"; 
    } else {
        std::cout << unique1 << " is not unique\n"; 
    }

    if (IsUnique2(unique2)) {
        std::cout << unique2 << " is unique\n"; 
    } else {
        std::cout << unique2 << " is not unique\n"; 
    }

    if (IsUnique2(unique3)) {
        std::cout << unique3 << " is unique\n"; 
    } else {
        std::cout << unique3 << " is not unique\n"; 
    }

//-------------------------------------------------------------------------------------------------------

    std::cout << " ---- CheckPermutation() ---- \n";

    std::string a = "abcdefg"; 
    std::string b = "gbdfcea";
    std::string c = "sffeced"; 

    if (CheckPermutation(a, b)) {
        std::cout << a << " & " << b << " are permutations\n"; 
    } else {
        std::cout << a << " & " << b << " are not permutations\n"; 
    }

    if (CheckPermutation(a, c)) {
        std::cout << a << " & " << c << " are permutations\n"; 
    } else {
        std::cout << a << " & " << c << " are not permutations\n"; 
    }

    return 0; 
}
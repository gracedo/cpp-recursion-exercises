/**
* File: generate-mnemonics.cpp
* ----------------------------
* This is an application that compiles phone numbers (or, more generally,
* numbers) to mnemonics, where the digit-to-letter mappings are consistent
* with those listed on a traditional telephone keypad.
*/
 
#include <string>    // for string
#include <iostream>  // for cout, endl
#include "console.h"
#include "simpio.h"  // for getLine
#include "vector.h"  // for the Vector class template
#include "map.h"     // for the Map class template
using namespace std;
 
/* Function Protytypes */
static void buildMappings(Map<char, string> &mappings);
static bool isAllPositiveDigits(const string &response);
static string getPhoneNumber();
static void generateMnemonics(const string &number, Map<char, string> &mappings, Vector<string> &mnemonics, string word);
 
/* Main Program */
int main() {
  Map<char, string> mappings;
  buildMappings(mappings);
  while (true) {
    string number = getPhoneNumber();
    if (number.empty()) break;
    Vector<string> mnemonics;
    string word = "";
    generateMnemonics(number, mappings, mnemonics, word);
    cout << "These are the possible mnemonics: " << endl << endl;
    int count = 0;
    foreach (string mnemonic in mnemonics) {
      cout << "  " << mnemonic;
      count = (count + 1) % 9;
      if (count % 9 == 0) cout << endl;
    }
    cout << endl;
  }
  return 0;
}
 
/* Creates the map of letters correlated to the numbers */
static void buildMappings(Map<char, string>& mappings) {
  const string options[] = {
    "ABC", "DEF", "GHI", "JKL",
    "MNO", "PQRS", "TUV", "WXYZ"
  };
     
  for (int i = 0; i < sizeof(options)/sizeof(options[0]); i++) {
    mappings['2' + i] = options[i];
  }
}
 
/* Checks if the input digits are valid */
static bool isAllPositiveDigits(const string& response) {
  for (int i = 0; i < response.size(); i++) {
    if (response[i] < '2' || response[i] > '9') {
      return false;
    }
  }
     
  return true;
}
 
/* Prompts user for the number input */
static string getPhoneNumber() {
  while (true) {
    string response = getLine("Enter a string of digits [or hit <enter> if you're done]: ");
    if (isAllPositiveDigits(response)) return response;
    cout << "Whatever you enter, make sure it includes only digit characters in ['2', '9'].  ";
    cout << "Please try again." << endl;
  }
}
 
/* Recursively generates all possible letter combinations */
static void generateMnemonics(const string &number, Map<char, string> &mappings, Vector<string> &mnemonics, string word) {
  if(number.size() == 0) { //base case
    mnemonics.add(word);
    return;
  }
     
  string letters = mappings[number[0]];
  string newNumber = number.substr(1, number.size()-1); //remove first number
  foreach(char ch in letters) {
    string tempWord = word + ch;
    generateMnemonics(newNumber, mappings, mnemonics, tempWord);
  }
}
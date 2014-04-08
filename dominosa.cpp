/**
* File: dominosa.cpp
* ------------------
* This animates the brute-force discovery of a solution
* to a 2 x n dominosa board.
*/
 
#include <iostream>
#include <cmath>
#include "simpio.h"
#include "grid.h"
#include "random.h"
#include "map.h"
#include "set.h"
#include "dominosa-graphics.h"
using namespace std;
 
/* Function Prototypes */
static void welcome();
static int getIntegerInRange(string prompt, int low, int high);
static void populateBoard(Grid<int>& board, int low, int high);
static bool canSolveBoard(DominosaDisplay &display, Grid<int> &board);
bool canPair(DominosaDisplay &display, Grid<int> &board, coord &nextCoord, Map<int, Set<int> > &seenPairs);
 
/* Main Program */
int main() {
  DominosaDisplay display;
  welcome();
  while(true) {
    int numColumns = getIntegerInRange("How many columns? [0 to exit]: ", 9, 25);
    if(numColumns == 0) break;
    Grid<int> board(2, numColumns);
    populateBoard(board, 1, ceil(2 * sqrt((double) numColumns)));
    display.drawBoard(board);

    if(canSolveBoard(display, board)) {
      cout << "The board can be solved, and one such solution is drawn above." << endl;
    } else {
      cout << "This board you see can't be solved." << endl;
    }
  }
   
  cout << "Okay, thanks for watching, and come back soon." << endl;
  cout << "Click the mouse anywhere in the window to exit." << endl;
  return 0;
}

/* Prints the welcome message */
static void welcome() {
  cout << "Here we'll illustrate the use of recursive backtracking to" << endl;
  cout << "discover a solution to various 2 x n Dominosa boards.  In some" << endl;
  cout << "cases there won't be any solutions, and in the cases where there are" << endl;
  cout << "multiple solutions, we'll just find one of them." << endl;
  cout << endl;
}

/* Prompts user for column dimension of board */
static int getIntegerInRange(string prompt, int low, int high) {
  while(true) {
    int response = getInteger(prompt);
    if(response == 0 || (response >= low && response <= high)) return response;
    cout << "Sorry, but I can't use that number." << endl;
  }
}

/* Populates the board with random integers */
static void populateBoard(Grid<int> &board, int low, int high) {
  for(int row = 0; row < board.numRows(); row++) {
    for(int col = 0; col < board.numCols(); col++) {
      board[row][col] = randomInteger(low, high);
    }
  }
}

/* Wrapper function for canPair function */
static bool canSolveBoard(DominosaDisplay &display, Grid<int> &board) {
  Map<int, Set<int> > seenPairs;
  coord startCoord;
  startCoord.row = 0;
  startCoord.col = 0;
  
  return canPair(display, board, startCood, seenPairs);
}

/* Recursively tries every possible pair */
bool canPair(DominosaDisplay &display, Grid<int> &board, coord &nextCoord, Map<int, Set<int> > &seenPairs) {
  int num1, num2;
  coord checkCoord, tempNextCoord;

  if(nextCoord.col > board.numCols()-1) return true; //base case

  if(nextCoord.row == 0) { //checks vertical pairs
    checkCoord.row = nextCoord.row + 1;
    checkCoord.col = nextCoord.col; //vertical
    display.provisonallyPair(nextCoord, checkCoord);
    num1 = board[nextCoord.row][nextCoord.col];
    num2 = board[checkCoord.row][checkCoord.col];
    if(seenPairs[num2].contains(num1)) { //checks if seenPairs includes this pair of numbers already
      display.vetoProvisionalPairing(nextCoord, checkCoord);
    } else { //if seenPairs does not include this number pair, add the pair into the map
      if(seenPairs.containsKey(num1)) seenPairs[num1].add(num2); //if key exists, add value to the set mapped to key
      else { //if key doesn't exist, create key and create set with value mapped to key
        Set<int> set;
        set.add(num2);
        seenPairs.put(num1, set);
      }
      if(seenPairs.containsKey(num2)) seenPairs[num2].add(num1); //vice versa because order doesn't matter for pairs
      else {
        Set<int> set;
        set.add(num1);
        seenPairs.put(num2, set);
      }
      tempNextCoord.row = nextCoord.row;
      tempNextCoord.col = nextCoord.col + 1;
      if(canPair(display, board, tempNextCoord, seenPairs)) {
        display.certifyPairing(nextCoord, checkCoord);
        return true;
      } else {
        seenPairs[num1].remove(num2);
        seenPairs[num2].remove(num1);
      }
    }
    
    display.eraseProvisionalPairing(nextCoord, checkCoord);
  }

  //we only get here if vertical path fails
  if(nextCoord.col == board.numCols()-1) return false;
  else { //checks horizontal pairs
    checkCoord.row = nextCoord.row;
    checkCoord.col = nextCoord.col + 1; //horizontal
    display.provisonallyPair(nextCoord, checkCoord);
    num1 = board[nextCoord.row][nextCoord.col];
    num2 = board[checkCoord.row][checkCoord.col];
    if(seenPairs[num2].contains(num1)) {
      display.vetoProvisionalPairing(nextCoord, checkCoord);
      display.eraseProvisionalPairing(nextCoord, checkCoord);
      return false;
    } else {
      if(seenPairs.containsKey(num1)) seenPairs[num1].add(num2);
      else {
        Set<int> set;
        set.add(num2);
        seenPairs.put(num1, set);
      }
      if(seenPairs.containsKey(num2)) seenPairs[num2].add(num1);
      else {
        Set<int> set;
        set.add(num1);
        seenPairs.put(num2, set);
      }
      if(nextCoord.row == 0) {
        tempNextCoord.row = nextCoord.row + 1;
        tempNextCoord.col = nextCoord.col;
      } else {
        tempNextCoord.row = nextCoord.row - 1;
        tempNextCoord.col = nextCoord.col + 2;
      }
      if(canPair(display, board, tempNextCoord, seenPairs)) {
        display.certifyPairing(nextCoord, checkCoord);
        return true;
      } else {
        display.eraseProvisionalPairing(nextCoord, checkCoord); 
        return false;
      }
    }
  }
}
/**
* File: trees.cpp
* ---------------
* Draws a recursive tree.
*/
 
#include <string>    // for string
#include <iostream>  // for cout, endl
#include "console.h" // required of all CS106 C++ programs
#include "gwindow.h" // for GWindow class and its setTitle, setColor, and drawPolarLine methods
#include "gtypes.h"  // for GPoint class
#include "random.h"  // for randomChance function
using namespace std;
 
/* Constants */
const static double kWindowWidth = 600;
const static double kWindowHeight = 600;
const static string kWindowTitle = "Recursive Trees";
const static double kTrunkLength  = kWindowHeight/4;
const static double kShrinkFactor = 0.70;
const static int kBranchAngleSeparation = 15;
const static int kTrunkStartAngle = 90;
const static string kLeafColor = "#2e8b57";
const static string kTrunkColor = "#8b7765";
const static double kBranchProbability = 1.0;
const static int kHighestOrder = 5;
 
/* Function Prototypes */
static void drawTree(GWindow &window, int order);
void drawBranch(GWindow& window, int order, GPoint pt, double length, double angle);
 
/* Main Program */
int main() {
  GWindow window(kWindowWidth, kWindowHeight);
  window.setWindowTitle(kWindowTitle);
  cout << "Repeatedly click the mouse in the graphics window to draw " << endl;
  cout << "recursive trees of higher and higher order." << endl;
  for (int order = 0; order <= kHighestOrder; order++) {
    waitForClick();
    window.clear();
    drawTree(window, order);
  }
     
  cout << endl
  cout << "All trees through order " << kHighestOrder << " have been drawn." << endl;
  cout << "Click the mouse anywhere in the graphics window to quit." << endl;
  waitForClick();
  return 0;
}
 
/* Wrapper function for drawBranch */
static void drawTree(GWindow& window, int order) {
  GPoint trunkBase(window.getWidth()/2, window.getHeight());
  drawBranch(window, order, trunkBase, kTrunkLength, kTrunkStartAngle);
}
 
/* Recursively draws the branches */
void drawBranch(GWindow& window, int order, GPoint pt, double length, double angle) {
  window.setColor(order < 2 ? kLeafColor : kTrunkColor);
  pt = window.drawPolarLine(pt, length, angle);
  if(order > 0) {
    for(int i = -3; i <= 3; i++) {
      if(randomChance(kBranchProbability)) drawBranch(window, order-1, pt, length*kShrinkFactor, angle + i * kBranchAngleSeparation);
    }
  }
}
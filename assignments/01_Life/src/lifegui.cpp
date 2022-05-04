/*
 * CS 106B, Homework 1, Game of Life
 * Implementation of the Life graphics support routines.  We have some
 * internal private helpers that do calculations for cell-size, 
 * colors, and "aging" the cells using shading. You are
 * welcome to read over this code, but you should not need to edit it.
 * See lifegui.h for documentation of each member function.
 * 
 * This is based on a previous implementation by Julie Zelenski,
 * with later modifications by Cynthia Lee + TAs.
 */

#include <sstream>  // for ostringstream
#include <iomanip>  // for setw, setfill
#include <ios>      // for hex stream manipulator
#include "random.h" // for randomInteger
#include "strlib.h" // for integerToString
#include "error.h"  // for error
#include "lifegui.h"
using namespace std;

static const int kMaxAge = 3;  // maximum age before color fading ends

const string LifeGUI::kDefaultWindowTitle("CS 106B Game of Life");

LifeGUI::LifeGUI() : window(kDisplayWidth, kDisplayHeight) {
    initializeColors();
    window.setVisible(true);
    window.setWindowTitle(kDefaultWindowTitle);
    // window.setExitOnClose(true); // uncomment to exit program on close
}

LifeGUI::~LifeGUI() {
    window.close();
}

void LifeGUI::resize(int numRows, int numColumns) {
    if (numRows <= 0 || numColumns <= 0) {
        error("LifeGUI::setDimensions number of rows and columns must both be positive!");
    }
    
    this->numRows = numRows;
    this->numColumns = numColumns;
    ages.resize(numRows, numColumns);
    computeGeometry();
    window.clear();
    window.setColor("Black");
    window.drawRect(max(0, upperLeftX - 1), max(0, upperLeftY - 1),
                    numColumns * cellDiameter + 2, numRows * cellDiameter + 2);
}

void LifeGUI::setTitle(string title) {
    window.setWindowTitle(title);
    windowTitle = title;
}

void LifeGUI::drawCell(int row, int column, bool alive) {
    if (!coordinateInRange(row, column)) {
        error(string(__FUNCTION__) + " asked to draw location (" +
              integerToString(row) + ", " + integerToString(column) +
              ") which is outside the bounds of the current simulation.");
    }
    
    int age = 0;
    if (alive) {
        age = 1;
    } else if (ages[row][column] > 0) {
        age = ages[row][column] + 1;
    }
    if (age > kMaxAge) {
        age = 0;
    }

    window.setColor(colors[age]);
    if(age == 0) {
        window.fillRect(upperLeftX + column * cellDiameter + 1, upperLeftY + row * cellDiameter + 1,
                        cellDiameter - 2, cellDiameter - 2);
    } else {
        window.fillOval(upperLeftX + column * cellDiameter + 1, upperLeftY + row * cellDiameter + 1,
                        cellDiameter - 2, cellDiameter - 2);
    }
    ages[row][column] = age;
}

int LifeGUI::scalePrimaryColor(int baseContribution, int age) const {
    const int maxContribution = 220;
    int remaining = maxContribution - baseContribution;
    return baseContribution + double(age) * remaining / kMaxAge;
}

void LifeGUI::initializeColors() {
    colors.add("White"); // colors[0] is used for age 0, and is always white
    int baseColor[] = {
        // randomInteger(0, 192), randomInteger(0, 192), randomInteger(0, 192)

        // default to a bluish color
        30, 30, 220
    };
    
    for (int age = 1; age <= kMaxAge; age++) {
        ostringstream oss;
        oss << "#";
        for (int primary = 0; primary < 3; primary++) {
            int contribution = scalePrimaryColor(baseColor[primary], age);
            oss << setw(2) << setfill('0') << hex << contribution;
        }
        colors.add(oss.str());
    }
}

void LifeGUI::computeGeometry() {
 	int hPixelsPerCell = window.getHeight()/numRows;
	int wPixelsPerCell = window.getWidth()/numColumns;
	cellDiameter = min(wPixelsPerCell, hPixelsPerCell);
	upperLeftX = max(1, (kDisplayWidth - numColumns * cellDiameter)/2);
	upperLeftY = max(1, (kDisplayHeight - numRows * cellDiameter)/2);
}

bool LifeGUI::coordinateInRange(int row, int column) const {
    return row >= 0 && row < numRows && column >= 0 && column < numColumns;
}

void LifeGUI::printBoard() {
    std::cout << windowTitle << std::endl;
    for(int i = 0; i < numRows; ++i) {
        for(int j = 0; j < numColumns; ++j) {
            std::cout << setw(3) << setfill(' ') << ages[i][j];
        }
        std::cout << std::endl;
    }
}

/*
 * CS 106B, Homework 1, Game of Life
 * Defines a display class needed for the Game of Life assignment.
 * Based on an earlier version written by Julie Zelenski,
 * with later modifications by Cynthia Lee + TAs.
 */

#ifndef _lifegui_h_
#define _lifegui_h_

#include <string>    // for std::string
#include "gwindow.h" // for GWindow
#include "vector.h"  // for Vector
#include "grid.h"    // for Grid

class LifeGUI {
public:
    /**
     * Constructs a Life window and makes it visible.
     */
    LifeGUI();
    
    /**
     * Hides the Life display window and then destroys it.
     */
    ~LifeGUI();
    
    /**
     * This will erase the graphics window completely and draw a black
     * border around the simulation rectangle which is centered in the
     * window.  The grid cells will be sized as large as will fit given
     * the grid geometry. Grids with more rows and columns will use smaller
     * cells. This function can be used at the beginning of a simulation or 
     * between generations to clear the window before drawing the next generation.
     */
    void resize(int rows, int cols);
    
    /**
     * Draws the cell at the specific row and column, replacing any previously
     * drawn cell at that location.  Rows and columns are specified using zero-based
     * indexing and (0,0) is the upper-left corner. If the location given is not in bounds, 
     * an error is thrown.
     *
     * At startup, a random color is chosen for the simulation.  Each cell
     * will be displayed in a shade which tells its age.  Cells that have just
     * been born (i.e. that have value 1) are the darkest, they get lighter with
     * age as the values go to 2, 3, and so on.  The cells stabilize as very faint
     * at generation MaxAge and older.  Passing 0 for age means the cell is not alive,
     * the cell is drawn in white, erasing any previous contents.
     */
    void drawCell(int row, int column, bool alive);
    
private:
    GWindow window;
    int numRows;
    int numColumns;
    int upperLeftX;
    int upperLeftY;
    int cellDiameter;
    Vector<std::string> colors;
    std::string windowTitle;
    Grid<int> ages;
    
    static const std::string kDefaultWindowTitle;
    static const int kDisplayWidth = 10 * 72; // 10 inches
    static const int kDisplayHeight = 7 * 72; // 7 inches
    
    void initializeColors();
    int scalePrimaryColor(int baseContribution, int age) const;
    void computeGeometry();
    bool coordinateInRange(int row, int column) const;
    
    LifeGUI(const LifeGUI& original);
    void operator=(const LifeGUI& rhs) const;

    /**
     * Updates the title of the window to specified string.
     */
    void setTitle(std::string title);

    /**
     * Prints the current board with ages. Used for debugging and for
     * text-only versions of the program.
     *
     * Example output:
     *                    Game of Life
     *                    1  2  1  0  0
     *                    0  1  0  0  0
     *                    0  0  0  0  0
     *                    0  0  0  0  4
     */
    void printBoard();
};

#endif 

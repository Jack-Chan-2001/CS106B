/*
 * CS 106B/X Fractal Recursion Problems
 * This instructor-provided file declares a class representing the overall
 * graphical user interface (GUI) for this part of the program.
 * See fractalgui.cpp for the implementation of each member.
 *
 * You don't need to modify this file.
 * Your recursive functions should work with an unmodified version of this file.
 *
 * @author Marty Stepp
 * @version 2016/10/16
 * - modified for 16au with additional fractals (tree)
 * @version 2015/04/24
 * - modified for 15sp version, including exception-handling
 * @version 2014/10/13
 * - modified for 14au version, including new version of floodFill
 * @version 2017/04/19
 * - took out flood fill, added mandelbrot set
 */

#ifndef _fractalgui_h
#define _fractalgui_h

#include <string>
#include "gevents.h"
#include "ginteractors.h"
#include "gobjects.h"
#include "gwindow.h"
#include "map.h"

class FractalGUI {
public:
    /*
     * Removes all shapes from the screen.
     */
    void clear();

    /*
     * The GUI's main loop that waits for events and handles them.
     */
    void eventLoop();

    /*
     * Creates and configures all graphical controls in the GUI.
     */
    void initialize();

private:
    /*
     * Initiates a draw Sierpinski triangle operation on the window.
     */
    void doSierpinski();

    /*
     * Initiates a recursive tree operation on the window.
     */
    void doTree();

    /*
     * Returns a map of available colors for mandelbrot set.
     */
    const Map<std::string, int>& getColorMap();

    /*
     * Processes one action event (button click) in the GUI window.
     */
    void handleActionEvent(GActionEvent& actionEvent);

    /*
     * Processes one mouse event in the GUI window.
     */
    void handleMouseEvent(GMouseEvent& mouseEvent);

    /*
     * Returns true if all text fields have valid integer coordinates entered into them.
     */
    bool hasAllCoords();

    /*
     * Sets the bottom status bar text to display in the southern area.
     * If isError is true, shows it in a red error color.
     */
    void setStatusText(const std::string& text, bool isError = false);

    /*
     * Scales the mandelbrot set based on zoom factor and where
     * the user clicked
     */
    void scale(int x, int y, double zoomFactor);

    // constants
    static const int WINDOW_WIDTH = 700;
    static const int WINDOW_HEIGHT = 550;
    static const int DRAG_RECT_COLOR = 0xaa8822;

    // overall window
    GWindow* window;

    // west buttons to draw fractals
    GButton* sierpinskiButton;
    GButton* treeButton;
    GButton* mandelbrotButton;
    GButton* diffButton;
    GButton* clearButton;

    // mandelbrot colors drop-down
    GChooser* colorChooser;

    // north text fields for figure parameters
    GTextField* xField;
    GTextField* yField;
    GTextField* sizeField;
    GTextField* orderField;
    GCheckBox* animationBox;

    // south status bar
    GLabel* statusBar;

    // temporary dragging rectangle for figure size
    GRect* dragRect;

    // temporary flags to remember if we are in midst of a mandelbrot set
    bool mandelbrotFillMode = false;
    bool mandelbrotFillInProgress = false;

    // x and y coordinates and zoom for mandelbrot
    double minMandelbrotX;
    double minMandelbrotY;
    double mandelbrotIncX;
    double mandelbrotIncY;
    double mandelbrotZoom;
};

#endif // _fractalgui_h

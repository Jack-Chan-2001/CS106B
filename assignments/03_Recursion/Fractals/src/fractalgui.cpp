/*
 * CS 106B/X Fractal Recursion Problems
 * This instructor-provided file defines the members of a class representing the
 * overall graphical user interface (GUI) for this part of the program.
 * See fractalgui.h for the class and member declarations.
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
 * - removed flood fill, added mandelbrot set
 */

#include "fractalgui.h"
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include "filelib.h"
#include "gfilechooser.h"
#include "grid.h"
#include "random.h"
#include "strlib.h"
#include "timer.h"
#include "fractals.h"

void FractalGUI::clear() {
    window->clearCanvas();
    if (dragRect) {
        delete dragRect;
        dragRect = nullptr;
    }
}

void FractalGUI::doSierpinski() {
    if (!hasAllCoords()) {
        setStatusText("Error: You must set an x, y, size, and order first.", /* isError */ true);
        return;
    }
    mandelbrotFillMode = false;
    int x = xField->getValueAsInteger();
    int y = yField->getValueAsInteger();
    int size = sizeField->getValueAsInteger();
    int order = orderField->getValueAsInteger();
    std::string statusText = "Sierpinski triangle: x=" + integerToString(x)
            + " y=" + integerToString(y)
            + " size=" + integerToString(size)
            + " order=" + integerToString(order);
    setStatusText(statusText + " ...");
    window->setColor(0x0);   // black

    Timer timmy;
    timmy.start();

    drawSierpinskiTriangle(*window, x, y, size, order);

    long elapsed = timmy.stop();
    statusText += " (" + longToString(elapsed) + "ms)";
    setStatusText(statusText);
}

void FractalGUI::doTree() {
    if (!hasAllCoords()) {
        setStatusText("Error: You must set an x, y, size, and order first.", /* isError */ true);
        return;
    }
    mandelbrotFillMode = false;
    int x = xField->getValueAsInteger();
    int y = yField->getValueAsInteger();
    int size = sizeField->getValueAsInteger();
    int order = orderField->getValueAsInteger();

    std::string statusText = "Recursive tree: x=" + integerToString(x)
            + " y=" + integerToString(y)
            + " size=" + integerToString(size)
            + " order=" + integerToString(order);
    setStatusText(statusText + " ...");
    window->setColor(0x0);   // black; drawTree should change it

    Timer timmy;
    timmy.start();

    drawTree(*window, x, y, size, order);

    long elapsed = timmy.stop();
    statusText += " (" + longToString(elapsed) + "ms)";
    setStatusText(statusText);
}

void FractalGUI::eventLoop() {
    while (true) {
        GEvent event = waitForEvent(ACTION_EVENT | MOUSE_EVENT | WINDOW_EVENT);
        bool animated = animationBox->isChecked();
        window->setRepaintImmediately(animated);

        if (event.getEventClass() == ACTION_EVENT) {
            GActionEvent actionEvent(event);
            handleActionEvent(actionEvent);
        } else if (event.getEventClass() == MOUSE_EVENT) {
            GMouseEvent mouseEvent(event);
            handleMouseEvent(mouseEvent);
        } else if (event.getEventClass() == WINDOW_EVENT) {
            if (event.getEventType() == WINDOW_CLOSED) {
                break;
            }
        }

        if (!animated) {
            window->repaint();
        }
    }
}

const Map<std::string, int>& FractalGUI::getColorMap() {
    static Map<std::string, int> colorMap;
    if (colorMap.isEmpty()) {
        colorMap["Red"]    = 0x8c1515;   // Stanford red
        colorMap["Yellow"] = 0xeeee00;   // yellow
        colorMap["Blue"]   = 0x0000cc;   // blue
        colorMap["Green"]  = 0x00cc00;   // green
        colorMap["Purple"] = 0xcc00cc;   // purple
        colorMap["Orange"] = 0xff8800;   // orange
        colorMap["Palette"] = 0x0;       // use custom palette
    }
    return colorMap;
}


void FractalGUI::handleActionEvent(GActionEvent& actionEvent) {
    if (actionEvent.getSource() == sierpinskiButton) {
        doSierpinski();
    } else if (actionEvent.getSource() == treeButton) {
        doTree();
    } else if (actionEvent.getSource() == mandelbrotButton) {
        if (!actionEvent.isShiftKeyDown()) {
            //setStatusText("Flood fill: adding rectangles");
            clear();
            std::string sizeStr = sizeField->getText();
            if (sizeStr == "") {
                sizeField->setText("200");
            }
            std::string colorStr = colorChooser->getSelectedItem();
            int color = getColorMap()[colorStr];
            mandelbrotZoom = 1;

            minMandelbrotX = -2.5;
            mandelbrotIncX = 3.5 / window->getCanvasWidth();

            minMandelbrotY = -1;
            mandelbrotIncY = 2.0 / window->getCanvasHeight();

            mandelbrotSet(*window,minMandelbrotX,mandelbrotIncX,
                          minMandelbrotY,mandelbrotIncY, sizeField->getValueAsInteger(),color);
            statusBar->setText(statusBar->getText()+" (done)");
        }
        mandelbrotFillMode = true;
    } else if (actionEvent.getSource() == diffButton) {
        if (actionEvent.isShiftKeyDown()) {
            // save current window to an image (probably for teacher only)
            std::string filename = GFileChooser::showSaveDialog();
            if (!filename.empty()) {
                window->saveCanvasPixels(filename);
            }
        } else {
            // pop up DiffImage tool
            std::string filename = openFileDialog("Choose image to compare to", "output");
            if (!filename.empty()) {
                window->compareToImage(filename);
            }
        }
    } else if (actionEvent.getSource() == clearButton) {
        clear();
        mandelbrotFillMode = false;
        setStatusText("Graphics cleared.");
    }
}

/*
 * This is probably not decomposed as much as it should be; each kind of mouse
 * event should be in its own function.  Oh well.
 */
void FractalGUI::handleMouseEvent(GMouseEvent& mouseEvent) {
    int mx = (int) mouseEvent.getX();
    int my = (int) mouseEvent.getY();
    if (mouseEvent.getEventType() == MOUSE_MOVED) {
        if (!mandelbrotFillInProgress) {
            int px = window->getPixel(mx, my);
            std::string statusText = "(x="
                    + integerToString((int) mouseEvent.getX()) + ", y="
                    + integerToString((int) mouseEvent.getY()) + "), color="
                    + convertRGBToColor(px);
            setStatusText(statusText);
        }
    } else if (mouseEvent.getEventType() == MOUSE_PRESSED) {
        if (!mouseEvent.isLeftClick()) {
            if (dragRect) {
                window->remove(dragRect);
                delete dragRect;
                dragRect = nullptr;
                window->repaint();
                return;
            }
        }

        xField->setValue(mx);
        yField->setValue(my);

        if (mandelbrotFillMode) {
            // get new middle point based on our zoom level
            //mandelbrotZoom+=1;
            if (mouseEvent.isShiftKeyDown()) {
                scale(mx,my,0.5);
            } else {
                scale(mx,my,2);
            }
            std::string sizeStr = sizeField->getText();
            if (sizeStr == "") {
                sizeField->setText("200");
            }
            clear();
            std::string colorStr = colorChooser->getSelectedItem();
            int color = getColorMap()[colorStr];
            mandelbrotSet(*window,minMandelbrotX,mandelbrotIncX,
                          minMandelbrotY,mandelbrotIncY,sizeField->getValueAsInteger(),color);
            statusBar->setText(statusBar->getText()+" (done)");
        }
    } else if (mouseEvent.getEventType() == MOUSE_DRAGGED) {
        if (!mandelbrotFillMode) {
            int x = xField->getValueAsInteger();
            int y = yField->getValueAsInteger();
            int mx = (int) mouseEvent.getX();
            int my = (int) mouseEvent.getY();
            if (mx > x && my > y) {
                int size = std::min(mx - x, my - y);
                sizeField->setValue(size);
                if (dragRect) {
                    dragRect->setBounds(x, y, size, size);
                } else {
                    dragRect = new GRect(x, y, size, size);
                    dragRect->setColor(DRAG_RECT_COLOR);
                    // dragRect->setLineWidth(2);
                    window->add(dragRect);
                }
            }
        }
    } else if (mouseEvent.getEventType() == MOUSE_RELEASED) {
        if (!mandelbrotFillMode && dragRect) {
            window->remove(dragRect);
            delete dragRect;
            dragRect = nullptr;
        }
    }
}

bool FractalGUI::hasAllCoords() {
    return stringIsInteger(xField->getText())
            && stringIsInteger(yField->getText())
            && stringIsInteger(sizeField->getText())
            && stringIsInteger(orderField->getText());
}


/*
 * Helper to create one button in the GUI.
 */
static GButton* createButton(const std::string& cmd, const std::string& icon) {
    GButton* button = new GButton();
    button->setText(cmd);
    button->setTextPosition(SwingConstants::SWING_CENTER, SwingConstants::SWING_BOTTOM);
    button->setActionCommand(cmd);
    button->setTooltip(cmd);
    button->setIcon(icon);
    return button;
}

void FractalGUI::initialize() {
    // GObject::setAntiAliasing(false);
    mandelbrotFillMode = false;
    mandelbrotFillInProgress = false;
    dragRect = nullptr;

    // create window
    window = new GWindow(WINDOW_WIDTH, WINDOW_HEIGHT, /* visible */ false);
    window->setResizable(true);
    window->setTitle("CS 106X Fractals");

    // create controls
    sierpinskiButton = createButton("Sierpinski Triangle", "triangle-icon.png");
    treeButton = createButton("Recursive Tree", "tree-icon.png");
    mandelbrotButton = createButton("Mandelbrot Set", "mandelbrot-icon.png");
    diffButton = createButton("Compare Output", "diff-icon.png");
    clearButton = createButton("Clear", "recycle-icon.png");
    colorChooser = new GChooser();
    colorChooser->setTooltip("Fill Color");
    colorChooser->addItems(getColorMap().keys());
    statusBar = new GLabel(" ");
    animationBox = new GCheckBox("Animate?");
    animationBox->setChecked(true);
    xField = new GTextField(4);
    xField->setPlaceholder("x");
    yField = new GTextField(4);
    yField->setPlaceholder("y");
    sizeField = new GTextField(4);
    sizeField->setPlaceholder("size");
    orderField = new GTextField(4);
    orderField->setValue(1);
    orderField->setPlaceholder("order");

    // perform layout
    window->addToRegion(new GLabel("x"), "North");
    window->addToRegion(xField, "North");
    window->addToRegion(new GLabel("y"), "North");
    window->addToRegion(yField, "North");
    window->addToRegion(new GLabel("size"), "North");
    window->addToRegion(sizeField, "North");
    window->addToRegion(new GLabel("order"), "North");
    window->addToRegion(orderField, "North");
    window->addToRegion(animationBox, "North");

    window->addToRegion(sierpinskiButton, "West");
    window->addToRegion(treeButton, "West");
    window->addToRegion(mandelbrotButton, "West");
    window->addToRegion(colorChooser, "West");
    window->addToRegion(diffButton, "West");
    window->addToRegion(clearButton, "West");

    window->addToRegion(statusBar, "South");

    window->pack();
    window->setWidth(WINDOW_WIDTH);
    window->center();
    window->setVisible(true);
}

void FractalGUI::setStatusText(const std::string& text, bool isError) {
    statusBar->setColor(isError ? 0xbb0000 : 0x0);
    statusBar->setText(text);
}

void FractalGUI::scale(int x, int y, double zoomFactor) {
    mandelbrotZoom *= zoomFactor;
    // this function receives an x and y coordinate from the current
    // window, and updates the minMandelbrotX, mandelbrotIncX,
    // minMandelbrotY, and mandelbrotIncY
    // variables to scale for the next iteration.

    // x will become the x mid-point of the mandelbrot set
    // y will become the y mid-point of the mandelbrot set

    // an initial zoom level of 1 will have the following ranges:
    // scale y from -1 to 1, based on height
    // scale x from -2.5 to 1, based on width

    // a y of 0 should map to -1
    // a y of height should map to 1
    // range of 1 - -1 = 2

    double height = window->getCanvasHeight();
    double midH = height / 2.0;
    double yOffset = y - midH; // distance from middle

    // we need to shift the top down by the scaled value of where we clicked,
    // in other words, we shift by yOffset * mandelbrotIncY
    // and this will be the new top position
    minMandelbrotY += yOffset * mandelbrotIncY;

    // the middle stays the same when we zoom
    double middleY = minMandelbrotY + midH * mandelbrotIncY;

    // set the new increment factor
    mandelbrotIncY = 2.0 / height / mandelbrotZoom;

    minMandelbrotY = middleY - 2.0 / mandelbrotZoom / 2;

    // an x of 0 should map to -2.5
    // an x of width should map to 1
    // range of 1 - -2.5 = 3.5

    double width = window->getCanvasWidth();
    double midW = width / 2.0;
    double xOffset = x - midW;

    // we need to shift the left over by the scaled value of where we clicked,
    // in other words, we shift by xOffset * mandelbrotIncX
    minMandelbrotX += xOffset * mandelbrotIncX;

    // the middle stays the same when we zoom
    double middleX = minMandelbrotX + midW * mandelbrotIncX;

    // set the new increment factor
    mandelbrotIncX = 3.5 / width / mandelbrotZoom;

    minMandelbrotX = middleX - 3.5 / mandelbrotZoom / 2;
}

/*
 * Main function to run the program.
 */
int main() {
    FractalGUI gui;
    gui.initialize();
    gui.eventLoop();
    return 0;
}

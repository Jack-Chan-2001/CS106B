// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header!

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "strlib.h"
#include "lifegui.h"
using namespace std;

void welcome_messages(void);
void input_file(string &filename, ifstream &fin);
void display_grid(Grid<string>& grid);
bool wrap_switch(string& wrapping);
void update_grid(Grid<string>& grid, const bool wrapping);
int count_neighbors(Grid<string>& grid, int row, int col, const bool wrapping);

int main() {
    welcome_messages();

    // input file
    ifstream fin;
    string file;
    input_file(file, fin);

    // construct the grid
    string row, col;
    getline(fin, row);
    getline(fin, col);
    Grid<string> grid(stringToInteger(row), stringToInteger(col));
    string line;
    for (int j = 0; j < grid.nRows; j++) {
        getline(fin, line);
        for (int k = 0; k < grid.nCols; k++) {
            grid[j][k] = line[k];
        }
    }
    fin.close();

    // wrap switch
    string wrap;
    bool wrapping = wrap_switch(wrap);

    // initial window
    display_grid(grid);

    // menu option
    string option;
    do
    {
        option = getLine("a)nimate, t)ick, q)uit?");
        if (equalsIgnoreCase(option, string("a"))) {
            string frames;
            do
            {
                frames = getLine("How many frames?");
                if (!stringIsInteger(frames)) {
                    cout << "Illegal integer format. Try again.\n";
                }
            } while (!stringIsInteger(frames));
            for (int i = 0; i < stringToInteger(frames); i++)
            {
                clearConsole();
                update_grid(grid, wrapping);
                display_grid(grid);
                pause(50);
            }
        }
        if (equalsIgnoreCase(option, string("t"))) {
            update_grid(grid, wrapping);
            display_grid(grid);
        }
        if (!equalsIgnoreCase(option, string("q")) && !equalsIgnoreCase(option, string("t")) && !equalsIgnoreCase(option, string("a"))) {
            cout << "Invalid choice; please try again.\n";
        }
    } while (!equalsIgnoreCase(option, string("q")));

    cout << "Have a nice Life!";
    return 0;
}

// introduciton to this program
void welcome_messages(void)
{
    cout << "Welcome to the CS 106B Game of Life,\n"
         << "a simulation of the lifecycle of a bacteria colony.\n"
         << "Cells (X) live and die by the following rules:"
         << "- A cell with 1 or fewer neighbors dies.\n"
         << "- Locations with 2 neighbors remain stable.\n"
         << "- Locations with 3 neighbors will create life.\n"
         << "- A cell with 4 or more neighbors dies.\n\n";
}

// spesify the file to read
void input_file(string &filename, ifstream &fin)
{
    string filepath;
    do
    {
        filename = getLine("Grid input file name? ");
        filepath = string("../Life/res/") + filename;
        if (fileExists(filepath)) {
            openFile(fin, filepath);
        } else {
            cout << "Unable to open that file.  Try again." << endl;
        }
    } while (!isFile(filepath));
}

// print the current state of the frid, without modifying that state
void display_grid(Grid<string>& grid)
{
    for (int row = 0; row < grid.nRows; row++) {
        for (int col = 0; col < grid.nCols; col++) {
            cout << grid[row][col];
        }
        cout << endl;
    }
}

bool wrap_switch(string& wrap)
{
    bool is_wrapping;
    do
    {
        wrap = getLine("Should the simulation wrap around the grid (y/n)?");
        if (equalsIgnoreCase(wrap, string("y"))) {
            is_wrapping = true;
        } else if (equalsIgnoreCase(wrap, string("n")))
        {
            is_wrapping = false;
        }

    } while (!equalsIgnoreCase(wrap, string("y")) && !equalsIgnoreCase(wrap, string("n")));

    return is_wrapping;
}

// advance grid from one generation to the next
void update_grid(Grid<string>& grid, const bool wrapping)
{
    Grid<string> grid_n = grid;
    int neighbors;
    for (int row = 0; row < grid.nRows; row++) {
        for (int col = 0; col < grid.nCols; col++) {
            neighbors = count_neighbors(grid, row, col, wrapping);
            switch (neighbors)
            {
            case 0:
            case 1:
                grid_n[row][col] = string("-");
                break;
            case 2:
                grid_n[row][col] = grid[row][col];
                break;
            case 3:
                grid_n[row][col] = string("X");
                break;
            case 4:
                grid_n[row][col] = string("-");
                break;
            default:
                grid_n[row][col] = string("-");
                break;
            }
        }
    }
    // update grid via copying grid_n back
    grid = grid_n;
}

int count_neighbors(Grid<string>& grid, int row, int col, const bool wrapping)
{
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
                if (grid.inBounds(row + i, col + j) && grid[row + i][col + j].compare("X") == 0) {
                    count++;
                }
                if (wrapping && !grid.inBounds(row + i, col + j)) {
                    int new_row = (row + i + grid.nRows) % grid.nRows;
                    int new_col = (col + j + grid.nCols) % grid.nCols;
                    if (grid[new_row][new_col].compare("X") == 0) {
                        count++;
                    }
                }
        }
    }
    return count;
}

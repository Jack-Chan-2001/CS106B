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

int main() {
    // TODO: Finish the program!
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

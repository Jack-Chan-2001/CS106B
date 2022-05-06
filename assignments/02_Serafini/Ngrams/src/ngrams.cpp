#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "io/simpio.h"
#include "util/strlib.h"
#include "collections/hashmap.h"
#include "collections/vector.h"
#include "collections/set.h"

using namespace std;

void welcome();
void input(fstream& infile);
int getN();

int main() {
    // TODO: Finish the program!
    welcome();

    fstream infile;
    input(infile);
    int N = getN();

    cout << "Exiting." << endl;
    return 0;
}

void welcome()
{
    cout << "Welcome to CS 106B Random Writer ('N-Grams').\n"
        << "This program makes random text based on a document.\n"
        << "Give me an input file and an 'N' value for groups\n"
        << "of words, and I'll create random text for you.\n\n";
}

void input(fstream& infile)
{
    string filename;
    string dirPath = "./res/";
    do {
        filename = getLine("Input file name?");
        string filePath = dirPath + filename;
        infile.open(filePath);
        if (infile.fail()) {
            cout << "Unable to open that file.  Try again.\n";
        }
    } while (infile.fail());

}

int getN()
{
    string strN;
    while (true) {
        strN = getLine("Value of N?");
        if (!stringIsInteger(strN)) {
            cout << "Illegal integer format.  Try again.\n";
            continue;
        }
        int N = stringToInteger(strN);
        if (N < 2) {
            cout << "N must be 2 or greater.\n";
            continue;
        }
        return N;
    }
}

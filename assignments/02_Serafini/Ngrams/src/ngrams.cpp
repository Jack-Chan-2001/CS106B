#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

void welcome();

int main() {
    // TODO: Finish the program!
    welcome();

    string filename;
    
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

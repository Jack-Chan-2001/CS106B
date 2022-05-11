/**
 * Boogle.h containing Boggle class
 * 
 */

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "hashset.h"
#include "set.h"
#include "grid.h"

using namespace std;

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText = "");
    char getLetter(int row, int col) const;
    bool checkWord(string word) const;
    bool humanWordSearch(string word);
    Set<string> computerWordSearch();
    int getScoreHuman() const;
    int getScoreComputer() const;

    friend ostream& operator<<(ostream& out, Boggle& boggle);
    HashSet<string> getFoundWords() const;
    int getDimension() const;

private:
    bool humanWordSearchHelper(string word, Grid<bool>& isUsed, int row, int col);
    void computerWordSearchHelper(Set<string>& result, string word, Grid<bool>& isUsed, int row, int col);
    
    int dimension;
    int humanScore;
    int computerScore;
    Grid<char> gameBoard;
    Lexicon dictionary;
    HashSet<string> foundWords;   
};

#endif // _boggle_h

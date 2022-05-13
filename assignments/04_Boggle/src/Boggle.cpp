/**
 * This file contains methods and properies to perform Boggle game.
 * This is adapted from a post repo by afeacer.
 */

#include "Boggle.h"
#include "shuffle.h"
#include "random.h"
#include "strlib.h"
#include "bogglegui.h"

const int ANIMATION_DELAY = 100;

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// letters on every cube in 5x5 "Big Boggle" version (extension)
static string BIG_BOGGLE_CUBES[25] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

// Initialize the Boggle game board.
Boggle::Boggle(Lexicon& dictionary, string boardText) {
    dimension = 4;
    humanScore = 0;
    computerScore = 0;
    this->dictionary = dictionary;
    gameBoard.resize(dimension, dimension);

    if (boardText == "") {
        // shuffle the cubes
        shuffle(CUBES, 16);
        for (int r = 0; r < gameBoard.numRows(); r++) {
            for (int c = 0; c < gameBoard.numCols(); c++) {
                gameBoard[r][c] = stringToChar(toLowerCase(charToString(CUBES[r * dimension + c][randomInteger(0, 5)])));
            }
        }
    } else {
        for (int r = 0; r < gameBoard.numRows(); r++) {
            for (int c = 0; c < gameBoard.numCols(); c++) {
                gameBoard[r][c] = boardText[r * dimension + c];
            }
        }
    }
}

char Boggle::getLetter(int row, int col) const {
    if (!gameBoard.inBounds(row, col)) {
        throw("Row and/or col are out of bounds");
    }
    return gameBoard[row][col];
}

// return true if thr word is valid, false otherwise.
bool Boggle::checkWord(string word) const {
    return (dictionary.contains(toLowerCase(word))
        && word.size() >= 4
        && !foundWords.contains(toUpperCase(word)));
}

bool Boggle::humanWordSearch(string word) {
    BoggleGUI::clearHighlighting();
    Grid<bool> isUsed(dimension, dimension, false);
    if (checkWord(word)) {
        for (int r = 0; r < gameBoard.numRows(); r++) {
            for (int c = 0; c < gameBoard.numCols(); c++) {
                if (humanWordSearchHelper(word, isUsed, r, c)) {
                    foundWords.add(word);
                    humanScore += word.length() - 3;
                    return true;
                }
            }
        }
    }
    return false;
}

bool Boggle::humanWordSearchHelper(string word, Grid<bool>& isUsed, int row, int col) {
    BoggleGUI::setAnimationDelay(ANIMATION_DELAY);
    BoggleGUI::setHighlighted(row, col, true);
    if (gameBoard[row][col] == word[0] && !isUsed[row][col]) {
        if (word.size() == 1) {
            return true;
        }
        isUsed[row][col] = true;
        for (int r = row - 1; r <= row + 1; r++) {
            for (int c = col - 1; c <= col + 1; c++) {
                if (gameBoard.inBounds(r, c)
                    && !isUsed[r][c]
                    && humanWordSearchHelper(word.substr(1), isUsed, r, c)) {
                    return true;
                }
            }
        }
        isUsed[row][col] = false;
    }
    BoggleGUI::setAnimationDelay(ANIMATION_DELAY);
    BoggleGUI::setHighlighted(row, col, false);
    return false;
}

int Boggle::getScoreHuman() const {
    return humanScore;
}

Set<string> Boggle::computerWordSearch() {
    Set<string> result;
    Grid<bool> isUsed(dimension, dimension, false);
    for (int r = 0; r < gameBoard.numRows(); r++) {
        for (int c = 0; c < gameBoard.numCols(); c++) {
            computerWordSearchHelper(result, "", isUsed, r, c);
        }
    }
    return result;
}

void Boggle::computerWordSearchHelper(Set<string>& result, string word, Grid<bool>& isUsed, int row, int col) {
    if (checkWord(word)) {
        foundWords.add(toUpperCase(word));
        result.add(toUpperCase(word));
        computerScore += word.size() - 3;
    }
    for (int r = row - 1; r <= row + 1; r++) {
        for (int c = col - 1; c <= col + 1; c++) {
            if (gameBoard.inBounds(r, c)
                && !isUsed[r][c]
                && dictionary.containsPrefix(word + gameBoard[r][c])) {
                isUsed[r][c] = true;
                computerWordSearchHelper(result, word + gameBoard[r][c], isUsed, r, c);
                isUsed[r][c] = false;
            }
        }
    }
}


int Boggle::getScoreComputer() const {
    return computerScore;
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    for (int r = 0; r < boggle.gameBoard.numRows(); r++) {
        for (int c = 0; c < boggle.gameBoard.numCols(); c++) {
            out << toUpperCase(charToString(boggle.getLetter(r, c)));
        }
        out << endl;
    }
    return out;
}

HashSet<string> Boggle::getFoundWords() const {
    return foundWords;
}

int Boggle::getDimension() const {
    return dimension;
}

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "collections/lexicon.h"
#include "collections/queue.h"
#include "collections/stack.h"
#include "collections/hashset.h"
#include "io/simpio.h"

using namespace std;

void welcome();
void promptDictionary(fstream& inFile, string& filename, Lexicon& dictionary);
bool promptWords(string& startWord, string& endWord);
bool isValidInput(const string& startWord, const string& endWord, const Lexicon& dictionary);
void findWord(const string& startWord, const string& endWord, const Lexicon& dictionary);
HashSet<string> findNeighbors(const string& word, const Lexicon& dictionary);

int main() {
    // TODO: Finish the program!
    fstream inFile;
    string fileName, startWord, endWord;
    Lexicon dictionary;

    welcome();
    promptDictionary(inFile, fileName, dictionary);

    while (promptWords(startWord, endWord)) {
        findWord(startWord, endWord, dictionary);
    }

    cout << "Have a nice day." << endl;
    return 0;
}

void welcome()
{
    cout << "Welcome to CS 106B Word Ladder.\n"
        << "Please give me two English words, and I will change the\n"
        << "first into the second by changing one letter at a time.\n\n";
}

void promptDictionary(fstream& inFile, string& filename, Lexicon& dictionary)
{
    string dirPath("res/");
    do {
        filename = getLine("Dictionary file name?");
        string filePath = dirPath + filename;
        inFile.open(filePath);
        if (inFile.fail()) {
            cout << "Unable to open that file.  Try again.\n";
        }
    } while (inFile.fail());

    dictionary = Lexicon(inFile);
}

bool promptWords(string& startWord, string& endWord)
{
    cout << "\nWord #1 (or Enter to quit): ";
    getline(cin, startWord);
    if (startWord.empty()) {
        return false;
    }

    cout << "\nWord #2 (or Enter to quit): ";
    getline(cin, endWord);

    if (endWord.empty()) {
        return false;
    }

    startWord = toLowerCase(startWord);
    endWord = toLowerCase(endWord);
    return true;
}

bool isValidInput(const string& startWord, const string& endWord, const Lexicon& dictionary)
{
    if (startWord.length() != endWord.length()) {
        cout << "The two words must be the same length.\n";
        return false;
    } else if (!dictionary.contains(startWord) || !dictionary.contains(endWord)) {
        cout << "The two words must be found in the dictionary.\n";
        return false;
    } else if (startWord == endWord) {
        cout << "The two words must be different.\n";
        return false;
    }
    return true;
}

void findWord(const string& startWord, const string& endWord, const Lexicon& dictionary)
{
    if (!isValidInput(startWord, endWord, dictionary)) return;

    Stack<string> ladder{ startWord };
    Queue<Stack<string>> ladders{ ladder };
    HashSet<string> usedWords{ startWord };

    while (!ladders.isEmpty()) {
        Stack<string> currLadder = ladders.dequeue();
        string currWord = currLadder.peek();
        if (currWord == endWord) {
            // Find a solution
            // print all stack
            // cout << currLadder << endl;
            cout << "A ladder from " << endWord << " back to " << startWord << endl;
            while (!currLadder.isEmpty()) {
                cout << currLadder.pop() << " ";
            }
            cout << endl;
            return;
        } else {
            // Find its neighbors and push back
            for (string neighbor : findNeighbors(currWord, dictionary)) {
                if (!usedWords.contains(neighbor)) {
                    Stack<string> copyLadder = currLadder;
                    copyLadder.push(neighbor);
                    usedWords.add(neighbor);
                    ladders.enqueue(copyLadder);
                }
            }
        }
    }
    cout << "No word ladder found from " << endWord << " back to " << startWord << endl;
}

HashSet<string> findNeighbors(const string& word, const Lexicon& dictionary)
{
    HashSet<string> neighbors;
    string tmp = word;
    for (int i = 0; i < word.length(); i++) {
        for (char ch = 'a'; ch <= 'z'; ch++) {
            tmp[i] = ch;
            if (dictionary.contains(tmp)) {
                neighbors.add(tmp);
            }
        }
        tmp = word;
    }
    return neighbors;
}

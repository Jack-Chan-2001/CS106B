#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "io/simpio.h"
#include "util/strlib.h"
#include "util/random.h"
#include "collections/hashmap.h"
#include "collections/vector.h"

using namespace std;

void welcome();
void input(fstream& infile);
int getN();
HashMap<Vector<string>, Vector<string>> getNGramMap(const int& N, fstream& infile);
int getRandomWordNumber();
void generateRandomText(HashMap<Vector<string>, Vector<string>>& NGramMap, const int& N, const int& randomWordNumber);

int main() {
    // TODO: Finish the program!
    welcome();

    fstream infile;
    input(infile);
    int N = getN();
    HashMap<Vector<string>, Vector<string>> NGramMap = getNGramMap(N, infile);

    cout << "DEBUG: " << NGramMap << endl;

    int randomWordNumber;
    while (randomWordNumber = getRandomWordNumber()) {
        generateRandomText(NGramMap, N, randomWordNumber);
    }
    

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

HashMap<Vector<string>, Vector<string>> getNGramMap(const int& N, fstream& infile)
{
    HashMap<Vector<string>, Vector<string>> NGramMap;
    Vector<string> window, suffix;
    string word;
    Vector<string> words;

    // read sentences
    while (infile >> word) {
        words.add(word);
    }
    for (int i = 0; i < words.size(); i++) {
        for (int j = i; j < i + N - 1; j++) {
            if (j >= words.size()) {
                // wrapping
                window.add(words[j % words.size()]);
            } else {
                window.add(words[j]);
            }
        }
        if (NGramMap.containsKey(window)) {
            suffix = NGramMap[window];
        } else {
            suffix.clear();
        }
        if (i + N - 1 >= words.size()) {
            suffix.add(words[(i + N - 1) % words.size()]);
        } else {
            suffix.add(words[i + N - 1]);
        }
        NGramMap[window] = suffix;
        window.clear();
    }
    return NGramMap;
}

int getRandomWordNumber()
{
    string strRandomWord;
    int randomWordNumber;
    while (true) {
        strRandomWord = getLine("# of random words to generate (0 to quit)?");
        if (!stringIsInteger(strRandomWord)) {
            cout << "Illegal integer format. Try again.\n";
            continue;
        }
        randomWordNumber = stringToInteger(strRandomWord);
        if (randomWordNumber != 0 && randomWordNumber < 4) {
            cout << "Must be at least 4 words.\n";
            continue;
        }         
        return randomWordNumber;
    }
}

void generateRandomText(HashMap<Vector<string>, Vector<string>>& NGramMap, const int& N, const int& randomWordNumber)
{
    cout << "... ";

    // set up window
    Vector<string> randomText = randomKey(NGramMap);
    Vector<string> randomWindow = randomText;
    string randomTextSuffix = randomElement(NGramMap[randomText]);

    for (int i = 0; i < randomWordNumber - randomWindow.size(); i++) {
        randomWindow.remove(0);
        randomWindow.add(randomTextSuffix);
        randomTextSuffix = randomElement(NGramMap[randomWindow]);
        randomText.add(randomTextSuffix);
    }

    for (string word : randomText) {
        cout << word << " ";
    }

    cout << "... \n";
}

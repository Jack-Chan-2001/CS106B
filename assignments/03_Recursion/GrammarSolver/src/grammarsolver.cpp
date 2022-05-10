#include "grammarsolver.h"
#include "collections/hashmap.h"
#include "util/random.h"

using namespace std;

/**
 * @brief generateMapFromFile Generates a HashMap of strings to Vector of strings by
 * reading a file that is written according to the BNF Grammar format.
 * @param input The istream, that is processing the file.
 * @param BNFMap The HashMap to be generated.
 */
void generateMapFromFile(istream & input, HashMap<string, Vector<string>> & BNFMap) {
    string line;
    while (getline(input, line)) {
        Vector<string> firstSplit = stringSplit(line, "::=");
        string nonTerminal = trim(firstSplit[0]);
        Vector<string> rules = stringSplit(firstSplit[1], "|");
        BNFMap.put(nonTerminal, rules);
    }
}

/**
 * @brief grammarGenerate Randomly generates grammar elements, which obey specified grammar rules.
 * @param input The istream, that is processing the file.
 * @param symbol An element of grammar.
 * @param BNFMap The HashMap containing the rules and elements of a grammar.
 * @param element The element to be generated.
 */
void grammarGenerateHelper(istream & input, string symbol, HashMap<string, Vector<string>> & BNFMap, string & element) {
    if (!BNFMap.containsKey(symbol)) { //base case
        element += " " + trim(symbol);
    } else { //recursive step
        Vector<string> rules = BNFMap.get(symbol);
        string rule = rules[randomInteger(0, rules.size() - 1)];
        rule = trim(rule);
        Vector<string> symbols = stringSplit(rule, " ");
        for (string currentSymbol: symbols) {
            grammarGenerateHelper(input, currentSymbol, BNFMap, element); //recursive call
        }
    }
}

/**
 * Generates grammar for a given symbol a certain number of times given
 * a BNF input file.
 *
 * This will be called by grammarmain.cpp.
 *
 * @param input - Input stream of BNF file.
 * @param symbol - Symbol to generate
 * @param times - Number of times grammar is generated
 * @return Vector of strings of size times with random generations of symbol
 */
Vector<string> grammarGenerate(istream & input, string symbol, int times) {
    HashMap<string, Vector<string>> BNFMap; //Chosed HashMap instead of Map for better performance.
    generateMapFromFile(input, BNFMap);
    Vector<string> toReturn;
    for (int i = 0; i < times; i++) {
        string element = "";
        grammarGenerateHelper(input, symbol, BNFMap, element); //call to the helper function
        toReturn.add(element);
    }
    return toReturn;
}




/*
 * CS 106B/X Grammar Solver
 * This client program contains a text menu for running your grammar solver.
 *
 * You don't need to modify this file.
 * Your recursive functions should work with an unmodified version of this file.
 *
 * @author Marty Stepp
 * @version 2016/10/16
 * - modified for 16au version
 * @version 2015/04/24
 * - modified for 15sp version, including exception-handling
 * @version 2014/10/13
 * - modified for 14au version, including new GBufferedImage version of floodFill
 */

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "strlib.h"
#include "grammarsolver.h"

int main() {
    std::cout << "Welcome to CS 106B Grammar Solver!" << std::endl;
    std::cout << "I will use grammar rules to generate random" << std::endl;
    std::cout << "sentences, phrases, and expressions for you." << std::endl;

    // prompt for input files repeatedly
    bool play = true;
    while (play) {
        std::cout << std::endl;
        std::ifstream input;
        promptUserForFile(input, "Grammar file name? ");

        // prompt for symbols repeatedly
        while (true) {
            std::string symbol = trim(getLine("Symbol to generate (Enter to quit)? "));
            if (symbol.empty()) {
                break;
            }

            int times = getInteger("How many to generate? ");
            rewindStream(input);
            std::cout << std::endl;

            // call student's grammarGenerate function
            Vector<std::string> result = grammarGenerate(input, symbol, times);

            // print the vector of results
            for (int i = 0; i < result.size(); i++) {
                std::cout << std::setw(2) << (i+1) << ": " << result[i] << std::endl;
            }
            std::cout << std::endl;
        }
        input.close();

        // check if user wants to load another file
        play = getYesOrNo("Again? (Y/N)");
    }

    std::cout << "Exiting." << std::endl;
    return 0;
}

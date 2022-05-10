/*
 * CS 106B Boggle
 * This file implements the graphics support functions for Boggle.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * Version: Tue 2014/02/10
 * Author : original version by Eric Roberts and Julie Zelenski;
 *          modified by Marty Stepp for Autumn 2013 and Winter 2014
 *
 * Recent Changes:
 * - 2014/02/10: Significant refactoring to improve performance and robustness.
 *               GUI was creating tons of wasted rectangles and shapes and not
 *               cleaning them up.  Caused increasing slowdown over time.
 *               GUI now uses stateful shapes that persist.
 *
 * Known open issue: Leaks memory for all GObjects created.  Oops.
 * I would fix this, but I don't have time to figure out whether
 * freeing them would break things in the Java Back End.  -- Marty
 */

/*
 * General implementation notes
 * ----------------------------
 * This file implements the graphics support functions for the Boggle
 * program.  It makes straightforward use of the extended graphics
 * library to draw the cubes, board, letters, scoreboard, etc. The
 * implementations of the individual functions are fairly simple
 * and many of the helper functions do not require individual comments.
 * For descriptions of the behavior of the exported functions,
 * please see the interface file.
 */

#include <string>
#include "bogglegui.h"
#include "error.h"
#include "filelib.h"
#include "gobjects.h"
#include "grid.h"
#include "gtypes.h"
#include "gwindow.h"
#include "sound.h"
#include "strlib.h"
#include "vector.h"
using namespace std;

namespace BoggleGUI {
    /*
     * Constants
     * ---------
     * To configure fixed spacing, fonts, etc.
     */
    const int MAX_DIMENSION = 6;
    const int BOGGLE_WINDOW_WIDTH = 800;
    const int BOGGLE_WINDOW_HEIGHT = 450;
    const double DEFAULT_ANIMATION_DELAY = 0;

    const double BOARD_BORDER = 8;         // width of border around the cubes
    const double INDENT = 20;              // indent at vertical edges of window
    const double LABEL_HEIGHT = 40;        // space at top of window for labels
    const double LABEL_DY = -5;            // adjustment for label text
    const double WORD_DY = 2;              // adjustment for word list
    const double HUMAN_PERCENTAGE = 0.38;  // percentage of space for word lists
                                           // to apportion to human
    const string CUBE_FONT = "Helvetica";  // Font for letters on cube face
    const double FONT_PERCENTAGE = 0.6;    // percentage of cube used for letter
    const string SCORE_FONT = "Helvetica"; // Font for scoreboard names & scores
    const int SCORE_FONT_SIZE = 18;
    const int WORD_FONT_SIZE = 11;

#ifdef _MSC_VER
    const string WORD_FONT = "Arial";
#else
    const string WORD_FONT = "Geneva";
#endif

    // colors used on the GUI, as hex strings
    const string BOARD_COLOR    = "#326BF6";
    const string DIE_COLOR      = "#E6E6E6";
    const string LETTER_COLOR   = "#023B46";
    const string DIE_COLOR_H    = "#FFEE66";   // _H = highlighted colors
    const string LETTER_COLOR_H = "#023BD6";
    const string WORD_COLOR     = "#02805D";
    const string LABEL_COLOR    = "#028040";

    /*
     * A static struct is used to gather the state that is local to this module.
     * Mostly this is geometry calculated to fit the cubes, word lists,
     * and labels neatly within the current size of the graphics window.
     * In addition, the module tracks the count of words listed for each player
     * (to place additional words in the correct location in the displayed word
     * list) and the scores for each player (which must be saved and erased
     * before updating). There is also a 2-d array of the letters currently
     * showing on the cubes, to enable drawing them inverted for the
     * highlighting function.
     *
     * We'd like to use GRectangle instead, but it doesn't support setter methods.
     */
    struct RectStruct { double x, y, w, h; };
    static struct {
        int wordCount[2];         // words displayed for each player
        RectStruct scoreBox[2];   // rectangle enscribing label + score for each player
        RectStruct statusBox;     // rectangle enscribing current game status
        double wordColumnWidth;   // width of word list columns
        double cubeSize;          // size of cube
        double fontSize;          // font size used for labeling cubes
        RectStruct board;         // rectangle enscribed the cubes
        int rowCount;             // dimensions of cube layout on board
        int columnCount;
    } gState;

    /*
     * Maintains information about a single letter cube on the screen.
     * Used so that the letter cube's color and letter can be updated without
     * needing to redraw them over the existing one.
     */
    struct CubeInfo {
        GRoundRect* rect;
        GLabel* label;
        CubeInfo() {
            rect = NULL;
            label = NULL;
        }
    };

    // Global pointers to graphical objects such as the graphics window,
    // and collections of objects for letter cubes and words recorded.
    // These are used to update/clear the letters and words as the game goes on.
    static GWindow* gwp = NULL;
    static GLabel* humanScoreLabel = NULL;
    static GLabel* computerScoreLabel = NULL;
    static GLabel* statusLabel = NULL;
    static Grid<CubeInfo> letterCubes;
    static Vector<GLabel*> recordedWordLabels;
    static int animationDelay = DEFAULT_ANIMATION_DELAY;

    /*
     * These are the prototypes for functions private to this module.  These
     * helper functions are used to implement the functions which are exported
     * from this module (see .h for prototypes of the exported functions).
     * See the bodies below for more detailed documentation of each function.
     */
    static void calculateGeometry(int rowCount, int columnCount);
    static void ensureInitialized();
    static void setupLetterCubes();
    static void setupPlayerLabels(Player player, string name);

    void clearHighlighting() {
        ensureInitialized();
        for (int row = 0; row < gState.rowCount; row++) {
            for (int col = 0; col < gState.columnCount; col++) {
                setHighlighted(row, col, false);
            }
        }
    }

    void close() {
        if (isInitialized()) {
            gwp->setVisible(false);
            gwp->close();
            delete gwp;
            gwp = NULL;
            humanScoreLabel = NULL;
            computerScoreLabel = NULL;
            statusLabel = NULL;
            letterCubes.resize(0, 0);
            recordedWordLabels.clear();
        }
    }

    void initialize(int rowCount, int columnCount) {
        if (rowCount < 0 || rowCount > MAX_DIMENSION
                        || columnCount < 0 || columnCount > MAX_DIMENSION) {
            error("init called with invalid dimensions.");
        }

        if (!isInitialized()) {
            gwp = new GWindow(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);
            gwp->setWindowTitle("CS 106B Boggle");
            gwp->setExitOnClose(true);
            gState.rowCount = rowCount;
            gState.columnCount = columnCount;
            letterCubes.resize(rowCount, columnCount);
            calculateGeometry(rowCount, columnCount);

            gState.wordCount[HUMAN] = 0;
            gState.wordCount[COMPUTER] = 0;

            // draw the overall background
            gwp->setColor(BOARD_COLOR);
            gwp->fillRect(gState.board.x, gState.board.y, gState.board.w, gState.board.h);

            // draw the labels of player names and score labels
            setupPlayerLabels(HUMAN, "Human");
            setupPlayerLabels(COMPUTER, "Computer");

            // set up status label
            statusLabel = new GLabel("");
            statusLabel->setFont(SCORE_FONT + "-" + integerToString(SCORE_FONT_SIZE));
            statusLabel->setColor(LABEL_COLOR);
            gwp->add(statusLabel, gState.statusBox.x, gState.statusBox.y);

            // draw the 4x4 grid of Boggle letter cubes
            setupLetterCubes();
        }

        reset();
    }

    bool isInitialized() {
        return gwp != NULL;
    }

    void labelAllCubes(string letters) {
        ensureInitialized();
        if ((int) letters.length() < gState.rowCount * gState.columnCount) {
            error("labelCubes called with too-short letters string: \"" + letters + "\"");
        }
        int row = 0;
        int col = 0;
        bool labeledAll = false;
        for (int i = 0; i < (int) letters.length(); i++) {
            char letter = letters[i];
            if (!isalpha(letter)) {
                continue;
            }
            labelCube(row, col, letter);
            col++;
            if (col == gState.columnCount) {
                col = 0;
                row++;
                if (row == gState.rowCount) {
                    labeledAll = true;
                    break;
                }
            }
        }
        if (!labeledAll) {
            error("labelCubes didn't label every cube from letters string: \"" + letters + "\"");
        }
    }

    void labelCube(int row, int col, char letter, bool highlighted) {
        ensureInitialized();
        if (row < 0 || row >= gState.rowCount || col < 0 || col >= gState.columnCount) {
            error(string("labelCube called with invalid row, col arguments.  Must be between (0, 0) and (")
                  + integerToString(gState.rowCount) + ", " + integerToString(gState.columnCount) + ")");
        }
        if (!isalpha(letter) && letter != ' ') {
            error(string("labelCube called with non-alphabetic character: '") + letter);
        }

        setHighlighted(row, col, highlighted);

        GLabel* label = letterCubes[row][col].label;
        label->setLabel(string(1, letter));
        if (isalpha(letter)) {
            label->setLocation(
                    letterCubes[row][col].rect->getX() + gState.cubeSize/2 - 0.45 * label->getWidth(),
                    letterCubes[row][col].rect->getY() + gState.cubeSize/2 + 0.36 * gState.fontSize);
        }
    }

    void playSound(string filename) {
        Sound s(filename);
        s.play();
    }

    void recordWord(string word, Player player) {
        ensureInitialized();
        if (player != HUMAN && player != COMPUTER) {
            error("recordWord called with invalid player argument.");
        }
        word = toLowerCase(trim(word));
        GLabel* label = new GLabel(word);
        label->setFont(WORD_FONT + "-" + integerToString(WORD_FONT_SIZE));
        label->setColor(WORD_COLOR);
        int wordCountInRow = int(gState.scoreBox[player].w/gState.wordColumnWidth);
        int row = gState.wordCount[player] / wordCountInRow;
        int col = gState.wordCount[player] % wordCountInRow;
        gState.wordCount[player]++;
        gwp->add(label, gState.scoreBox[player].x + col * gState.wordColumnWidth,
                      gState.scoreBox[player].y + (row + 1) * WORD_FONT_SIZE + WORD_DY);
        if (col == wordCountInRow - 1) {
            gwp->repaint();
        }
        recordedWordLabels.add(label);
    }

    void reset() {
        ensureInitialized();

        // clear out the counts of words recorded so far and scores
        gState.wordCount[HUMAN] = 0;
        gState.wordCount[COMPUTER] = 0;
        setScore(0, HUMAN);
        setScore(0, COMPUTER);

        // clear the word cubes
        for (int row = 0; row < gState.rowCount; row++) {
            for (int col = 0; col < gState.columnCount; col++) {
                labelCube(row, col, ' ', /* highlighted */ false);
            }
        }

        // clear out the words found by each player
        for (GLabel* label : recordedWordLabels) {
            gwp->remove(label);
        }
        recordedWordLabels.clear();
    }

    void setAnimationDelay(int ms) {
        animationDelay = ms;
    }

    void setHighlighted(int row, int col, bool highlighted) {
        letterCubes[row][col].rect->setFillColor(highlighted ? DIE_COLOR_H : DIE_COLOR);
        letterCubes[row][col].label->setColor(highlighted ? LETTER_COLOR_H : LETTER_COLOR);
        if (highlighted && animationDelay > 0) {
            pause(animationDelay);
        }
    }

    void setScore(int points, Player player) {
        ensureInitialized();
        GLabel* scoreLabel = player == COMPUTER ? computerScoreLabel : humanScoreLabel;
        scoreLabel->setLabel(integerToString(points));
    }

    void setStatusMessage(std::string status) {
        ensureInitialized();
        statusLabel->setLabel(status);
    }

    void shutdown() {
        if (isInitialized()) {
            close();
            exitGraphics();
        }
    }

    /*
     * This internal helper does all the messy math to work out how to divide
     * up the space within the current graphics window to neatly fit the board,
     * the cubes, and the word lists.
     * This function does not actually add any GObjects to the GWindow;
     * but it does set the state of the 'gState' structure to be used later.
     */
    static void calculateGeometry(int rowCount, int columnCount) {
        double boardSize = min(gwp->getWidth() / 3 - 2 * INDENT,
                               gwp->getHeight() - 2 * LABEL_HEIGHT);
        gState.cubeSize = min((boardSize-BOARD_BORDER)/rowCount,
                              (boardSize-BOARD_BORDER)/columnCount);
        gState.board.w = gState.board.h = boardSize;
        gState.board.y = LABEL_HEIGHT + LABEL_HEIGHT;
        double leftover = gwp->getWidth() - gState.board.w - 2*INDENT;
        gState.scoreBox[HUMAN].x = INDENT;
        gState.scoreBox[HUMAN].y = gState.scoreBox[COMPUTER].y = gState.board.y;
        gState.scoreBox[HUMAN].h = gState.scoreBox[COMPUTER].h = LABEL_HEIGHT;
        gState.scoreBox[HUMAN].w = leftover*HUMAN_PERCENTAGE;
        gState.board.x = gState.scoreBox[HUMAN].x + gState.scoreBox[HUMAN].w + INDENT;
        gState.scoreBox[COMPUTER].x = gState.board.x + gState.board.w + INDENT;
        gState.scoreBox[COMPUTER].w = gwp->getWidth()
                                    - gState.scoreBox[COMPUTER].x - INDENT;
        gState.statusBox.x = INDENT;
        gState.statusBox.y = LABEL_HEIGHT;
        gState.statusBox.w = gwp->getWidth() - 2*INDENT;
        gState.statusBox.h = LABEL_HEIGHT;

        gState.rowCount = rowCount;
        gState.columnCount = columnCount;

        // figure out an ideal font size by creating labels of different sizes
        GLabel label("mmmmm");
        label.setFont(WORD_FONT + "-" + integerToString(WORD_FONT_SIZE));
        gState.wordColumnWidth = label.getWidth();

        for (gState.fontSize = 8; gState.fontSize < 48; gState.fontSize++) {
            GLabel label2("M");
            label2.setFont(CUBE_FONT + "-" + integerToString(int(gState.fontSize)));
            if (label2.getWidth() > FONT_PERCENTAGE * gState.cubeSize) {
                break;
            }
        }
    }

    /*
     * Throws an ErrorException if the GUI has not been initialized.
     */
    static void ensureInitialized() {
        if (!isInitialized()) {
            error("Boggle GUI has not been initialized yet");
        }
    }

    /*
     * Creates all cubes in position, including a blank letter centered in
     * the middle of each cube.  Initially the cubes are not highlighted.
     */
    static void setupLetterCubes() {
        double lineWidth = 2;
        double cubeSize = gState.cubeSize - lineWidth;
        double cubeY = gState.board.y + BOARD_BORDER/2;
        for (int row = 0; row < gState.rowCount; row++) {
            double cubeX = gState.board.x + BOARD_BORDER/2;
            for (int col = 0; col < gState.columnCount; col++) {
                // display the letter cubes as rounded rectangles
                double cubeRoundRadius = gState.cubeSize/5;
                GRoundRect* rect = new GRoundRect(cubeX, cubeY, cubeSize,
                                                  cubeSize, cubeRoundRadius * 2);
                rect->setLineWidth(lineWidth);
                rect->setColor("Black");
                rect->setFilled(true);
                rect->setFillColor(DIE_COLOR);
                gwp->add(rect);
                letterCubes[row][col].rect = rect;

                // draw the letter on the cube
                GLabel* label = new GLabel("M");   // start as "M" for getWidth
                label->setFont(CUBE_FONT + "-" + integerToString(int(gState.fontSize)));
                label->setColor(LETTER_COLOR);
                label->setLocation(cubeX + gState.cubeSize/2 - label->getWidth()/2,
                                   cubeY + gState.cubeSize/2 + 0.4 * gState.fontSize);
                label->setLabel(" ");
                gwp->add(label);
                letterCubes[row][col].label = label;
                cubeX += (int) cubeSize + lineWidth;
            }
            cubeY += (int) cubeSize + lineWidth;
        }
    }

    /*
     * Labels player word list with specified name and draws a line underneath.
     * Also sets up the score label for the specified player.
     */
    static void setupPlayerLabels(Player player, string name) {
        gwp->setColor(LABEL_COLOR);
        gwp->drawLine(gState.scoreBox[player].x, gState.scoreBox[player].y,
                      gState.scoreBox[player].x + gState.scoreBox[player].w,
                      gState.scoreBox[player].y);
        GLabel* lab = new GLabel(name);
        lab->setFont(SCORE_FONT + "-" + integerToString(SCORE_FONT_SIZE));
        gwp->add(lab, gState.scoreBox[player].x, gState.scoreBox[player].y + LABEL_DY);

        GLabel** scoreLabel = player == COMPUTER ? &computerScoreLabel : &humanScoreLabel;
        *scoreLabel = new GLabel("0");
        (*scoreLabel)->setFont(SCORE_FONT + "-" + integerToString(SCORE_FONT_SIZE));
        (*scoreLabel)->setColor(LABEL_COLOR);
        int offset = 32;
        gwp->add(*scoreLabel, gState.scoreBox[player].x + gState.scoreBox[player].w - offset,
                 gState.scoreBox[player].y + LABEL_DY);
    }
}

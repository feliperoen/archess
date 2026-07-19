/*
 * This class inherits methods from the Interface class to handle user inputss
 * and has methods to display elements of the chess game on the screen
 * using ncurses library,
 * such as the board, pieces, clocks, moves and more.
*/


#ifndef InterfaceNcurses_H
#define GAMECINTROLLER_H

#define COLOR_DARKGRAY 8
#define COLOR_LIGHTGRAY 9
#define COLOR_MATTEWHITE 10
#define COLOR_LIGHTWHITE 11

#include <iostream>
#include <locale.h>
#include <csignal>
#include <ncurses.h>

#include "Interface.h"

#include "Coordinates.h"


class InterfaceNcurses : public Interface
{
    public:
        InterfaceNcurses();
        ~InterfaceNcurses();

        int getMenuSelection(const std::atomic<bool>& interruption) override;

        Settings::SettingValues getSettings(const std::atomic<bool>& interruption) override;

        Board::Coordinates getSelectedSquare(Board* board, MoveLog* moveLog, Clock* clock, GameOverFlags& gameOverFlags,
            std::atomic<bool>& interruption) override;

        char getPromotion(Clock*, const std::atomic<bool>& timesUp, const std::atomic<bool>& interruption) override;

        bool getConfirmation(const std::string confirmQuery, const std::atomic<bool>& interruption) override;

        void printError(const std::string errorMessage) override;

    protected:

    private:
        MEVENT event;

        /*Prints specific screen elements used as subwindows of the main windows.*/
        void printInstructions();
        void printMenuItems();
        void printMenuHeader();
        void printClock(Clock* clock);
        void printBoard(Board* board);
        void printMoveLog(MoveLog* moveLog);
        void printWhiteCaptured(Board* board);
        void printBlackCaptured(Board* board);
        void printEndGame(GameOverFlags& gameOverFlags);

        /*Displays the entire screens.*/
        void printMenuWindow();
        void printSettingsWindow();
        void printNewGameWindow(Board* board, MoveLog* moveLog, GameOverFlags& gameOverFlags);

        /*The functions below build and destroy the main windows and their subwindows,
        deallocating the pointers allocated to them.*/
        void createMenuWindow();
        void createNewGameWindow();
        void createSettingsWindow();

        void destroyMenuWindow();
        void destroyNewGameWindow();
        void destroySettingsWindow();


        char convertX(int x);
        int convertY(int y);
        std::string convertToUnicode(char pieceNotation);

        /*Subwindows.*/
        WINDOW* error_win=nullptr;
        WINDOW* confirmation_win=nullptr;
        WINDOW* instructions_win=nullptr;

        WINDOW* list_win=nullptr;
        WINDOW* menuHeader_win=nullptr;

        WINDOW* board_win=nullptr;
        WINDOW* white_clock=nullptr;
        WINDOW* black_clock=nullptr;
        WINDOW* moveLog_win=nullptr;
        WINDOW* whiteCaptured_win=nullptr;
        WINDOW* blackCaptured_win=nullptr;
        WINDOW* promotion_win=nullptr;
        WINDOW* gameOver_win=nullptr;

        /*Main windows.*/
        WINDOW* menu_win=nullptr;
        WINDOW* newGame_win=nullptr;
        WINDOW* settings_win=nullptr;

        bool boardPerspective=WHITE;

        /*Variables that control the position of elements on the screen.*/
        int xBoard=NULL_INT,
            yBoard=NULL_INT,
            /*These first two control the position of the board on the screen.
            They are used by other elements on the screen to position themselves,
            as everything revolves around the board in the game.*/

            yWhiteCursor=NULL_INT,
            xWhiteCursor=NULL_INT,

            yBlackCursor=NULL_INT,
            xBlackCursor=NULL_INT,

            yWhiteClock=NULL_INT,
            yBlackClock=NULL_INT,

            yBlackCaptured=NULL_INT,
            yWhiteCaptured=NULL_INT,

            moveLogHeight=NULL_INT,

            menuSelecter=NULL_INT; /*Controls the position of the selection arrow in the menus.*/


        /*String vectors that receive texts to be displayed in the windows.*/
        std::vector<std::string> key, /*Functionality keys.*/
                                 instructions, /*Instructions associated with the keys.*/

                                 menuItems; /*List of menu items.*/

        typedef struct
        {
            int height, width, y, x;
        } Button;

        bool checkClickButton(Button button, int mouseX, int mouseY);
};

#endif // InterfaceNcurses_H

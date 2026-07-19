/*
 * This class has methods to display elements of the chess game on the screen
 * using ncurses library,
 * such as the board, pieces, clocks, moves, and more.
*/


#ifndef GAMECONTROLLER_H
#define GAMECINTROLLER_H

#include <iostream>
#include <locale.h>
#include <ncurses.h>

#include "Board.h"
#include "Interface.h"

class GameController : public Interface
{
    public:
        GameController();
        virtual ~GameController();

        int getMenu() override;
        void getBoard(Board*, int, int) override;

        /*void getNewGame(Board*);
        void getMove(Board*, Pieces*, int, int, bool*);
        char getPromotion(int, int);

        char convertX(int);
        int convertY(int);*/


    protected:

    private:
};

#endif // GAMECONTROLLER_H

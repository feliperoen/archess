/*
* This class contains methods to control the application,
* orchestrating the other classes according to the settings and chess rules.
*/

#ifndef GameController_H
#define GameController_H

#include <iostream>
#include <vector>
#include <atomic>
#include <string>
#include <csignal>
#include <sstream>
#include <stack>

#include "Settings.h"
#include "Board.h"
#include "Clock.h"
#include "Pawn.h"
#include "King.h"
#include "Rook.h"
#include "Bishop.h"
#include "Queen.h"
#include "Knight.h"
#include "MoveLog.h"
#include "Interface.h"
#include "SoundManager.h"

#include "Coordinates.h"

class GameController
{
    public:
        /*The constructor receives a pointer to an object related to the graphic interface
        that will display information about the application and capture inputs from the user.*/
        GameController(Interface* interface);
        ~GameController();

        void mainMenu(Interface* interface);

    protected:

    private:
        /*The methods below control the main features of the game, as settings and new game rules,
        using the pointer to the interface object to display the screens related to each part of the application.*/
        void settingsControl(Interface* interface);
        void newGame(Interface* interface);

        /*Removes the promoted pawn and places a queen, rook, bishop, or knight in its place, according to the user's choice.*/
        void setPromotion(Board* board, Pieces* promotedPawn, const char columnPromotion, const char promotionPieceNotation);


        /*Returns a pointer to a board with pieces positioned for a new game.*/
        Board* setNewgame();

        Pieces* createPiece(const Pieces::PieceType pieceType, const bool pieceColor);

        void loadFEN(const std::string& fen);

        /*Converts the settings values related to clock time into minutes.*/
        int convertClockTime();

        bool getUserInputWithTimeout(int timeoutInSeconds);

        /*Instantiates a pointer to a Settings object that will manage the configurations of the application.*/
        Settings* settings = nullptr;

        SoundManager* soundManager = nullptr;

        struct MoveData
        {
            Pieces* pieceMoved = nullptr;

            Pieces* capturedPiece=nullptr;

            int previousRow = NULL_INT,
                targetRow = NULL_INT;

            char previousCol = NULL_CHAR,
                 targetCol = NULL_CHAR;

            bool isCapture=false,

                 castlingWhiteKingside=false,
                 castlingWhiteQueenside=false,
                 castlingBlackKingside=false,
                 castlingBlackQueenside=false,

                 anPassant = false;

        };
};

#endif // GameController_H

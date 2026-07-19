/*
*  This class implements methods and variables to save the moves to a file at end of the game.
*/

#ifndef MOVELOG_H
#define MOVELOG_H

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <chrono>

#include "Board.h"

class MoveLog
{
    public:

        MoveLog();
        ~MoveLog();

        /*Analyzes the moves and writes the appropriate notation
        to the vector of strings whiteMoves and blackMoves.*/
        void setMoveNotation(Board* board, const char previousColumn, const int previousRow, const char targetColumn, const int targeRow);
        void setPromotionNotation(const char piecePromovedNotation);
        void setCheckNotation();
        void setCheckMateNotation();

        void setMoveTime(const std::chrono::hours hour, const std::chrono::minutes minute, const std::chrono::seconds second);

        void setTurn(const bool turnColor);

        void setMoveLogFile();/*Writes the contents of the vectors to a text file.*/

        const std::vector<std::string>& getWhiteMoves() const;
        const std::vector<std::string>& getBlackMoves() const;

        void popWhiteMoves();
        void popBlackMoves();

        int getNumberOfMoves() const;

        void decrementMoves();


    protected:

    private:
        bool turn=WHITE;

        int moves=1;

        std::vector<std::string> whiteMoves;
        std::vector<std::string> whiteMovesTimes;

        std::vector<std::string> blackMoves;
        std::vector<std::string> blackMovesTimes;


};

#endif // MOVELOG_H

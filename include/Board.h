/*
*  This class sets up an 8x8 grid of structs
*  containing variables relevant to the chessboard.
*/

#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <array>
#include <string>
#include <sstream>

#include "Constants.h"

#include "Pieces.h"

class Board
{
    private:

    public:
        Board(const bool canCastlingWhiteKingSide, const bool canCastlingWhiteQueenSide,
             const bool canCastlingBlackKingSide, const bool canCastlingBlackQueenSide);

        ~Board();

        void setPiece(Pieces* ptrPiece, const int tergetRow, const char targetColumn);
        /*Sets the piece (Piece* ptrPiece) in the square (int destrow, char)
        and do the necessary checks.*/

        void setAllowedMove(const int row, const char column);
        void setThreatenedSquare(const int row, const char column);

        bool getColor(const int row, const char column) const;
        Pieces* getPiece(const int row, const char column) const;
        bool getAllowedMove(const int row, const char column) const;
        bool getThreatenedSquare(const int row, const char column) const;

        bool checkAllowedMoves() const; /*Checks if any square has the allowedMove flag set to true.*/

        void removePiece(const int row, const char column);
        void removeAllowedMoves(); /*Remove all allowed moves on the board.*/
        void removeAllowedMove(const int row, const char column);
        void removeThreatenedSquares();

        void checkPassant(const Pieces* ptrPawn); /*Checks if a pawn(Piece* ptrPawn) crossed a threatened square for another pawn in your last move.*/
        void setPassantColumn(const char column);
        char getPassantColumn() const;
        bool getTurnPassant() const;

        void setCastlingWhiteKingside(const bool isAllowed);
        void setCastlingWhiteQueenside(const bool isAllowed);
        void setCastlingBlackKingside(const bool isAllowed);
        void setCastlingBlackQueenside(const bool isAllowed);

        bool getCastlingWhiteKingside() const;
        bool getCastlingWhiteQueenside() const;
        bool getCastlingBlackKingside() const;
        bool getCastlingBlackQueenside() const;

        bool isPinnedPiece(Pieces* ptrPiece, const int targetRow, const char targetColumn); /*Checks if the piece(Piece* ptrPiece) are pinned to make a move*/
        bool isCheck(const  bool kingsColor); /*Receives a bool referring to the king’s color and checks if he is in check.*/
        bool isCheckMate(const bool color);

        /*Methods to be used by MoveLog.*/
        bool willBeKingInCheck(Pieces* ptrPiece, const int row, const char column);/*Checks whether when Piece* moves the king will be in check.*/
        bool isDoubleMove(Pieces* ptrPiece, const int row, const char column);/*Checks if any other piece can move to the same square(int row, char column) as Piece* ptrPiece.*/

        std::string generateFEN(const bool turn, const int halfMoveClock, const int fullMoveNumber);
        void loadFEN(const std::string& fen);

        /*A struct is used by other classes
        that need to return coordinates for a specific square.*/
        struct Coordinates
        {
            int row = NULL_INT;
            char column = NULL_CHAR;
        };

        const std::vector<Pieces*>& getWhiteCaptured() const;
        const std::vector<Pieces*>& getBlackCaptured() const;

        const std::vector<Pieces*>& getWhitePromoted() const;
        const std::vector<Pieces*>& getBlackPromoted() const;

        void popWhiteCaptured();
        void popBlackCaptured();

        void popWhitePromoted();
        void popBlackPromoted();

        void setWhitePromoted(Pieces* promotedPawn);
        void setBlackPromoted(Pieces* promotedPawn);

        static constexpr int ROW_MIN = 1,
                             ROW_MAX = 8;

        static constexpr char COLUMN_MIN = 'a',
                              COLUMN_MAX = 'h';

        static constexpr bool  castlingWhiteKingSideAllowed = true,
                               castlingWhiteQueenSideAllowed  = true,

                               castlingBlackKingSideAllowed = true,
                               castlingBlackQueenSideAllowed = true,

                               castlingWhiteKingSideDenied = false,
                               castlingWhiteQueenSideDenied  = false,

                               castlingBlackKingSideDenied = false,
                               castlingBlackQueenSideDenied = false;

    protected:
        /*The struct bellow represents a square on the chessboard,
        and the variables are its characteristics.
        The pointer points to the object representing the piece on that square.
        If there's no piece on the square, the pointer returns to NULL.
        Objects derived from Piece class have methods to set the variable allowedMove to true
        so they can move to the square to which the variable belongs.
        The same happens for the threatenedSquare variable,
        when setting this variable to true they signal that they are attacking the square.*/
        struct Square
        {
            bool squareColor, /*Sets the color considering the constants WHITE as true and BLACK as false.*/
                 allowedMove,
                 threatenedSquare;

            Pieces* piece;

            Square(bool squareColor=false, bool allowedMove=false, bool threatenedSquare=false, Pieces* piece=nullptr)
            : squareColor(squareColor), allowedMove(allowedMove), threatenedSquare(threatenedSquare), piece(piece) {}
        };

        std::array<std::array<Square, ROW_MAX>, COLUMN_MAX> board;
        /*Declaring an 8x8 matrix of structs that represents the board.*/

        /*Variables to indicate if is possible castling.*/
        bool castlingWhiteKingSide,
             castlingWhiteQueenSide,

             castlingBlackKingSide,
             castlingBlackQueenSide;

        /*Indicates the column of the pawn tha can make en passant moviment.*/
        char passantColumn=NULL_CHAR;

        /*Variable to indicate which player can execute an passant.*/
        bool turnPassant;

        /*Vectors of pointers to captured pieces.*/
        std::vector<Pieces*> whiteCaptured;
        std::vector<Pieces*> blackCaptured;

        std::vector<Pieces*> whitePromoted;
        std::vector<Pieces*> blackPromoted;

};

#endif // BOARD_H

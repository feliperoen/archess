/*
 * This class has methods and variables
 * to be inherited by other classes representing specific pieces.
*/

#ifndef PIECES_H
#define PIECES_H

class Board;

class Pieces
{
    public:

        /*The constructor initializes the piece's characteristics.*/
        Pieces(const bool color, const int row, const char column, const bool isSelected);
        virtual ~Pieces();

        void setColor(const bool color);
        void setColumn(const char column);
        void setRow(const int row);
        void setSelectedPiece(const bool selected);

        bool getColor() const;
        char getColumn() const;
        int getRow() const;
        char getNotation() const;
        bool getSelected() const;

        virtual void setMove(Board*) = 0;
        /*Defines on the board which squares the piece can move to.*/

        virtual void setThreat (Board*) = 0;
        /*Defines on the board which squares the piece is attacking.*/

        enum PieceType
        {
            KING,
            QUEEN,
            ROOK,
            BISHOP,
            KNIGHT,
            PAWN
        };

        PieceType getPieceType() const;

    protected:
        int pieceRow;

        char pieceColumn,
             pieceNotation;

        bool pieceColor, /*White is true, black is false.*/
             pieceSelected;
        /*
            The chess pieces' notation follows this pattern:
            K = King
            Q = Queen
            B = Bishop
            N = Knight
            R = Rook
            P = Pawn

            Uppercase letters will be used for white pieces, and lowercase letters for black pieces (k, q, b, n, r and p).

        */

        PieceType pieceType;

    private:


};

#endif // PIECES_H

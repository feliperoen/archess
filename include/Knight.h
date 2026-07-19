/*
 *This class implements the knight piece.
*/

#ifndef KNIGHT_H
#define KNIGHT_H

#include "Pieces.h"
#include "Board.h"

class Knight : public Pieces
{
    public:
        Knight(const bool color, const int row, const char column, const bool isSelected);
        ~Knight();

        void setMove(Board*) override;
        void setThreat (Board*) override;

    protected:

    private:
};

#endif // KNIGHT_H

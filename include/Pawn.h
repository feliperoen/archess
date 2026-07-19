/*
 *This class implements the pawn piece.
*/

#ifndef PAWN_H
#define PAWN_H

#include "Pieces.h"
#include "Board.h"

class Pawn : public Pieces
{
    public:
        Pawn(const bool color, const int row, const char column, const bool isSelected);
        ~Pawn();

        void setMove(Board*) override;
        void setThreat (Board*) override;


    protected:

    private:
};

#endif // PAWN_H

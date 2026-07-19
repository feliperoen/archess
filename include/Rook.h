/*
 *This class implements the rook piece.
*/

#ifndef ROOK_H
#define ROOK_H

#include "Pieces.h"
#include "Board.h"

class Rook : public Pieces
{
    public:
        Rook(const bool color, const int row, const char column, const bool isSelected);
        ~Rook();

        void setMove(Board*) override;
        void setThreat (Board*) override;


    protected:

    private:
};

#endif // ROOK_H

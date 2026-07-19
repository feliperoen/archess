/*
 *This class implements the queen piece.
*/

#ifndef QUEEN_H
#define QUEEN_H

#include "Pieces.h"
#include "Board.h"

class Queen : public Pieces
{
    public:
        Queen(const bool color, const int row, const char column, const bool isSelected);
        ~Queen();

        void setMove(Board*) override;
        void setThreat (Board*) override;


    protected:

    private:
};

#endif // QUEEN_H

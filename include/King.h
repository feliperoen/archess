/*
 *This class implements the king piece.
*/

#ifndef KING_H
#define KING_H

#include "Pieces.h"
#include "Board.h"


class King : public Pieces
{
    public:
        King(const bool color, const int row, const char column, const bool isSelected);
        ~King();

        void setMove(Board*) override;
        void setThreat (Board*) override;



    protected:

    private:
        /*Checks if the destination square for the king is threatened by opponent pieces.*/
        bool getThreat(Board* board, int row, char column) const;
};

#endif // KING_H

/*
 *This class implements the bishop piece.
*/

#ifndef BISHOP_H
#define BISHOP_H

#include "Pieces.h"
#include "Board.h"

class Bishop : public Pieces
{
    public:
        Bishop(const bool color, const int row, const char column, const bool isSelected);
        ~Bishop();

        void setMove(Board*) override;
        void setThreat (Board*) override;


    protected:

    private:
};

#endif // BISHOP_H

#ifndef PAWNMOVES_H
#define PAWNMOVES_H

#include "Pieces.h"
#include "Board.h"


class PawnMoves
{
    public:
        PawnMoves();
        virtual ~PawnMoves();

        void setPromotion(char);
        void setLeftPassant(char);
        void setRightPassant(char);
        char getLeftPassant() const;
        char getRightPassant() const;
        void getPassant(Pieces*);

    protected:

        char leftPassant=NULL_CHAR,
             rightPassant=NULL_CHAR;



    private:
};

#endif // PAWNMOVES_H

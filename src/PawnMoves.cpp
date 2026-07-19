#include "PawnMoves.h"

PawnMoves::PawnMoves()
{
    //ctor
}

PawnMoves::~PawnMoves()
{
    //dtor
}


void PawnMoves::setLeftPassant(char column)
{
    leftPassant=column;
}


void PawnMoves::setRightPassant(char column)
{
    rightPassant=column;
}


void PawnMoves::setPromotion(char promovedPiece)
{
    /*Pieces* ptrPiece = nullptr;

    switch (promovedPiece) {
        case 'Q':
        case 'q':
            ptrPiece = new Queen(color);
            break;

        case 'R':
        case 'r':
            ptrPiece = new Rook(color);
            break;

        case 'B':
        case 'b':
            ptrPiece = new Bishop(color);
            break;

        case 'N':
        case 'n':
            ptrPiece = new Knight(color);
            break;
    }

    if (ptrPiece)
    {
        ptrPiece->setRow(promovedPiece->getRow());
        ptrPiece->setColumn(promovedPiece->getColumn());
        setPiece(ptrPiece->getRow(), ptrPiece->getColumn(), ptrPiece);
    }*/
}


char PawnMoves::getLeftPassant() const
{
    return leftPassant;
}

char PawnMoves::getRightPassant() const
{
    return rightPassant;
}

void PawnMoves::getPassant(Pieces* ptrPiece)
{

    /*int row=ptrPiece->getRow();
    char column=ptrPiece->getColumn();

    Pieces* ptrCheck;


    ptrCheck = getPiece(row, column-1);
    if(ptrCheck!=nullptr && (ptrCheck->getNotation()=='P'||ptrCheck->getNotation()=='p'))
        setLeftPassant(ptrCheck->getColumn());


    ptrCheck = getPiece(row, column+1);
    if(ptrCheck!=nullptr && (ptrCheck->getNotation()=='P'||ptrCheck->getNotation()=='p'))
        setRightPassant(ptrCheck->getColumn());*/


}

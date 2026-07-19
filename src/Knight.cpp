
#include "Knight.h"

Knight::Knight(const bool color, const int row, const char column, const bool isSelected)
        :Pieces(color, row, column, isSelected)
{
    //ctor

    pieceNotation = (pieceColor)? 'N' : 'n';

    pieceType = PieceType::KNIGHT;
}

Knight::~Knight()
{
    //dtor
}

void Knight::setMove(Board* board)
{
    Pieces* ptrPiece;

    auto checkAndSetMove=[&](int i, char j)
    {
        ptrPiece = board->getPiece(i, j);

        if(ptrPiece==nullptr || ptrPiece->getColor()!=pieceColor)
        {
            if(!board->isPinnedPiece(this, i, j))
                board->setAllowedMove(i, j);
        }

    };

    for(int i=pieceRow+2; i>pieceRow-5; i-=4)
        for(char j=pieceColumn+1; j>pieceColumn-2; j-=2)
            if(i>=Board::ROW_MIN&&i<=Board::ROW_MAX && j>=Board::COLUMN_MIN&&j<=Board::COLUMN_MAX)
                checkAndSetMove(i, j);


    for(char j=pieceColumn+2; j>pieceColumn-5; j-=4)
        for(int i=pieceRow+1; i>pieceRow-2; i-=2)
            if(i>=Board::ROW_MIN&&i<=Board::ROW_MAX && j>=Board::COLUMN_MIN&&j<=Board::COLUMN_MAX)
                checkAndSetMove(i, j);

}



void Knight::setThreat(Board* board)
{
    Pieces* ptrPiece;

    auto checkAndSetThreat=[&](int i, char j)
    {
        ptrPiece = board->getPiece(i, j);

        if(ptrPiece==nullptr)
            board->setThreatenedSquare(i, j);

        else if (ptrPiece->getColor()==pieceColor || (ptrPiece->getNotation()=='k'||ptrPiece->getNotation()=='K'))
            board->setThreatenedSquare(i, j);
    };

    for(int i=pieceRow+2; i>pieceRow-5; i-=4)
        for(char j=pieceColumn+1; j>pieceColumn-2; j-=2)
            if(i>=Board::ROW_MIN&&i<=Board::ROW_MAX && j>=Board::COLUMN_MIN&&j<=Board::COLUMN_MAX)
                checkAndSetThreat(i, j);


    for(char j=pieceColumn+2; j>pieceColumn-5; j-=4)
        for(int i=pieceRow+1; i>pieceRow-2; i-=2)
            if(i>=Board::ROW_MIN&&i<=Board::ROW_MAX && j>=Board::COLUMN_MIN&&j<=Board::COLUMN_MAX)
                checkAndSetThreat(i, j);

}


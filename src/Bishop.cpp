
#include "Bishop.h"

Bishop::Bishop(const bool color, const int row, const char column, const bool isSelected)
    :Pieces(color, row, column, isSelected)
{
    //ctor

    pieceNotation = (pieceColor)? 'B' : 'b';

    pieceType = PieceType::BISHOP;
}

Bishop::~Bishop()
{
    //dtor
}

void Bishop::setMove(Board* board)
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

        if(ptrPiece!=nullptr)
            return true;
        else
            return false;
    };


    {/*DIAGONAL MOVE*/
    for(int i=pieceRow+1, j=pieceColumn+1; i<=Board::ROW_MAX&&j<=Board::COLUMN_MAX; ++i, ++j)
        if(checkAndSetMove(i, j))
            break;

    for(int i=pieceRow+1, j=pieceColumn-1; i<=Board::ROW_MAX&&j>=Board::COLUMN_MIN; ++i, --j)
        if(checkAndSetMove(i, j))
            break;

    for(int i=pieceRow-1, j=pieceColumn+1; i>=Board::ROW_MIN&&j<=Board::COLUMN_MAX; --i, ++j)
        if(checkAndSetMove(i, j))
            break;

    for(int i=pieceRow-1, j=pieceColumn-1; i>=Board::ROW_MIN&&j>=Board::COLUMN_MIN; --i, --j)
        if(checkAndSetMove(i, j))
            break;
    }

}



void Bishop::setThreat(Board* board)
{

    Pieces* ptrPiece=nullptr;

    auto checkAnSetThreat=[&](int i, char j)
    {
        ptrPiece = board->getPiece(i, j);
        if(ptrPiece==nullptr)
            board->setThreatenedSquare(i, j);

        if(ptrPiece!=nullptr)
        {
            if(ptrPiece->getColor()==pieceColor)
            {
                board->setThreatenedSquare(i, j);
                return true;
            }
            else if (ptrPiece->getNotation()=='k'||ptrPiece->getNotation()=='K')
            {
                board->setThreatenedSquare(i, j);
            }
            else
            {
                return true;
            }
        }

        return false;

    };


    {/*DIAGONAL*/
    for(int i=pieceRow+1, j=pieceColumn+1; i<=Board::ROW_MAX&&j<=Board::COLUMN_MAX; ++i, ++j)
        if(checkAnSetThreat(i, j))
            break;

    for(int i=pieceRow+1, j=pieceColumn-1; i<=Board::ROW_MAX&&j>=Board::COLUMN_MIN; ++i, --j)
        if(checkAnSetThreat(i, j))
            break;

    for(int i=pieceRow-1, j=pieceColumn+1; i>=Board::ROW_MIN&&j<=Board::COLUMN_MAX; --i, ++j)
        if(checkAnSetThreat(i, j))
            break;

    for(int i=pieceRow-1, j=pieceColumn-1; i>=Board::ROW_MIN&&j>=Board::COLUMN_MIN; --i, --j)
        if(checkAnSetThreat(i, j))
            break;
    }

}




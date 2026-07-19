
#include "Queen.h"

Queen::Queen(const bool color, const int row, const char column, const bool isSelected)
        :Pieces(color, row, column, isSelected)
{
    //ctor

    pieceNotation = (pieceColor)? 'Q' : 'q';

    pieceType = PieceType::QUEEN;

}

Queen::~Queen()
{
    //dtor
}

void Queen::setMove(Board* board)
{
    Pieces* ptrPiece=nullptr;

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

    {/*HORIZONTAL MOVE*/
    for(char j=pieceColumn+1; j<=Board::COLUMN_MAX; ++j)
        if(checkAndSetMove(pieceRow, j))
            break;

    for(char j=pieceColumn-1; j>=Board::COLUMN_MIN; --j)
        if(checkAndSetMove(pieceRow, j))
            break;
    }


    {/*VERTICAL MOVE*/
    for(int i=pieceRow+1; i<=Board::ROW_MAX; ++i)
        if(checkAndSetMove(i, pieceColumn))
            break;

    for(int i=pieceRow-1; i>=Board::ROW_MIN; --i)
        if(checkAndSetMove(i, pieceColumn))
            break;
    }


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


void Queen::setThreat(Board* board)
{
    Pieces* ptrPiece;

    auto checkAndSetThreat=[&](int i, char j)
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

    {/*HORIZONTAL MOVE*/
    for(char j=pieceColumn+1; j<=Board::COLUMN_MAX; ++j)
        if(checkAndSetThreat(pieceRow, j))
            break;

    for(char j=pieceColumn-1; j>=Board::COLUMN_MIN; --j)
        if(checkAndSetThreat(pieceRow, j))
            break;
    }


    {/*VERTICAL MOVE*/
    for(int i=pieceRow+1; i<=Board::ROW_MAX; ++i)
        if(checkAndSetThreat(i, pieceColumn))
            break;

    for(int i=pieceRow-1; i>=Board::ROW_MIN; --i)
        if(checkAndSetThreat(i, pieceColumn))
            break;
    }


    {/*DIAGONAL MOVE*/
    for(int i=pieceRow+1, j=pieceColumn+1; i<=Board::ROW_MAX&&j<=Board::COLUMN_MAX; ++i, ++j)
        if(checkAndSetThreat(i, j))
            break;

    for(int i=pieceRow+1, j=pieceColumn-1; i<=Board::ROW_MAX&&j>=Board::COLUMN_MIN; ++i, --j)
        if(checkAndSetThreat(i, j))
            break;

    for(int i=pieceRow-1, j=pieceColumn+1; i>=Board::ROW_MIN&&j<=Board::COLUMN_MAX; --i, ++j)
        if(checkAndSetThreat(i, j))
            break;

    for(int i=pieceRow-1, j=pieceColumn-1; i>=Board::ROW_MIN&&j>=Board::COLUMN_MIN; --i, --j)
        if(checkAndSetThreat(i, j))
            break;
    }
}

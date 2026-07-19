
#include "King.h"

King::King(const bool color, const int row, const char column, const bool isSelected)
        :Pieces(color, row, column, isSelected)
{
    //ctor

    pieceNotation = (pieceColor)? 'K' : 'k';

    pieceType = PieceType::KING;
}

King::~King()
{
    //dtor
}



void King::setMove(Board* board)
{
    Pieces* ptrPiece;

    for(int j=-1; j<=1; ++j)
    {
        for (int i=1; i>=-1; --i)
        {
            ptrPiece = board->getPiece(pieceRow+i, pieceColumn+j);
            if(pieceRow+i>=Board::ROW_MIN&&pieceRow+i<=Board::ROW_MAX && pieceColumn+j>=Board::COLUMN_MIN&&pieceColumn+j<=Board::COLUMN_MAX)
            {
                    if(board->getPiece(pieceRow+i, pieceColumn+j)==nullptr||ptrPiece->getColor()!=pieceColor)
                    {
                        if(!getThreat(board, pieceRow+i, pieceColumn+j))
                            board->setAllowedMove(pieceRow+i, pieceColumn+j);
                    }
            }
        }

        /*CHECKS IF CASTLING IS POSSIBLE AND SETS THE MOVE IF IT IS.*/
        if(board->getPiece(pieceRow, pieceColumn+1)==nullptr&&board->getPiece(pieceRow, pieceColumn+2)==nullptr)
        {
            if(!getThreat(board, pieceRow, pieceColumn+1) && !getThreat(board, pieceRow, pieceColumn+2))
            {
                if(pieceColor)
                {
                    if(board->getCastlingWhiteKingside()&&!board->isCheck(pieceColor))
                        board->setAllowedMove(pieceRow, pieceColumn+2);

                }
                else
                {
                    if(board->getCastlingBlackKingside()&&!board->isCheck(pieceColor))
                        board->setAllowedMove(pieceRow, pieceColumn+2);
                }
            }
        }

        if(board->getPiece(pieceRow, pieceColumn-1)==nullptr&&board->getPiece(pieceRow, pieceColumn-2)==nullptr)
        {
            if(!getThreat(board, pieceRow, pieceColumn-1) && !getThreat(board, pieceRow, pieceColumn-2))
            {
                if(pieceColor)
                {
                    if(board->getCastlingWhiteQueenside()&&!board->isCheck(pieceColor))
                        board->setAllowedMove(pieceRow, pieceColumn-2);

                }
                else
                {
                    if(board->getCastlingBlackQueenside()&&!board->isCheck(pieceColor))
                        board->setAllowedMove(pieceRow, pieceColumn-2);
                }
            }
        }


    }
}

void King::setThreat(Board* board)
{

   Pieces* ptrPiece;

   for(int j=-1; j<=1; ++j)
    {
        for (int i=1; i>=-1; --i)
        {
            ptrPiece = board->getPiece(pieceRow+i, pieceColumn+j);

            if(pieceRow+i>=Board::ROW_MIN&&pieceRow+i<=Board::ROW_MAX && pieceColumn+j>=Board::COLUMN_MIN&&pieceColumn+j<=Board::COLUMN_MAX)
            {
                    if(board->getPiece(pieceRow+i, pieceColumn+j)==nullptr||ptrPiece->getColor()==pieceColor)
                    {
                        board->setThreatenedSquare(pieceRow+i, pieceColumn+j);
                    }
            }
        }
    }
}


bool King::getThreat(Board* board, int checkpieceRow, char checkColumn) const
{
    Pieces* ptrPiece = nullptr;
    Pieces* ptrKing = nullptr;
    bool threatened = false;

    for (int i=Board::ROW_MIN; i<=Board::ROW_MAX; ++i)
    {
        for (char j=Board::COLUMN_MIN; j<=Board::COLUMN_MAX; ++j)
        {
            ptrPiece = board->getPiece(i, j);

            if (ptrPiece)
            {
                if (ptrPiece->getColor() != pieceColor)
                {
                    ptrPiece->setThreat(board);

                    if (board->getThreatenedSquare(checkpieceRow, checkColumn))
                    {
                        board->removeThreatenedSquares();
                        threatened = true;
                        return threatened;
                    }
                }
                else if (ptrPiece->getNotation() == 'K' || ptrPiece->getNotation() == 'k')
                {
                    ptrKing = ptrPiece;
                    continue;
                }
            }
        }

    }
    board->removeThreatenedSquares();
    return threatened;
}









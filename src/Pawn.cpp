
#include "Pawn.h"

Pawn::Pawn(const bool color, const int row, const char column, const bool isSelected)
        :Pieces(color, row, column, isSelected)
{
    pieceNotation = (pieceColor)? 'P' : 'p';

    pieceType = PieceType::PAWN;
}

Pawn::~Pawn()
{
    //dtor
}


void Pawn::setMove(Board* board)
{
    Pieces* ptrPiece;

    if(pieceColor) /*WHITE PAWN.*/
    {

        /*CAPTURE.*/
        if (pieceColumn<=Board::COLUMN_MAX)
            ptrPiece = board->getPiece(pieceRow+1, pieceColumn+1);
        if (board->getPiece(pieceRow+1, pieceColumn+1)!=nullptr && !ptrPiece->getColor())
            if(pieceRow+1>=Board::ROW_MIN && pieceRow+1<=Board::ROW_MAX && pieceColumn+1>=Board::COLUMN_MIN && pieceColumn+1<=Board::COLUMN_MAX)
            {
                if(!board->isPinnedPiece(this, pieceRow+1, pieceColumn+1))
                    board->setAllowedMove(pieceRow+1, pieceColumn+1);
            }


        ptrPiece = board->getPiece(pieceRow+1, pieceColumn-1);
        if (board->getPiece(pieceRow+1, pieceColumn-1)!=nullptr && !ptrPiece->getColor())
            if(pieceRow+1<=Board::ROW_MAX && pieceColumn+1>=Board::COLUMN_MIN)
            {
                if(!board->isPinnedPiece(this, pieceRow+1, pieceColumn-1))
                    board->setAllowedMove(pieceRow+1, pieceColumn-1);
            }


        /*EN PASSANT.*/
        if(board->getPassantColumn()==pieceColumn+1 && pieceRow==5)
        {
            ptrPiece = board->getPiece(pieceRow+1, pieceColumn+1);
            if (board->getPiece(pieceRow+1, pieceColumn+1)==nullptr)
                    if(!board->isPinnedPiece(this, pieceRow+1, pieceColumn+1))
                        board->setAllowedMove(pieceRow+1, pieceColumn+1);
        }


        if(board->getPassantColumn()==pieceColumn-1 && pieceRow==5)
        {
            ptrPiece = board->getPiece(pieceRow+1, pieceColumn-1);
            if (board->getPiece(pieceRow+1, pieceColumn-1)==nullptr)
                    if(!board->isPinnedPiece(this, pieceRow+1, pieceColumn-1))
                        board->setAllowedMove(pieceRow+1, pieceColumn-1);
        }


        /*MOVE FORWARD.*/
        if (pieceRow==2)
        {
            if (board->getPiece(pieceRow+1, pieceColumn)==nullptr)
            {
                if(!board->isPinnedPiece(this, pieceRow+1, pieceColumn))
                    board->setAllowedMove(pieceRow+1, pieceColumn);

                if (board->getPiece(pieceRow+2, pieceColumn)==nullptr)
                {
                    if(!board->isPinnedPiece(this, pieceRow+2, pieceColumn))
                        board->setAllowedMove(pieceRow+2, pieceColumn);
                }
            }
        }
        else
        {
            if (board->getPiece(pieceRow+1, pieceColumn)==nullptr)
            {
                if(!board->isPinnedPiece(this, pieceRow+1, pieceColumn))
                    board->setAllowedMove(pieceRow+1, pieceColumn);
            }
        }
    }
    else /*BLACK PAWN.*/
    {
        /*CAPTURE.*/
        ptrPiece = board->getPiece(pieceRow-1, pieceColumn+1);
        if (board->getPiece(pieceRow-1, pieceColumn+1)!=nullptr && ptrPiece->getColor())
            if(pieceRow+1>=Board::ROW_MIN && pieceRow+1<=Board::ROW_MAX && pieceColumn+1>=Board::COLUMN_MIN && pieceColumn+1<=Board::COLUMN_MAX)
            {
                if(!board->isPinnedPiece(this, pieceRow-1, pieceColumn+1))
                    board->setAllowedMove(pieceRow-1, pieceColumn+1);

            }

        ptrPiece = board->getPiece(pieceRow-1, pieceColumn-1);
        if (board->getPiece(pieceRow-1, pieceColumn-1)!=nullptr && ptrPiece->getColor())
            if(pieceRow-1>=Board::ROW_MIN && pieceColumn-1>=Board::COLUMN_MIN)
            {
                if(!board->isPinnedPiece(this, pieceRow-1, pieceColumn-1))
                    board->setAllowedMove(pieceRow-1, pieceColumn-1);
            }

        /*EN PASSANT.*/
         if(board->getPassantColumn()==pieceColumn+1 && pieceRow==4)
        {
            ptrPiece = board->getPiece(pieceRow-1, pieceColumn+1);
            if (board->getPiece(pieceRow-1, pieceColumn+1)==nullptr)
                if(!board->isPinnedPiece(this, pieceRow-1, pieceColumn+1))
                    board->setAllowedMove(pieceRow-1, pieceColumn+1);
        }

        if(board->getPassantColumn()==pieceColumn-1 && pieceRow==4)
        {
            ptrPiece = board->getPiece(pieceRow-1, pieceColumn-1);
            if (board->getPiece(pieceRow-1, pieceColumn-1)==nullptr)
                if(!board->isPinnedPiece(this, pieceRow-1, pieceColumn-1))
                    board->setAllowedMove(pieceRow-1, pieceColumn-1);
        }

        /*MOVE FORWARD.*/
        if (pieceRow==7)
        {
            if (board->getPiece(pieceRow-1, pieceColumn)==nullptr)
            {
                if(!board->isPinnedPiece(this, pieceRow-1, pieceColumn))
                    board->setAllowedMove(pieceRow-1, pieceColumn);

                if (board->getPiece(pieceRow-2, pieceColumn)==nullptr)
                {
                    if(!board->isPinnedPiece(this, pieceRow-2, pieceColumn))
                        board->setAllowedMove(pieceRow-2, pieceColumn);
                }
            }
        }
        else
        {
            if (board->getPiece(pieceRow-1, pieceColumn)==nullptr)
            {
                if(!board->isPinnedPiece(this, pieceRow-1, pieceColumn))
                    board->setAllowedMove(pieceRow-1, pieceColumn);
            }
        }
    }
}



void Pawn::setThreat(Board* board)
{
    if(pieceColor)
    {

            if(pieceRow+1>=Board::ROW_MIN && pieceRow+1<=Board::ROW_MAX && pieceColumn+1>=Board::COLUMN_MIN && pieceColumn+1<=Board::COLUMN_MAX)
                board->setThreatenedSquare(pieceRow+1, pieceColumn+1);

            if(pieceRow+1>=Board::ROW_MIN && pieceRow+1<=Board::ROW_MAX && pieceColumn+1>=Board::COLUMN_MIN && pieceColumn+1<=Board::COLUMN_MAX)
                board->setThreatenedSquare(pieceRow+1, pieceColumn-1);

    }
    else
    {
            if(pieceRow+1>=Board::ROW_MIN && pieceRow+1<=Board::ROW_MAX && pieceColumn+1>=Board::COLUMN_MIN && pieceColumn+1<=Board::COLUMN_MAX)
                board->setThreatenedSquare(pieceRow-1, pieceColumn+1);

            if(pieceRow+1>=Board::ROW_MIN && pieceRow+1<=Board::ROW_MAX && pieceColumn+1>=Board::COLUMN_MIN && pieceColumn+1<=Board::COLUMN_MAX)
                board->setThreatenedSquare(pieceRow-1, pieceColumn-1);

    }
}











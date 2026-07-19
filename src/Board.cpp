
#include <Board.h>

Board::Board(const bool canCastlingWhiteKingSide, const bool canCastlingWhiteQueenSide,
             const bool canCastlingBlackKingSide, const bool canCastlingBlackQueenSide)
        : castlingWhiteKingSide(canCastlingWhiteKingSide),
          castlingWhiteQueenSide(canCastlingWhiteQueenSide),
          castlingBlackKingSide(canCastlingBlackKingSide),
          castlingBlackQueenSide(canCastlingBlackQueenSide)
{
    /*In the constructor, the characteristics of each square and rules for castling are initialized.*/

    /*The two for loops will traverse the board matrix.*/
    for (int row=0; row<8; ++row)
    {
        for (char column='a'; column<='h'; ++column)
        {
            /*Sets the color of the squares.*/
            board[row][column-'a'].squareColor=((row+column)%2 == 0)? WHITE : BLACK;
        }
    }
}



Board::~Board()
{
    //dtor

    /*Deletes all pointers related to pieces.*/
    for(int row=0; row<8; ++row)
    {
        for(char column='a'; column<='h'; ++column)
        {
            if(board[row][column-'a'].piece)
            {
                delete board[row][column-'a'].piece;
                board[row][column-'a'].piece=nullptr;
            }
        }

    }

    /*DELETES POINTERS RELATED TO CAPTURED PIECES.*/
    for (Pieces* piece : whiteCaptured)
        delete piece;

    for (Pieces* piece : blackCaptured)
        delete piece;

    whiteCaptured.clear();
    blackCaptured.clear();


    /*DELETES POINTERS RELATED TO PROMOTED PAWNS.*/
    for (Pieces* piece : whitePromoted)
        delete piece;

    for (Pieces* piece : blackPromoted)
        delete piece;

    whitePromoted.clear();
    blackPromoted.clear();

}



void Board::setPiece(Pieces* ptrPiece, int targetRow, char targetColumn)
{
    int previousRow = ptrPiece->getRow();
    char previousColumn = ptrPiece->getColumn();

    Pieces* capturedPiece;

    /*The conditional test bellow will check if the piece is changing its position and perform the necessary changes and verifications.
      Else, if it's the first time the piece is being placed on the board, the method will simply make the pointer point to it.*/
    if(targetRow >= ROW_MIN && targetRow <= ROW_MAX && targetColumn >= COLUMN_MIN && targetColumn <= COLUMN_MAX)
    if (ptrPiece->getRow() != NULL_INT || ptrPiece->getColumn() != NULL_CHAR)
    {

        if (board[targetRow-1][targetColumn-'a'].allowedMove)
        {
            /*REMOVE THE PIECE FROM PREVIOUS SQUARE.*/
            board[previousRow-1][previousColumn-'a'].piece=nullptr;


            /*REMOVE PAWN CAPTURED EN PASSANT.*/
            if((ptrPiece->getNotation()=='P'||ptrPiece->getNotation()=='p') &&
               (targetColumn==previousColumn+1||targetColumn==previousColumn-1) && getPiece(targetRow, targetColumn)==nullptr)
            {
                if(ptrPiece->getColor())
                    board[targetRow-2][targetColumn-'a'].piece=nullptr;
                else
                    board[targetRow][targetColumn-'a'].piece=nullptr;
            }


            /*MOVE ROOK AFTER CASTLING.*/
            if((ptrPiece->getNotation()=='K'||ptrPiece->getNotation()=='k')&&(targetColumn==previousColumn+2||targetColumn==previousColumn-2))
            {
                Pieces* castlingRook;

                if(targetColumn==previousColumn+2)
                {

                    castlingRook=board[targetRow-1]['h'-'a'].piece;
                    board[targetRow-1]['h'-'a'].piece=nullptr;
                    board[targetRow-1]['f'-'a'].piece=castlingRook;
                    castlingRook->setColumn('f');
                }

                if(targetColumn==previousColumn-2)
                {
                    castlingRook=board[targetRow-1]['a'-'a'].piece;
                    board[targetRow-1]['a'-'a'].piece=nullptr;
                    board[targetRow-1]['d'-'a'].piece=castlingRook;
                    castlingRook->setColumn('d');
                }
            }

            /*IF A PIECE WAS CAPTURED, MOVES THE CORRESPONDING OBJECT TO THE CAPTURED PIECES VECTOR.*/
            if(board[targetRow-1][targetColumn-'a'].piece)
            {
                capturedPiece = board[targetRow-1][targetColumn-'a'].piece;

                if(capturedPiece->getColor())
                    whiteCaptured.push_back(capturedPiece);
                else
                    blackCaptured.push_back(capturedPiece);
            }

            /*MOVE THE PIECE TO THE DESTINATION SQUARE.*/
            board[targetRow-1][targetColumn-'a'].piece=ptrPiece;

            /*SET NEW ROW AND COLUMN TO THE PIECE.*/
            ptrPiece->setRow(targetRow);
            ptrPiece->setColumn(targetColumn);


            /*CHECK CASTLING.*/
            if (ptrPiece->getNotation()=='K'||ptrPiece->getNotation()=='k')
            {
                if(ptrPiece->getColor())
                {
                    castlingWhiteKingSide=false;
                    castlingWhiteQueenSide=false;
                }
                else
                {
                    castlingBlackKingSide=false;
                    castlingBlackQueenSide=false;
                }
            }

            if(ptrPiece->getNotation()=='R'||ptrPiece->getNotation()=='r')
            {
                if(ptrPiece->getColor())
                {
                    if(previousRow==1&&previousColumn=='a')
                        castlingWhiteQueenSide=false;

                    if(previousRow==1&&previousColumn=='h')
                        castlingWhiteKingSide=false;
                }
                else
                {
                    if(previousRow==8&&previousColumn=='a')
                        castlingBlackQueenSide=false;

                    if(previousRow==8&&previousColumn=='h')
                        castlingBlackKingSide=false;
                }
            }
        }
    }
    else
    {
        ptrPiece->setRow(targetRow);
        ptrPiece->setColumn(targetColumn);
        board[targetRow-1][targetColumn-'a'].piece=ptrPiece;
    }


    /*CHECK AN PASSANT.*/
    if((ptrPiece->getNotation()=='p'&& targetRow==previousRow-2)||(ptrPiece->getNotation()=='P'&& targetRow==previousRow+2))
        checkPassant(ptrPiece);
}



void Board::setAllowedMove(const int row, const char column)
{
    board[row-1][column-'a'].allowedMove=true;
}



void Board::setThreatenedSquare(const int row, const char column)
{
    board[row-1][column-'a'].threatenedSquare=true;
}



void Board::setPassantColumn(const char column)
{
    passantColumn=column;
}



void Board::removePiece(const int row, const char column)
{
    board[row-1][column-'a'].piece=nullptr;
}




bool Board::checkAllowedMoves() const
{

    for (int i=0; i<8; ++i)
    {
        for (char j=97; j<105; ++j)
        {
            if(board[i][j-'a'].allowedMove)
            {
                return true;
            }
        }
    }

    return false;
}



void Board::removeThreatenedSquares()
{

    for (int i=0; i<8; ++i)
    {
        for (char j=97; j<105; ++j)
        {
            board[i][j-'a'].threatenedSquare=false;
        }

    }
}

void Board::removeAllowedMoves()
{

    for (int i=0; i<8; ++i)
    {
        for (char j=97; j<105; ++j)
        {
            board[i][j-'a'].allowedMove=false;
        }

    }
}

void Board::removeAllowedMove(const int row, const char column)
{
    board[row-1][column-'a'].allowedMove=false;
}

bool Board::getColor(const int row, const char column) const
{
    return board[row-1][column-'a'].squareColor;
}

Pieces* Board::getPiece(const int row, const char column) const
{
    return board[row-1][column-'a'].piece;
}



bool Board::getAllowedMove(const int row, const char column) const
{
    return board[row-1][column-'a'].allowedMove;
}

bool Board::getThreatenedSquare(const int row, const char column) const
{
    return board[row-1][column-'a'].threatenedSquare;
}

char Board::getPassantColumn() const
{
    return passantColumn;
}


bool Board::getTurnPassant() const
{
    return turnPassant;
}



void Board::setCastlingWhiteKingside(const bool isAllowed)
{
    castlingWhiteKingSide = isAllowed;
}

void Board::setCastlingWhiteQueenside(const bool isAllowed)
{
    castlingWhiteQueenSide = isAllowed;
}

void Board::setCastlingBlackKingside(const bool isAllowed)
{
    castlingBlackKingSide = isAllowed;
}

void Board::setCastlingBlackQueenside(const bool isAllowed)
{
    castlingBlackQueenSide = isAllowed;
}


bool Board::getCastlingWhiteKingside() const
{
    return castlingWhiteKingSide;
}

bool Board::getCastlingWhiteQueenside() const
{
    return castlingWhiteQueenSide;
}

bool Board::getCastlingBlackKingside() const
{
    return castlingBlackKingSide;
}

bool Board::getCastlingBlackQueenside() const
{
    return castlingBlackQueenSide;
}



void Board::checkPassant(const Pieces* ptrPiece)
{

    int row=ptrPiece->getRow();
    char column=ptrPiece->getColumn();

    Pieces* ptrCheck=nullptr;

    ptrCheck = getPiece(row, column-1);
    if(ptrCheck && (ptrCheck->getNotation()=='P'|| ptrCheck->getNotation()=='p'))
    {
        passantColumn=(ptrPiece->getColumn());
        turnPassant=ptrPiece->getColor();
    }



    ptrCheck = getPiece(row, column+1);
    if(ptrCheck!=nullptr && (ptrCheck->getNotation()=='P'||ptrCheck->getNotation()=='p'))
    {
        passantColumn=(ptrPiece->getColumn());
        turnPassant=ptrPiece->getColor();
    }

}




bool Board::isCheck(const bool pieceColor)
{
    Pieces* ptrPiece=nullptr;
    int row=NULL_INT;
    char column=NULL_CHAR;

    bool check;

    for(int i=0; i<8; ++i)
    {
        for(char j='a'; j<='h'; ++j)
        {
            ptrPiece=board[i][j-'a'].piece;

            if(ptrPiece)
            {
                if(ptrPiece->getColor()!=pieceColor)
                {
                    ptrPiece->setThreat(this);
                }
                else if(ptrPiece->getNotation()=='K'||ptrPiece->getNotation()=='k')
                {
                    row=ptrPiece->getRow();
                    column=ptrPiece->getColumn();
                }
            }
        }
    }

    if(getThreatenedSquare(row, column))
    {
        removeThreatenedSquares();
        check=true;
    }
    else
    {
        removeThreatenedSquares();
        check=false;
    }

    return check;

}

bool Board::isPinnedPiece(Pieces* ptrPiece, const int targetRow, const char targetColumn)
{
    bool check=false;

    int antRow=ptrPiece->getRow();
    char antColumn=ptrPiece->getColumn();

    Pieces* bkpPiece=nullptr;

    bkpPiece=board[targetRow-1][targetColumn-'a'].piece;

    board[antRow-1][antColumn-'a'].piece=nullptr;

    board[targetRow-1][targetColumn-'a'].piece=ptrPiece;
    ptrPiece->setRow(targetRow);
    ptrPiece->setColumn(targetColumn);

    check=isCheck(ptrPiece->getColor());

    board[antRow-1][antColumn-'a'].piece=ptrPiece;
    ptrPiece->setRow(antRow);
    ptrPiece->setColumn(antColumn);

    if(bkpPiece)
    {
        board[targetRow-1][targetColumn-'a'].piece=bkpPiece;
    }
    else
    {
        board[targetRow-1][targetColumn-'a'].piece=nullptr;
    }

    return check;

}

bool Board::isCheckMate(const bool color)
{
    Pieces* ptrPiece=nullptr;
    bool checkMate=false;

    for(int i=0; i<8; ++i)
    {
        for(char j='a'; j<='h'; ++j)
        {

            ptrPiece=board[i][j-'a'].piece;

            if(ptrPiece)
                if(ptrPiece->getColor()==color)
                    ptrPiece->setMove(this);
        }
    }

    if(!checkAllowedMoves())
        checkMate=true;

    removeAllowedMoves();

    return checkMate;
}



bool Board::willBeKingInCheck(Pieces* ptrPiece, const int targetRow, const char targetColumn)
{
    bool check=false;
    int antRow=ptrPiece->getRow();
    char antColumn=ptrPiece->getColumn();
    Pieces* bkpPiece=nullptr;

    bkpPiece=board[targetRow-1][targetColumn-'a'].piece;

    board[antRow-1][antColumn-'a'].piece=nullptr;

    board[targetRow-1][targetColumn-'a'].piece=ptrPiece;
    ptrPiece->setRow(targetRow);
    ptrPiece->setColumn(targetColumn);

    check=isCheck(!ptrPiece->getColor());

    board[antRow-1][antColumn-'a'].piece=ptrPiece;
    ptrPiece->setRow(antRow);
    ptrPiece->setColumn(antColumn);

    if(bkpPiece)
    {
        board[targetRow-1][targetColumn-'a'].piece=bkpPiece;
    }
    else
    {
        board[targetRow-1][targetColumn-'a'].piece=nullptr;
    }

    return check;

}


bool Board::isDoubleMove(Pieces* ptrPiece, const int targetRow, const char targetColumn)
{
    Pieces* chkPiece;

    removeAllowedMoves();

    for (int i=0; i<8; ++i)
    {
        for (char j=97; j<=104; ++j)
        {
                chkPiece=board[i][j-'a'].piece;

                if(chkPiece)
                {
                    if(chkPiece->getNotation()==ptrPiece->getNotation() && ptrPiece!=chkPiece)
                    {
                        chkPiece->setMove(this);

                        if(getAllowedMove(targetRow, targetColumn))
                        {
                            removeAllowedMoves();
                            ptrPiece->setMove(this);
                            return true;
                        }

                        removeAllowedMoves();
                    }
                }
            }
        }

    ptrPiece->setMove(this);
    return false;
}



const std::vector<Pieces*>& Board::getWhiteCaptured() const
{
    return whiteCaptured;
}

const std::vector<Pieces*>& Board::getBlackCaptured() const
{
    return blackCaptured;
}


const std::vector<Pieces*>& Board::getWhitePromoted() const
{
    return whitePromoted;
}

const std::vector<Pieces*>& Board::getBlackPromoted() const
{
    return blackPromoted;
}


void Board::popWhiteCaptured()
{
    whiteCaptured.pop_back();
}

void Board::popBlackCaptured()
{
    blackCaptured.pop_back();
}


void Board::popWhitePromoted()
{
    whitePromoted.pop_back();
}

void Board::popBlackPromoted()
{
    blackPromoted.pop_back();
}





void Board::setWhitePromoted(Pieces* promotedPawn)
{
    whitePromoted.push_back(promotedPawn);
}

void Board::setBlackPromoted(Pieces* promotedPawn)
{
    blackPromoted.push_back(promotedPawn);
}



std::string Board::generateFEN(const bool turn, const int halfMoveClock, const int fullMoveNumber)
{

    /*The FEN for the starting position of a chess game is:

    rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1

    */

    std::ostringstream fen;

    for (int row=7; row<0; --row)
    {
        int emptySquaresCount=0;

        for (char col='a'; col<='h'; ++col)
        {
            Square& square = board[row][col-'a'];

            if(square.piece==nullptr)
                ++emptySquaresCount;
            else
            {
                if(emptySquaresCount>0)
                {
                    fen << emptySquaresCount;
                    emptySquaresCount=0;
                }

                fen << square.piece->getNotation();
            }
        }

        if(emptySquaresCount>0)
            fen << emptySquaresCount;

        if(row != 0)
            fen << '/';

    }

    fen << ' ' << (turn? 'w' : 'b') << ' ';

    bool anyCastle=false;
    if(castlingWhiteKingSide) {fen << 'K'; anyCastle=true;}
    if(castlingWhiteQueenSide) {fen << 'Q'; anyCastle=true;}
    if(castlingBlackKingSide) {fen << 'k'; anyCastle=true;}
    if(castlingBlackQueenSide) {fen << 'q'; anyCastle=true;}
    if(!anyCastle) {fen << '-';}

    fen << ' ';

    if(passantColumn!=NULL_CHAR)
    {
        fen << passantColumn;

        if(turn)
            fen << '6';
        else
            fen << '3';
    }
    else
        fen << '-';

    fen << ' ' << halfMoveClock << ' ' << fullMoveNumber;

    return fen.str();

}









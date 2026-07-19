#include "MoveLog.h"

MoveLog::MoveLog()
{
    //ctor
}

MoveLog::~MoveLog()
{
    //dtor
}



void MoveLog::setMoveNotation(Board* board, const char previousColumn, const int previousRow, const char targetColumn, const int targeRow)
{
    Pieces* ptrPiece = board->getPiece(previousRow, previousColumn);

    Pieces* ptrTarget= board->getPiece(targeRow, targetColumn);

    char pieceNotation = toupper(ptrPiece->getNotation());

    std::string moveLog;

    /*CHECKS CASTLING*/
    if((pieceNotation=='K' || pieceNotation=='k') && ptrPiece->getColumn()+2==targetColumn)
    {
        moveLog += "O-O";
    }
    else if((pieceNotation=='K' || pieceNotation=='k') && ptrPiece->getColumn()-2==targetColumn)
    {
        moveLog += "O-O-O";
    }
    else
    {

        if(pieceNotation != 'P' && pieceNotation!= 'p') /*CHECKS IF PIECE ISNT A PAWN*/
        {
            moveLog += pieceNotation;

            if(board->isDoubleMove(ptrPiece, targetColumn, targeRow))
            {
                moveLog += previousColumn;
                moveLog += std::to_string(previousRow);
            }
        }

        /*CHECKS IF THE MOVE IS A CAPTURE*/
        if(ptrTarget)
        {
            if(pieceNotation == 'P' || pieceNotation == 'p')
            {
                if(board->isDoubleMove(ptrPiece, targetColumn, targeRow))
                {
                    moveLog += previousColumn;
                    moveLog += std::to_string(targeRow);
                }
                else
                moveLog += previousColumn;
            }

                moveLog += 'x';

        }else if(ptrTarget==nullptr && (pieceNotation == 'P' || pieceNotation == 'p')
            && (targetColumn==previousColumn+1 || targetColumn==previousColumn-1))
        {
            moveLog += previousColumn;
            moveLog += 'x';

            /*UNCOMMENT TO WRITE THE PASSANT NOTATION TO THE FILE.*/
            /*moveLog += columnDestin;
            moveLog += to_string(targeRow);
            moveLog += " e.p.";*/
        }
        /*else
        {*/ moveLog += targetColumn;
            moveLog += std::to_string(targeRow);
        /*}*/

    }


    /*SAVES MOVE*/
    if(turn)
    {
        whiteMoves.push_back(moveLog);
        //turn = !turn;
    }
    else
    {
        blackMoves.push_back(moveLog);
        moves++;
    }

}



void MoveLog::setMoveLogFile()
{
    time_t now = time(nullptr); /*CAPTURES THE CURRENT TIME TO USE IN THE FILE NAME.*/

    char fileName[100];

    /*GENERATES THE FILE NAME USING DATE AND TIME TO AVOID DUPLICATES.*/
    strftime(fileName, sizeof(fileName), "game_%Y%m%d%H%M%S.txt", localtime(&now));

    /*SETS FOLDER NAME TO SAVE GAMES.*/
    std::string folderPath = "Saved_games";

    /*CREATES THE FOLDER, IF DOESN'T EXIST.*/
    if(!std::filesystem::exists(folderPath))
        std::filesystem::create_directory(folderPath);


    /*SETS THE FILE PATH.*/
    std::string filePath = folderPath + "/" + fileName;


    try
    {
        /*TRIES TO CREATE THE FILE, THROW AN EXCEPTION IT IF FAILS.*/
        std::ofstream moveLogFile(filePath);

        /*CONFIGURES THE OBJECT OF STREAM TO THROW EXCEPTIONS.*/
        moveLogFile.exceptions(std::ofstream::failbit | std::ofstream::badbit);

        /*WRITES THE HEADER TO THE FILE.*/
        moveLogFile << std::left << std::setw(10) << "Nª";
        moveLogFile << std::left << std::setw(33) << "White moves";
        moveLogFile << std::left << std::setw(34) << "Black moves";
        moveLogFile << "\n";

        /*CAPTURE THE NUMBER OS MOVES.*/
        size_t maxSize = whiteMoves.size();

        for(size_t i=0; i<maxSize; ++i) /*USE THE NUMBER OF MOVES TO SET THE FOR LOOP LIMIT.*/
        {
            moveLogFile << std::left << std::setw(9) << i + 1;

            moveLogFile << std::left << std::setw(10) << whiteMoves[i];

            if(!whiteMovesTimes.empty())
                moveLogFile << " | " << std::left << std::setw(20) <<  whiteMovesTimes[i];
            else
                moveLogFile << std::left << std::setw(23) << " | ";


            if (i < blackMoves.size())
            {
                moveLogFile << std::left << std::setw(10) << blackMoves[i];

                if(!blackMovesTimes.empty())
                    moveLogFile << " | " << std::left << std::setw(20) << blackMovesTimes[i];
                else
                    moveLogFile << std::left << std::setw(20) << " | ";

                moveLogFile << "\n";
            }
            else
            {
                moveLogFile << "\n";
            }
        }
    }
    catch(const std::filesystem::filesystem_error& e) /*RE-TRHOWS THE CAUGHTS EXCEPTIONS.*/
    {
        throw;
    }
    catch(const std::ofstream::failure& e)
    {
        throw;
    }
    catch(const std::exception& e)
    {
        throw;
    }
}



void MoveLog::setPromotionNotation(char piecePromovedNotation)
{
    if(!turn)
    {
        if(!blackMoves.empty())
        {
            std::string& lastMove = blackMoves.back();

            if(!lastMove.empty())
            {
                lastMove.push_back('=');
                lastMove.push_back(toupper(piecePromovedNotation));

            }
        }
    }
    else
    {
        if(!whiteMoves.empty())
        {
            std::string& lastMove = whiteMoves.back();

            if(!lastMove.empty())
            {
                lastMove.push_back('=');
                lastMove.push_back(toupper(piecePromovedNotation));
            }
        }
    }
}


void MoveLog::setCheckNotation()
{
    if(!turn)
    {
        if(!blackMoves.empty())
        {
            std::string& lastMove = blackMoves.back();

            if(!lastMove.empty())
            {
                lastMove.push_back('+');
            }
        }
    }
    else
    {
        if(!whiteMoves.empty())
        {
            std::string& lastMove = whiteMoves.back();

            if(!lastMove.empty())
            {
                lastMove.push_back('+');
            }
        }
    }
}



void MoveLog::setCheckMateNotation()
{
    if(turn)
    {
        if(!blackMoves.empty())
        {
            std::string& lastMove = blackMoves.back();

            if(!lastMove.empty())
            {
                lastMove.back() = '#';
            }
        }
    }
    else
    {
        if(!whiteMoves.empty())
        {
            std::string& lastMove = whiteMoves.back();

            if(!lastMove.empty())
            {
                lastMove.back() = '#';
            }
        }
    }
}


void MoveLog::setMoveTime(const std::chrono::hours hour, const std::chrono::minutes minute, const std::chrono::seconds second)
{
    std::ostringstream stream;

    stream << std::setw(2) << std::setfill('0') << hour.count() << ":"
           << std::setw(2) << std::setfill('0') << minute.count() << ":"
           << std::setw(2) << std::setfill('0') << second.count();

    std::string formattedTime = stream.str();

    if(turn)
        whiteMovesTimes.push_back(formattedTime);
    else
        blackMovesTimes.push_back(formattedTime);
}


void MoveLog::setTurn(const bool turnColor)
{
    turn=turnColor;
}


const std::vector<std::string>& MoveLog::getWhiteMoves() const
{
    return whiteMoves;
}

const std::vector<std::string>& MoveLog::getBlackMoves() const
{
    return blackMoves;
}

int MoveLog::getNumberOfMoves() const
{
    return moves;
}

void MoveLog::popWhiteMoves()
{
    whiteMoves.pop_back();
}

void MoveLog::popBlackMoves()
{
    blackMoves.pop_back();
}

void MoveLog::decrementMoves()
{
    if(moves>1)
        --moves;
}

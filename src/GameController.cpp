
#include "GameController.h"

std::atomic<bool> interruption(false);

void signalHandler(int signal)
{
    interruption=true;
}



GameController::GameController(Interface* interface)
{
    //ctor

    if (setlocale(LC_ALL, "en_US.UTF-8") == NULL)
        interface->printError("Failed to set locale to en_US.UTF-8.");

    try /*Tries to initialize settings by creating the 'config.ini' file to save configurations.*/
    {
        settings = new Settings("config.ini");
    }
    catch(const std::ios_base::failure& e) /*Displays the cause of possible errors when creating the file.*/
    {
        char errorMessage[256];

        snprintf(errorMessage, sizeof(errorMessage),
         "Unable to save the configuration file:\n%s.\nSettings may be lost upon restart.",
         strerror(errno));

        interface->printError(errorMessage);
    }
    catch(std::exception& e)
    {
        interface->printError("Unable to save the configuration file.\nSettings may be lost upon restart.");
        getUserInputWithTimeout(10);
        interface->printError("Unable to save the configuration file.\nSettings may be lost upon restart.");
    }

    try
    {
        soundManager = new SoundManager();
    }
    catch(std::exception& e)
    {
        interface->printError(e.what());
    }

    signal(SIGHUP, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGINT, signalHandler);

}

GameController::~GameController()
{
    //dtor

    delete settings;
    delete soundManager;
}



void GameController::mainMenu(Interface* interface)
{

    int input=NULL_INT;

    while(input!=3 && !interruption)
    {
        /*Uses the interface object to display and return the user's selection.*/
        input = interface->getMenuSelection(interruption);

        if(input==1)
            newGame(interface);

        if(input==2)
            settingsControl(interface);

        if(input==3)
        {
            delete interface;
            break;
        }

    }

}


void GameController::newGame(Interface* interface)
{
    int logRow[2];
    char logColumn[2];
    /*These vectors store the coordinates of the source square and the destination square of each move
    to be sent to the move log.*/

    char promotion=NULL_CHAR;

    int clockTime=convertClockTime(),
        halfMoveClock=0,
        fullMoveNumber=1;

    bool gameOver = false,
         turn = WHITE,
         capture = false;

    std::atomic<bool> isMove = false,
                      isCapture = false,
                      isCheck = false,
                      mute = !settings->getSoundConfig();

    std::stack<MoveData> moveHistory;

    /*Variables to control the end of the game.*/
    GameOverFlags gameOverFlags;

    Board::Coordinates coordinates;

    Pieces* ptrPiece=nullptr;

    Pieces* selectedSquare=nullptr;

    Board* board = setNewgame();

    MoveLog* moveLog = new MoveLog();
    /*The move log is responsible for saving each move
    and writing them to a file at the end of the game,
    if the player requests it.*/

    Clock* clock = new Clock();

    /*Loads the configuration values from the file into the interface control variables so it knows what to display during the game.*/
    interface->setConfigurations(settings->getFlipBoardConfig(), settings->getUseUnicodeConfig(), settings->getSoundConfig(),
        settings->getCheckColorConfig(), settings->getAllowedMovesConfig(), settings->getClockTimeConfig());

    /*START CLOCK IN OTHER THREAD.*/
    std::thread tClock(&Clock::timer, clock, 60*clockTime, 60*clockTime, std::ref(gameOverFlags), std::ref(interruption));

    std::thread tSound(&SoundManager::gameSoundController, soundManager, std::ref(isMove), std::ref(isCapture), std::ref(isCheck), std::ref(mute), std::ref(gameOverFlags), std::ref(interruption));

    do /*Main game loop.*/
    {
        board->removeAllowedMoves();

        /*CHECK CHECKMATE OR DRAW.*/
        if(board->isCheckMate(turn))
        {
            if(board->isCheck(turn))
            {
                moveLog->setCheckMateNotation(); /*Sets checkmate notation.*/
                gameOverFlags.checkMate=true;
            }
            else
                gameOverFlags.draw=true;
        }

        if(halfMoveClock==150)
            gameOverFlags.draw=true;

        if(gameOverFlags.checkMate || gameOverFlags.resign || gameOverFlags.timesUp || gameOverFlags.draw || gameOverFlags.aborted)
            gameOver=true;


        /*SELECT PIECE.*/
        while(true)
        {
            /*Updates the screen and gets which piece the user selected to play.
            If the game has ended (if any of the variables that control the end of the game are true),
            this function will only update the screen with the game result.*/
            coordinates = interface->getSelectedSquare(board, moveLog, clock, gameOverFlags, interruption);


            /*CHECKS IF BACK THE MOVE WAS REQUEST.*/
            if(coordinates.column=='<' && !moveHistory.empty())
                break; /*Breaks the current loop to back the previous move in the next loop.*/

            /*CHECKS IF THERE WAS A RESIGNATION OR IF THE GAME WAS ABORTED.*/
            if(coordinates.column=='0')
                if(!moveLog->getWhiteMoves().empty() && !moveLog->getBlackMoves().empty())
                    gameOverFlags.resign=true;
                else
                    gameOverFlags.aborted=true;

            if(gameOverFlags.checkMate || gameOverFlags.resign || gameOverFlags.timesUp || gameOverFlags.draw || gameOverFlags.aborted|| interruption)
                break;


            if(coordinates.row >=Board::ROW_MIN && coordinates.row <=Board::ROW_MAX &&
               coordinates.column >=Board::COLUMN_MIN && coordinates.column <=Board::COLUMN_MAX)
                ptrPiece = board->getPiece(coordinates.row, coordinates.column);


            if(ptrPiece && ptrPiece->getColor()==turn)
            {
                ptrPiece->setMove(board);

                if(board->checkAllowedMoves())
                {
                    ptrPiece->setSelectedPiece(true);
                    logColumn[0]=coordinates.column;
                    logRow[0]=coordinates.row;

                    break;
                }
            }
        }

        /*SELECT THE DESTINATION SQUARE.*/
        while(!gameOverFlags.checkMate && !gameOverFlags.resign && !gameOverFlags.timesUp && !gameOverFlags.draw && !gameOverFlags.aborted && !interruption)
        {

            /*CHECKS IF BACK THE MOVE WAS REQUEST.*/
            if(coordinates.column=='<' && !moveHistory.empty())
            {
                MoveData previousMove = moveHistory.top();

                board->setAllowedMove(previousMove.previousRow, previousMove.previousCol);
                board->setPiece(previousMove.pieceMoved, previousMove.previousRow, previousMove.previousCol);
                board->removeAllowedMove(previousMove.targetRow, previousMove.targetCol);

                if(previousMove.isCapture)
                {
                    board->setAllowedMove(previousMove.targetRow, previousMove.targetCol);
                    board->setPiece(previousMove.capturedPiece, previousMove.targetRow, previousMove.targetCol);
                    board->removeAllowedMove(previousMove.targetRow, previousMove.targetCol);

                    if(previousMove.capturedPiece->getColor())
                        board->popWhiteCaptured();
                    else
                        board->popBlackCaptured();

                }

                if(previousMove.pieceMoved->getColor())
                {
                    moveLog->popWhiteMoves();

                }
                else
                {
                    moveLog->popBlackMoves();
                    moveLog->decrementMoves();
                }


                moveHistory.pop();

                turn=!turn;
                clock->setTurn(turn);
                moveLog->setTurn(turn);
                interface->setTurn(turn);
                board->removeAllowedMoves();

                /*REMOVES PASSANT.*/
                if(ptrPiece->getColor()!=board->getTurnPassant())
                    board->setPassantColumn(NULL_CHAR);

                ptrPiece->setSelectedPiece(false);

                break;
            }

            while(true)
            {
                coordinates=interface->getSelectedSquare(board, moveLog, clock, gameOverFlags, interruption);

                if(coordinates.column=='0' || coordinates.row==27 || gameOverFlags.timesUp)
                    break;

                if(coordinates.row >=1 && coordinates.row <=8 && coordinates.column >='a' && coordinates.column <='h')
                    selectedSquare=board->getPiece(coordinates.row, coordinates.column);

                if(selectedSquare && selectedSquare->getColor()==turn)
                {
                    board->removeAllowedMoves();

                    selectedSquare->setMove(board);

                    if(board->checkAllowedMoves())
                    {
                        ptrPiece->setSelectedPiece(false);
                        selectedSquare->setSelectedPiece(true);
                        ptrPiece=selectedSquare;
                    }
                    else
                        ptrPiece->setMove(board);

                }
                else if(!selectedSquare || selectedSquare->getColor()!=turn)
                    break;

            }

            /*CHECKS IF THERE WAS A RESIGNATION OR IF THE GAME WAS ABORTED.*/
            if(coordinates.column=='0')
            {
                if(!moveLog->getWhiteMoves().empty() && !moveLog->getBlackMoves().empty())
                    gameOverFlags.resign=true;
                else
                    gameOverFlags.aborted=true;

                break;
            }

            /*CANCEL MOVE.*/
            if(coordinates.row==27)
            {
                board->removeAllowedMoves();

                if(ptrPiece)
                    ptrPiece->setSelectedPiece(false);

                break;
            }

            /*SETTING PIECE ON THE SQUARE.*/
            if(board->getAllowedMove(coordinates.row, coordinates.column))
            {
                /*CHECKS IF IS A PROMOTION.*/
                if((ptrPiece->getNotation()=='P'&&coordinates.row==8)||(ptrPiece->getNotation()=='p'&&coordinates.row==1))
                {
                    promotion=interface->getPromotion(clock, gameOverFlags.timesUp, interruption);

                    /*CHECKS IF THERE WAS A RESIGNATION OR IF THE GAME WAS ABORTED DURING PROMOTION.*/
                    if(promotion=='0')
                        if(!moveLog->getWhiteMoves().empty() && !moveLog->getBlackMoves().empty())
                            gameOverFlags.resign=true;
                        else
                            gameOverFlags.aborted=true;

                    if(gameOverFlags.resign || gameOverFlags.timesUp || gameOverFlags.aborted)
                        break;

                    /*CANCEL PROMOTION.*/
                    if(promotion==27)
                    {
                        board->removeAllowedMoves();
                        ptrPiece->setSelectedPiece(false);
                        break;
                    }
                    else /*PROMOVES.*/
                    {
                        logColumn[1]=coordinates.column;
                        logRow[1]=coordinates.row;
                        moveLog->setMoveNotation(board, logColumn[0], logRow[0], logColumn[1], logRow[1]);

                        if(clockTime != 0)
                        {
                            if(turn)
                                moveLog->setMoveTime(clock->getWhiteHours(), clock->getWhiteMinutes(), clock->getWhiteSeconds());
                            else
                                moveLog->setMoveTime(clock->getBlackHours(), clock->getBlackMinutes(), clock->getBlackSeconds());
                        }

                        if(board->getPiece(coordinates.row, coordinates.column))
                            capture=true;

                        setPromotion(board, ptrPiece, coordinates.column, promotion);

                        ptrPiece = board->getPiece(coordinates.row, coordinates.column);
                        moveLog->setPromotionNotation(ptrPiece->getNotation());

                        if(board->isCheck(!turn))
                        {
                            moveLog->setCheckNotation();
                            isCheck=true;
                        }
                        else if(capture)
                            isCapture=true;
                        else
                            isMove=true;

                        halfMoveClock=0;

                        if(turn==BLACK)
                            ++fullMoveNumber;

                        turn=!turn;
                        clock->setTurn(turn);
                        moveLog->setTurn(turn);
                        interface->setTurn(turn);
                    }
                }
                else /*IF IS NOT A PROMOTION, MAKE A NORMAL MOVE.*/
                {
                    logColumn[1]=coordinates.column;
                    logRow[1]=coordinates.row;
                    moveLog->setMoveNotation(board, logColumn[0], logRow[0], logColumn[1], logRow[1]);

                    if(clockTime != 0)
                    {
                        if(turn)
                            moveLog->setMoveTime(clock->getWhiteHours(), clock->getWhiteMinutes(), clock->getWhiteSeconds());
                        else
                            moveLog->setMoveTime(clock->getBlackHours(), clock->getBlackMinutes(), clock->getBlackSeconds());
                    }

                    MoveData movePlayed;

                    movePlayed.pieceMoved = ptrPiece;
                    movePlayed.previousRow = logRow[0];
                    movePlayed.previousCol = logColumn[0];
                    movePlayed.targetRow = logRow[1];
                    movePlayed.targetCol = logColumn[1];

                    if(board->getPiece(coordinates.row, coordinates.column))
                    {
                        capture=true;
                        movePlayed.isCapture=true;
                        movePlayed.capturedPiece = board->getPiece(coordinates.row, coordinates.column);
                    }

                    board->setPiece(ptrPiece, coordinates.row, coordinates.column);


                    if(board->isCheck(!turn))
                    {
                        moveLog->setCheckNotation();
                        isCheck=true;
                    }
                    else if(capture)
                        isCapture=true;
                    else
                        isMove=true;

                    /*UPDATE HALF MOVE CLOCK.*/
                    if(capture || ptrPiece->getNotation()=='P' || ptrPiece->getNotation()=='p')
                        halfMoveClock=0;
                    else
                        ++halfMoveClock;

                    if(turn==BLACK)
                        ++fullMoveNumber;

                    ptrPiece->setSelectedPiece(false);
                    capture=false;

                    moveHistory.push(movePlayed);
                    turn=!turn;
                    clock->setTurn(turn);
                    moveLog->setTurn(turn);
                    interface->setTurn(turn);

                }

                board->removeAllowedMoves();

                /*REMOVES PASSANT.*/
                if(ptrPiece->getColor()!=board->getTurnPassant())
                    board->setPassantColumn(NULL_CHAR);

                ptrPiece->setSelectedPiece(false);

                break;

            }

        }

    }while(!gameOver && !interruption);

    /*Waits for the user to press a key after displaying the message that the game has ended due to time.
    This approach is used because the variable `timesUp`, when true, indicates the end of time for one of the players
    and causes the function responsible for displaying messages on the screen and capturing user input to terminate,
    abruptly closing the game loop.*/
    if(gameOverFlags.timesUp)
        getUserInputWithTimeout(5);

    if(ptrPiece)
        ptrPiece->setSelectedPiece(false);

    /*SAVES GAME.*/
    while(!interruption)
    {
        if(!moveLog->getBlackMoves().empty() && !moveLog->getWhiteMoves().empty()) /*Checks if both players have made a move.*/
        {
            if(interface->getConfirmation("Save game?", interruption))
            {
                try
                {
                    moveLog->setMoveLogFile();
                    break;
                }
                catch(const std::filesystem::filesystem_error& e)
                {
                    interface->printError("Unable to create the move log path. The game may be lost \nupon exit.");
                    getUserInputWithTimeout(10);
                }
                catch(const std::ios_base::failure& e)
                {
                    char errorMessage[256];

                    snprintf(errorMessage, sizeof(errorMessage),
                     "Unable to save the move log \nfile: %s. Settings may be lost upon restart.",
                     strerror(errno));
                    interface->printError(errorMessage);
                    getUserInputWithTimeout(10);
                }
                catch(const std::exception& e)
                {
                    interface->printError("Unable to save the move log \nfile. The game may be lost \nupon exit.");
                    getUserInputWithTimeout(10);
                }
            }
            else
                break;
        }
        else
            break;
    }

    tClock.join(); /*Clock thread termination.*/
    tSound.join();

    delete moveLog;
    delete clock;
    delete board;
}


void GameController::settingsControl(Interface* interface)
{
    /*Loads the configuration values from the file into the interface control variables so it displays the last valid settings on the settings screen.*/
    interface->setConfigurations(settings->getFlipBoardConfig(), settings->getUseUnicodeConfig(), settings->getSoundConfig(),
                                 settings->getCheckColorConfig(), settings->getAllowedMovesConfig(), settings->getClockTimeConfig());

    /*Gets the user's configuration selection.*/
    Settings::SettingValues ControllerSettingValues = interface->getSettings(interruption);

    /*Writes the settings saved by the user to the file so that they can be loaded after the application restarts.*/
    settings->setClockTimeConfig(ControllerSettingValues.clockTime);
    settings->setAutoFlipBoardConfig(ControllerSettingValues.autoFlipBoard);
    settings->setUseUnicodeConfig(ControllerSettingValues.useUnicodes);
    settings->setSoundConfig(ControllerSettingValues.sound);
    settings->setCheckColorConfig(ControllerSettingValues.checkColor);
    settings->setAllowedMovesConfig(ControllerSettingValues.allowedMoves);

    try /*Tries write the configuration settings to the file.*/
    {
        settings->setConfigFile();
    }
    catch(const std::exception& e)
    {
        interface->printError("Unable to save the configuration file.\nSettings may be lost upon restart.");
        getUserInputWithTimeout(10);
    }
}



Board* GameController::setNewgame()
{
    Board* board = new Board(Board::castlingWhiteKingSideAllowed, Board::castlingWhiteQueenSideAllowed,
                             Board::castlingBlackKingSideAllowed, Board::castlingBlackQueenSideAllowed);

    /*DECLARES VECTORS OF WHITE PIECES.*/
    std::vector<Pieces*> whitePawns(8);
    std::vector<Pieces*> whiteKing(1);
    std::vector<Pieces*> whiteQueen(1);
    std::vector<Pieces*> whiteRooks(2);
    std::vector<Pieces*> whiteBishops(2);
    std::vector<Pieces*> whiteKnights(2);

    /*CONFIGURES WHITE PIECES*/

    char col=Board::COLUMN_MIN;

    for(Pieces*& pawn : whitePawns)
    {
        pawn = createPiece(Pieces::PAWN, WHITE);

        //board->setPiece(pawn, 2, col);

        ++col;
    }

    for(Pieces* king : whiteKing)
    {
        king = createPiece(Pieces::KING, WHITE);

        //board->setPiece(king, 1, 'e');
    }

    for(Pieces* queen : whiteQueen)
    {
        queen = createPiece(Pieces::QUEEN, WHITE);

        //board->setPiece(queen, 1, 'd');
    }


    col=Board::COLUMN_MIN;
    for(Pieces* rook : whiteRooks)
    {
        rook = createPiece(Pieces::ROOK, WHITE);

        //board->setPiece(rook, 1, col);

        col+=7;
    }

    col=Board::COLUMN_MIN+2;
    for(Pieces* bishop : whiteBishops)
    {
        bishop = createPiece(Pieces::BISHOP, WHITE);

        //board->setPiece(bishop, 1, col);

        col+=3;
    }

    col=Board::COLUMN_MIN+1;
    for(Pieces* knight : whiteKnights)
    {
        knight = createPiece(Pieces::KNIGHT, WHITE);

        //board->setPiece(knight, 1, col);

        col+=5;
    }



    /*DECLARES VECTORS OF BLACK PIECES.*/
    std::vector<Pieces*> blackPawns(8);
    std::vector<Pieces*> blackKing(1);
    std::vector<Pieces*> blackQueen(1);
    std::vector<Pieces*> blackRooks(2);
    std::vector<Pieces*> blackBishops(2);
    std::vector<Pieces*> blackKnights(2);

    /*CONFIGURES BLACK PIECES*/
    col=Board::COLUMN_MIN;
    for(Pieces* pawn : blackPawns)
    {
        pawn = createPiece(Pieces::PAWN, BLACK);

        //board->setPiece(pawn, 7, col);

        col++;
    }

    for(Pieces* king : blackKing)
    {
        king = createPiece(Pieces::KING, BLACK);

        //board->setPiece(king, 8, 'e');
    }

    for(Pieces* queen : blackQueen)
    {
        queen = createPiece(Pieces::QUEEN, BLACK);

        //board->setPiece(queen, 8, 'd');
    }


    col=Board::COLUMN_MIN;
    for(Pieces* rook : blackRooks)
    {
        rook = createPiece(Pieces::ROOK, BLACK);

        //board->setPiece(rook, 8, col);

        col+=7;
    }

    col=Board::COLUMN_MIN+2;
    for(Pieces* bishop : blackBishops)
    {
        bishop = createPiece(Pieces::BISHOP, BLACK);

        //board->setPiece(bishop, 8, col);

        col+=3;
    }

    col=Board::COLUMN_MIN+1;
    for(Pieces* knight : blackKnights)
    {
        knight = createPiece(Pieces::KNIGHT, BLACK);

        //board->setPiece(knight, 8, col);

        col+=5;

    }




//    int j=NULL_INT;
//
//
//    /*CONFIGURES WHITE PIECES.*/
//    for(int i=0; i<8; ++i)
//        pawn[i] = new Pawn(WHITE, 2, 97+i, false);
//
//    king[0] = new King(WHITE, 1, 'e', false);
//
//
//    j=0;
//    for(int i=0; i<2; ++i)setNewgame
//    {
//        rook[i] = new Rook(WHITE, 1, 97+j, false);
//        j+=7;
//    }
//
//    j=0;
//    for(int i=0; i<2; ++i)
//    {
//        bishop[i] = new Bishop(WHITE, 1, 99+j, false);
//        j+=3;
//    }
//
//    queen[0] = new Queen(WHITE, 1, 'd', false);
//
//    j=0;
//    for(int i=0; i<2; ++i)
//    {
//        knight[i] = new Knight(WHITE, 1, 98+j, false);
//        j+=5;
//    }
//
//
//    /*CONFIGURES BLACK PIECES.*/
//    j=0;
//    for(int i=8; i<16; ++i)
//    {
//        pawn[i] = new Pawn(BLACK, 7, 97+j, false);
//        ++j;
//    }
//
//    king[1] = new King(BLACK, 8, 'e', false);
//
//    j=0;
//    for(int i=2; i<4; ++i)
//    {
//        rook[i] = new Rook(BLACK, 8, 97+j, false);
//        j+=7;
//    }
//
//    j=0;
//    for(int i=2; i<4; ++i)
//    {
//        bishop[i] = new Bishop(BLACK, 8, 99+j, false);
//        j+=3;
//
//    }
//
//    queen[1] = new Queen(BLACK, 8, 'd', false);
//
//    j=0;
//    for(int i=2; i<4; ++i)
//    {
//        knight[i] = new Knight(BLACK, 8, 98+j, false);
//        j+=5;
//    }
//
//
//    /*SETS PIECES.*/
//    for(Pawn* pawn : pawn)
//        board->setPiece(pawn, pawn->getRow(), pawn->getColumn());
//
//    for(King* king : king)
//        board->setPiece(king, king->getRow(), king->getColumn());
//
//    for(Rook* rook : rook)
//        board->setPiece(rook, rook->getRow(), rook->getColumn());
//
//    for(Bishop* bishop : bishop)
//        board->setPiece(bishop, bishop->getRow(), bishop->getColumn());
//
//    for(Queen* queen : queen)
//        board->setPiece(queen, queen->getRow(), queen->getColumn());
//
//    for(Knight* knight : knight)
//        board->setPiece(knight, knight->getRow(), knight->getColumn());
//
//
//    /*RETURNS A POINTER TO THE BOARD WITH THE PIECES POSITIONED FOR A NEW GAME.*/
    return board;

}

void GameController::setPromotion(Board* board, Pieces* promotedPawn, const char columnPromotion, const char promotionPieceNotation)
{
    Pieces* promotedPiece=nullptr;

    switch (promotionPieceNotation)
    {
        case 'Q':
        case 'q':
            promotedPiece = createPiece(Pieces::QUEEN, promotedPawn->getColor());
            break;

        case 'R':
        case 'r':
            promotedPiece = createPiece(Pieces::ROOK, promotedPawn->getColor());

        case 'B':
        case 'b':
            promotedPiece = createPiece(Pieces::BISHOP, promotedPawn->getColor());
            break;

        case 'N':
        case 'n':
            promotedPiece = createPiece(Pieces::KNIGHT, promotedPawn->getColor());
            break;

    }

    /*.*/
    if(promotedPawn->getColor())
        board->setWhitePromoted(promotedPawn);
    else
        board->setBlackPromoted(promotedPawn);

    board->removePiece(promotedPawn->getRow(), promotedPawn->getColumn());

    /*SETS THE NEW SELECTED PIECE ON THE BOARD*/
    board->setPiece(promotedPiece, promotedPiece->getColor()? 8 : 1, columnPromotion);

}

int GameController::convertClockTime()
{
    switch(settings->getClockTimeConfig())
    {
        case 0: return 0;

        case 1: return 1;

        case 2: return 3;

        case 3: return 10;

        case 4: return 15;

        case 5: return 30;

        case 6: return 60;
    }
}

bool GameController::getUserInputWithTimeout(int timeoutInSeconds)
{
    int input;

    /*File descriptor to stdin.*/
    int fd = fileno(stdin);
    fd_set set;
    struct timeval timeout;

    /*Initializes the set of descriptors and the timeout.*/
    FD_ZERO(&set);
    FD_SET(fd, &set);

    /*Sets the timeout (seconds and microseconds) for user input.*/
    timeout.tv_sec = timeoutInSeconds;
    timeout.tv_usec = 0;

    /*Checks if there is input available on stdin within the timeout.*/
    int result = select(fd + 1, &set, NULL, NULL, &timeout);

    if (result > 0)
    {
        std::cin.clear();
        std::cin.get();
        return true;
    }
    else if (result == 0)
    {
        return false;
    }
    else
    {
        perror("select");
        return false;
    }
}

Pieces* GameController::createPiece(const Pieces::PieceType pieceType, const bool pieceColor)
{
    Pieces* newPiece;

    switch(pieceType)
    {
        case Pieces::KING:
             newPiece = new King(pieceColor, NULL_INT, NULL_CHAR, false);
             break;

        case Pieces::QUEEN:
             newPiece = new Queen(pieceColor, NULL_INT, NULL_CHAR, false);
             break;

        case Pieces::ROOK:
             newPiece = new Rook(pieceColor, NULL_INT, NULL_CHAR, false);
             break;

        case Pieces::BISHOP:
             newPiece = new Bishop(pieceColor, NULL_INT, NULL_CHAR, false);
             break;

        case Pieces::KNIGHT:
             newPiece = new Knight(pieceColor, NULL_INT, NULL_CHAR, false);
             break;

        case Pieces::PAWN:
             newPiece = new Pawn(pieceColor, NULL_INT, NULL_CHAR, false);
             break;

    }

    return newPiece;
}

void GameController::loadFEN(const std::string& fen)
{
    int halfMoveClock,
        fullMoveNumber,

        row = 7,
        col = 0;

    std::istringstream fenStream(fen);

    std::string boardPart,
                turnPart,
                castlingPart,
                passantPart;


    fenStream >> boardPart >> turnPart >> castlingPart >> passantPart;


        for(char c : boardPart)
        {
            if (c== '/')
            {
                --row;
                col=0;
            }
            else if(isdigit(c))
            {
                col+=c-'0';
            }
            else
            {
                bool color = isupper(c);
                char pieceType = tolower(c);

                /*Creates the correlated piece. */
            }
        }
}










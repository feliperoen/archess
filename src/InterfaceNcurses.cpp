
#include "InterfaceNcurses.h"


/*Controls the screen resizing when the window is maximized or the font is enlarged.*/
std::atomic<bool> screenResized(false);

void handleWinch(int signal)
{
    screenResized = true;
}



InterfaceNcurses::InterfaceNcurses()
{
    //ctor

    setlocale(LC_ALL, "en_US.UTF-8");
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS, NULL);
    noecho();
    curs_set(0);

    start_color();

    init_color(COLOR_DARKGRAY, 260, 260, 260);
    init_color(COLOR_LIGHTGRAY, 500, 500, 500);
    init_color(COLOR_MATTEWHITE, 800, 800, 800);
    init_color(COLOR_LIGHTWHITE, 999, 999, 999);\

    init_pair(1, COLOR_BLACK, COLOR_DARKGRAY);
    init_pair(2, COLOR_BLACK, COLOR_LIGHTGRAY);
    init_pair(3, COLOR_BLACK, COLOR_MATTEWHITE);

    init_pair(4, COLOR_CYAN, COLOR_DARKGRAY);
    init_pair(5, COLOR_CYAN, COLOR_LIGHTGRAY);

    init_pair(8, COLOR_LIGHTWHITE, COLOR_BLACK);
    init_pair(9, COLOR_LIGHTWHITE, COLOR_DARKGRAY);
    init_pair(10, COLOR_LIGHTWHITE, COLOR_LIGHTGRAY);

    init_pair(11, COLOR_RED, COLOR_BLACK);
    init_pair(12, COLOR_RED, COLOR_DARKGRAY);
    init_pair(13, COLOR_RED, COLOR_LIGHTGRAY);

    signal(SIGWINCH, handleWinch);

    refresh();

}

InterfaceNcurses::~InterfaceNcurses()
{
    //dtor

    destroySettingsWindow();
    destroyNewGameWindow();
    destroyMenuWindow();
    endwin();
}



int InterfaceNcurses::getMenuSelection(const std::atomic<bool>& interruption)
{
    int input,
        yButton,
        xButton;

    if(newGame_win)
        destroyNewGameWindow();

    if(settings_win)
        destroySettingsWindow();

    if(!menu_win)
        createMenuWindow();

    printMenuWindow();

    nodelay(stdscr, TRUE);

    while(true)
    {
        input=getch();

        if(screenResized)
        {
            screenResized = false;
            createMenuWindow();
            printMenuWindow();
        }

        if(input!=ERR || interruption)
            break;

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    nodelay(stdscr, FALSE);

    if(interruption)
        return 3;


    if(getmouse(&event) == OK)
    {
        if(event.bstate & BUTTON1_PRESSED)
        {

        }

        if(event.bstate & BUTTON4_PRESSED)
        {
             if(menuSelecter>0)
                --menuSelecter;
        }

        if(event.bstate & BUTTON5_PRESSED)
        {
            if(menuSelecter<menuItems.size()-1)
                ++menuSelecter;
        }
    }



    switch (input)
    { //Controls the cursor based on the arrow keys pressed and formatting.


        case KEY_UP:
            if(menuSelecter>0)
                menuSelecter--;
            break;

        case KEY_DOWN:
            if(menuSelecter<menuItems.size()-1)
                menuSelecter++;
            break;

        case '\n':
            if(menuSelecter==0)
            {
                destroyMenuWindow();
                createNewGameWindow();

                return 1;
            }

            if(menuSelecter==1)
            {
                destroyMenuWindow();
                createSettingsWindow();

                return 2;
            }

            if(menuSelecter==2)
                return 3;

    }

    return 0;


}

Settings::SettingValues InterfaceNcurses::getSettings(const std::atomic<bool>& interruption)
{

    int input,
        listBegin=15,
        optionsBegin=45,
        arrowSelecter=4;

    Settings::SettingValues displaySettingValues;

    displaySettingValues.clockTime=settingValues.clockTime;
    displaySettingValues.autoFlipBoard=settingValues.autoFlipBoard;
    displaySettingValues.useUnicodes=settingValues.useUnicodes;
    displaySettingValues.sound=settingValues.sound;
    displaySettingValues.checkColor=settingValues.checkColor;
    displaySettingValues.allowedMoves=settingValues.allowedMoves;

    while(true)
    {

        if(screenResized)
        {
            screenResized = false;
            createSettingsWindow();
            printSettingsWindow();
        }

        mvwprintw(settings_win, 1, 30, "SETTINGS");

        if(displaySettingValues.clockTime==0)
            mvwprintw(settings_win, 4, optionsBegin, "Off");

        if(displaySettingValues.clockTime==1)
            mvwprintw(settings_win, 4, optionsBegin, "1 minute");

        if(displaySettingValues.clockTime==2)
            mvwprintw(settings_win, 4, optionsBegin, "3 minutes");

        if(displaySettingValues.clockTime==3)
            mvwprintw(settings_win, 4, optionsBegin, "10 minutes");

        if(displaySettingValues.clockTime==4)
            mvwprintw(settings_win, 4, optionsBegin, "15 minutes");

        if(displaySettingValues.clockTime==5)
            mvwprintw(settings_win, 4, optionsBegin, "30 minutes");

        if(displaySettingValues.clockTime==6)
            mvwprintw(settings_win, 4, optionsBegin, "1 hour");

        if(displaySettingValues.autoFlipBoard)
            mvwprintw(settings_win, 6, optionsBegin, "Auto");
        else
            mvwprintw(settings_win, 6, optionsBegin, "Manual");

        if(displaySettingValues.useUnicodes)
            mvwprintw(settings_win, 8, optionsBegin, "On");
        else
            mvwprintw(settings_win, 8, optionsBegin, "Off");

        if(displaySettingValues.sound)
            mvwprintw(settings_win, 10, optionsBegin, "On");
        else
            mvwprintw(settings_win, 10, optionsBegin, "Off");

        if(displaySettingValues.checkColor)
            mvwprintw(settings_win, 12, optionsBegin, "On");
        else
            mvwprintw(settings_win, 12, optionsBegin, "Off");

        if(displaySettingValues.allowedMoves)
            mvwprintw(settings_win, 14, optionsBegin, "On");
        else
            mvwprintw(settings_win, 14, optionsBegin, "Off");


        printSettingsWindow();


        nodelay(stdscr, TRUE);

        while(true)
        {
            input=getch();

            if(input!=ERR || interruption)
                break;

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        nodelay(stdscr, FALSE);

        if(interruption)
        {
            displaySettingValues.clockTime=settingValues.clockTime;
            displaySettingValues.autoFlipBoard=settingValues.autoFlipBoard;
            displaySettingValues.useUnicodes=settingValues.useUnicodes;
            displaySettingValues.sound=settingValues.sound;
            displaySettingValues.checkColor=settingValues.checkColor;
            displaySettingValues.allowedMoves=settingValues.allowedMoves;

            werase(confirmation_win);
            wrefresh(confirmation_win);

            return displaySettingValues;
        }

        if(getmouse(&event) == OK)
        {
            if(event.bstate & BUTTON1_PRESSED)
            {

            }

            if(event.bstate & BUTTON4_PRESSED)
            {
                 if(menuSelecter>0)
                    --menuSelecter;
            }

            if(event.bstate & BUTTON5_PRESSED)
            {
                if(menuSelecter<menuItems.size()-1)
                    ++menuSelecter;
            }
        }

        switch(input)
        {
            case KEY_UP:
                if(menuSelecter==0)
                {
                    menuSelecter=6;
                }
                else if(menuSelecter>0)
                {
                    menuSelecter-=1;
                }
                break;

            case KEY_DOWN:
                if(menuSelecter==6)
                {
                    menuSelecter=0;
                }
                else if(menuSelecter<6)
                {
                    menuSelecter+=1;
                }
                break;

            case KEY_RIGHT:
                if(menuSelecter==0)
                {
                    if(displaySettingValues.clockTime==6)
                        displaySettingValues.clockTime=0;
                    else if(displaySettingValues.clockTime<6)
                        displaySettingValues.clockTime++;
                }

                if(menuSelecter==1)
                    displaySettingValues.autoFlipBoard = !displaySettingValues.autoFlipBoard;

                if(menuSelecter==2)
                    displaySettingValues.useUnicodes = !displaySettingValues.useUnicodes;

                if(menuSelecter==3)
                    displaySettingValues.sound = !displaySettingValues.sound;

                if(menuSelecter==4)
                    displaySettingValues.checkColor = !displaySettingValues.checkColor;

                if(menuSelecter==5)
                    displaySettingValues.allowedMoves = !displaySettingValues.allowedMoves;

                break;

            case KEY_LEFT:
                if(menuSelecter==0)
                {
                    if(displaySettingValues.clockTime==0)
                        displaySettingValues.clockTime=6;
                    else if(displaySettingValues.clockTime>0)
                        displaySettingValues.clockTime--;
                }

                if(menuSelecter==1)
                    displaySettingValues.autoFlipBoard = !displaySettingValues.autoFlipBoard;

                if(menuSelecter==2)
                    displaySettingValues.useUnicodes = !displaySettingValues.useUnicodes;

                if(menuSelecter==3)
                    displaySettingValues.sound = !displaySettingValues.sound;

                if(menuSelecter==4)
                    displaySettingValues.checkColor = !displaySettingValues.checkColor;

                if(menuSelecter==5)
                    displaySettingValues.allowedMoves = !displaySettingValues.allowedMoves;

                break;

            case KEY_F(2):
                if(getConfirmation("Reset settings to default values?", interruption))
                {

                    displaySettingValues.clockTime=3;
                    displaySettingValues.autoFlipBoard=false;
                    displaySettingValues.useUnicodes=true;
                    displaySettingValues.sound=true;
                    displaySettingValues.checkColor=true;
                    displaySettingValues.allowedMoves=true;

                    break;
                }

            case '\n':
                if(menuSelecter==6)
                {
                    if(getConfirmation("Save the settings before exiting?", interruption))
                    {
                        werase(confirmation_win);
                        wrefresh(confirmation_win);

                        return displaySettingValues;

                    }
                    else
                    {
                        displaySettingValues.clockTime=settingValues.clockTime;
                        displaySettingValues.autoFlipBoard=settingValues.autoFlipBoard;
                        displaySettingValues.useUnicodes=settingValues.useUnicodes;
                        displaySettingValues.sound=settingValues.sound;
                        displaySettingValues.checkColor=settingValues.checkColor;
                        displaySettingValues.allowedMoves=settingValues.allowedMoves;

                        werase(confirmation_win);
                        wrefresh(confirmation_win);

                        return displaySettingValues;
                    }
                }
        }

    }
}

Board::Coordinates InterfaceNcurses::getSelectedSquare(Board* board, MoveLog* moveLog, Clock* clock,
    GameOverFlags& gameOverFlags, std::atomic<bool>& interruption)
{

    Board::Coordinates coordinates;

    int input;

    if(settingValues.autoFlipBoard)
        boardPerspective=playerTurn;

    Button resignButton = {1, 8, yBoard+19, xBoard+19};
    Button flipBoardButton = {1, 12, yBoard+19, xBoard};

    do{
        printNewGameWindow(board, moveLog, gameOverFlags);

        nodelay(stdscr, TRUE);

        while(true)
        {
            input=getch();

            if(screenResized)
            {
                screenResized = false;
                createNewGameWindow();
                printNewGameWindow(board, moveLog, gameOverFlags);
            }

            if(settingValues.clockTime != 0)
                printClock(clock);


            if(gameOverFlags.timesUp || input!=ERR || interruption)
                break;

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        nodelay(stdscr, FALSE);

        if(gameOverFlags.timesUp || interruption)
        {
            nodelay(stdscr, FALSE);
            coordinates.row=27;
            coordinates.column=NULL_CHAR;
            return coordinates;
        }


        if (getmouse(&event) == OK)
        {
            if(event.bstate & BUTTON1_PRESSED)
            {
                if(checkClickButton(resignButton, event.y, event.x))
                {
                    if(!gameOverFlags.checkMate && !gameOverFlags.resign && !gameOverFlags.timesUp && !gameOverFlags.draw && !gameOverFlags.aborted)
                    {
                        coordinates.column='0';
                        return coordinates;
                    }
                }
                else if(checkClickButton(flipBoardButton, event.y, event.x))
                {
                    boardPerspective = !boardPerspective;

                    yWhiteClock = boardPerspective? 10 : -1;
                    yBlackClock = boardPerspective? -1 : 10;

                    yBlackCaptured = boardPerspective? 10 : -1;
                    yWhiteCaptured = boardPerspective? -1 : 10;

                    mvwin(white_clock, yBoard+yWhiteClock, xBoard+16);
                    mvwin(black_clock, yBoard+yBlackClock, xBoard+16);

                    mvwin(blackCaptured_win, yBoard+yBlackCaptured, xBoard);
                    mvwin(whiteCaptured_win, yBoard+yWhiteCaptured, xBoard);

                }
                else
                {
                    coordinates.row=convertY(event.y);
                    coordinates.column=convertX(event.x);
                    return coordinates;
                }
            }
            else if(BUTTON1_PRESSED || BUTTON1_RELEASED)
            {
                if(!gameOverFlags.checkMate && !gameOverFlags.resign && !gameOverFlags.timesUp && !gameOverFlags.draw && !gameOverFlags.aborted)
                {
                    coordinates.row=convertY(event.y);
                    coordinates.column=convertX(event.x);
                    return coordinates;
                }
            }


        }

        switch (input)
        { //Controls the cursor based on the arrow keys pressed and formatting.

            case KEY_UP:
                if(playerTurn)
                {
                    if(boardPerspective)
                    {
                        if(yWhiteCursor>yBoard+1)
                            yWhiteCursor--;
                    }
                    else
                    {
                        if(yWhiteCursor<yBoard+8)
                            yWhiteCursor++;
                    }
                }
                else
                {
                    if(boardPerspective)
                    {
                        if(yBlackCursor>yBoard+1)
                            yBlackCursor--;
                    }
                    else
                    {
                        if(yBlackCursor<yBoard+8)
                            yBlackCursor++;
                    }
                }

                break;


            case KEY_DOWN:
                if(playerTurn)
                {
                    if(boardPerspective)
                    {
                        if(yWhiteCursor<yBoard+8)
                            yWhiteCursor++;
                    }
                    else
                    {
                        if(yWhiteCursor>yBoard+1)
                            yWhiteCursor--;
                    }
                }
                else
                {
                    if(boardPerspective)
                    {
                        if(yBlackCursor<yBoard+8)
                            yBlackCursor++;
                    }
                    else
                    {
                        if(yBlackCursor>yBoard+1)
                            yBlackCursor--;
                    }
                }

                break;


            case KEY_LEFT:
                if(playerTurn)
                {
                    if(boardPerspective)
                    {
                        if(xWhiteCursor>xBoard+4)
                            xWhiteCursor-=3;
                    }
                    else
                    {
                        if(xWhiteCursor<xBoard+22)
                            xWhiteCursor+=3;
                    }
                }
                else
                {
                    if(boardPerspective)
                    {
                        if(xBlackCursor>xBoard+4)
                            xBlackCursor-=3;
                    }
                    else
                    {
                        if(xBlackCursor<xBoard+22)
                            xBlackCursor+=3;
                    }
                }

                break;


            case KEY_RIGHT:
                if(playerTurn)
                {
                    if(boardPerspective)
                    {
                        if(xWhiteCursor<xBoard+22)
                            xWhiteCursor+=3;
                    }
                    else
                    {
                        if(xWhiteCursor>xBoard+4)
                            xWhiteCursor-=3;
                    }
                }
                else
                {
                    if(boardPerspective)
                    {
                        if(xBlackCursor<xBoard+22)
                            xBlackCursor+=3;
                    }
                    else
                    {
                        if(xBlackCursor>xBoard+4)
                            xBlackCursor-=3;
                    }
                }

                break;


            case 27:
                coordinates.row=27;

                return coordinates;

            case KEY_F(2):
                boardPerspective = !boardPerspective;

                yWhiteClock = boardPerspective? 10 : -1;
                yBlackClock = boardPerspective? -1 : 10;

                yBlackCaptured = boardPerspective? 10 : -1;
                yWhiteCaptured = boardPerspective? -1 : 10;

                mvwin(white_clock, yBoard+yWhiteClock, xBoard+16);
                mvwin(black_clock, yBoard+yBlackClock, xBoard+16);

                mvwin(blackCaptured_win, yBoard+yBlackCaptured, xBoard);
                mvwin(whiteCaptured_win, yBoard+yWhiteCaptured, xBoard);

                break;

            case KEY_F(4):
                coordinates.column='0';

                return coordinates;

            case KEY_PPAGE:
                coordinates.column='>';

                return coordinates;

            case KEY_NPAGE:
                coordinates.column='<';

                return coordinates;

            case '\n':
                if(playerTurn)
                {
                    coordinates.row=convertY(yWhiteCursor);
                    coordinates.column=convertX(xWhiteCursor);

                }
                else
                {
                    coordinates.row=convertY(yBlackCursor);
                    coordinates.column=convertX(xBlackCursor);
                }

                return coordinates;
        }

    }while(input!=27);
}

char InterfaceNcurses::getPromotion(Clock* clock, const std::atomic<bool>& timesUp, const std::atomic<bool>& interruption)
{

    wattron(promotion_win, COLOR_PAIR(3) | A_BOLD);

    int selecter=16,
        input,

        yButton=NULL_INT,
        xButton=NULL_INT;

    char promotedTo = NULL_CHAR;

    /*
        King: ♔ (\u2654), ♚ (\u265A)
        Queen: ♕ (\u2655), ♛ (\u265B)
        Rook: ♖ (\u2656), ♜ (\u265C)
        Bishop: ♗ (\u2657), ♝ (\u265D)
        Knight: ♘ (\u2658), ♞ (\u265E)
        Pawn: ♙ (\u2659), ♟ (\u265F)
    */

    getbegyx(promotion_win, yButton, xButton);

    Button queenButton =  {1, 1, yButton, xButton+16},
           rookButton =   {1, 1, yButton, xButton+19},
           bishopButton = {1, 1, yButton, xButton+22},
           knightButton = {1, 1, yButton, xButton+25},
           resignButton = {1, 8, yBoard+19, xBoard+19};


    do{

        mvwprintw(promotion_win, 0, 0, "  Select piece: ");

        if(settingValues.useUnicodes)
        {
            if(selecter==16)
                wattron(promotion_win, A_BLINK);
            mvwprintw(promotion_win, 0, 16, "\u2655  ");
            wattroff(promotion_win, A_BLINK);

            if(selecter==19)
                wattron(promotion_win, A_BLINK);
            mvwprintw(promotion_win, 0, 19, "\u2656  ");
            wattroff(promotion_win, A_BLINK);

            if(selecter==22)
                wattron(promotion_win, A_BLINK);
            mvwprintw(promotion_win, 0, 22, "\u2657  ");
            wattroff(promotion_win, A_BLINK);

            if(selecter==25)
                wattron(promotion_win, A_BLINK);
            mvwprintw(promotion_win, 0, 25, "\u2658  ");
            wattroff(promotion_win, A_BLINK);
        }
        else
        {
            if(selecter==16)
            wattron(promotion_win, A_BLINK);
            mvwprintw(promotion_win, 0, 16, "Q  ");
            wattroff(promotion_win, A_BLINK);

            if(selecter==19)
            wattron(promotion_win, A_BLINK);
            mvwprintw(promotion_win, 0, 19, "R  ");
            wattroff(promotion_win, A_BLINK);

            if(selecter==22)
                wattron(promotion_win, A_BLINK);
            mvwprintw(promotion_win, 0, 22, "B  ");
            wattroff(promotion_win, A_BLINK);

            if(selecter==25)
                wattron(promotion_win, A_BLINK);
            mvwprintw(promotion_win, 0, 25, "N  ");
            wattroff(promotion_win, A_BLINK);
        }


        wrefresh(promotion_win);

        nodelay(stdscr, TRUE);

        while(true)
        {
            if(settingValues.clockTime !=0)
                printClock(clock);

            input=getch();

            if(timesUp || input!=ERR || interruption)
                break;

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        nodelay(stdscr, FALSE);

        if(timesUp || interruption)
            return 27;


        if(getmouse(&event) == OK)
        {
            if(event.bstate & BUTTON1_PRESSED)
            {
                if(checkClickButton(queenButton, event.y, event.x))
                {
                    if(selecter==16)
                    {
                        werase(promotion_win);
                        wrefresh(promotion_win);
                        return 'Q';
                    }
                    else
                        selecter=16;
                }
                else if(checkClickButton(rookButton, event.y, event.x))
                {
                    if(selecter==19)
                    {
                        werase(promotion_win);
                        wrefresh(promotion_win);
                        return 'R';
                    }
                    else
                        selecter=19;

                }
                else if(checkClickButton(bishopButton, event.y, event.x))
                {
                    if(selecter==22)
                    {
                        werase(promotion_win);
                        wrefresh(promotion_win);
                        return 'B';
                    }
                    else
                        selecter=22;
                }
                else if(checkClickButton(knightButton, event.y, event.x))
                {
                    if(selecter==25)
                    {
                        werase(promotion_win);
                        wrefresh(promotion_win);
                        return 'N';
                    }
                    else
                        selecter=25;
                }
                else if(checkClickButton(resignButton, event.y, event.x))
                {
                    return '0';

                }

            }

            if(event.bstate & BUTTON4_PRESSED)
            {
                if(selecter<25)
                        selecter+=3;
                    else
                        selecter=16;
            }

            if(event.bstate & BUTTON5_PRESSED)
            {
                 if(selecter>16)
                        selecter-=3;
                    else
                        selecter=25;
            }
        }

        switch (input)
        { //Controls the cursor based on the arrow keys pressed and formatting.

            case KEY_RIGHT:
                    if(selecter<25)
                        selecter+=3;
                    else
                        selecter=16;

                    break;

            case KEY_LEFT:
                    if(selecter>16)
                        selecter-=3;
                    else
                        selecter=25;

                break;

            case KEY_F(4):
                werase(promotion_win);

                wrefresh(promotion_win);

                return '0';

            case 27:
                werase(promotion_win);

                wrefresh(promotion_win);

                return input;

            case '\n':
                if(selecter==16)
                    promotedTo='Q';

                if(selecter==19)
                    promotedTo='R';

                if(selecter==22)
                    promotedTo='B';

                if(selecter==25)
                    promotedTo='N';

                werase(promotion_win);

                wrefresh(promotion_win);

                input=27;
                return promotedTo;
        }


    }while(input!=27);


}

bool InterfaceNcurses::getConfirmation(const std::string confirmQuery, const std::atomic<bool>& interruption)
{
    int input=NULL_INT,
        yButton,
        xButton;

    bool x;

    getbegyx(confirmation_win, yButton, xButton);

    Button yesButton = {1, 5, yButton, xButton+confirmQuery.size()+6},
           noButton  = {1, 4, yButton, xButton+confirmQuery.size()+1};



    do
    {
        mvwprintw(confirmation_win, 0, 0, "%s", confirmQuery.c_str());

        if(x==0)
        {
            wattron(confirmation_win, COLOR_PAIR(3) | A_BOLD);
            mvwprintw(confirmation_win, 0, confirmQuery.size()+1, " NO ");
            wattroff(confirmation_win, COLOR_PAIR(3) | A_BOLD);

            mvwprintw(confirmation_win, 0, confirmQuery.size()+6, " YES ");
        }
        else
        {
            mvwprintw(confirmation_win, 0, confirmQuery.size()+1, " NO ");

            wattron(confirmation_win, COLOR_PAIR(3) | A_BOLD);
            mvwprintw(confirmation_win, 0, confirmQuery.size()+6, " YES ");
            wattroff(confirmation_win, COLOR_PAIR(3) | A_BOLD);
        }

        wrefresh(confirmation_win);

        nodelay(stdscr, TRUE);

        while(true)
        {

            input=getch();

            if(input!=ERR || interruption)
                break;

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        if (getmouse(&event) == OK)
        {
            if(event.bstate & BUTTON1_CLICKED)
            {
                if(checkClickButton(noButton, event.y, event.x))
                {
                    if(x==false)
                        return false;
                    else
                        x=false;
                }
                else if(checkClickButton(yesButton, event.y, event.x))
                {
                    if(x==true)
                        return true;
                    else
                        x=true;
                }
            }
            else if ((event.bstate & BUTTON4_PRESSED) || (event.bstate & BUTTON5_PRESSED))
            {
                x=!x;
            }
        }


        if(interruption)
            return false;

        switch(input)
        {
            case KEY_RIGHT:
                if(x==false)
                    x=true;
                else
                    x=false;
                break;

            case KEY_LEFT:
                if(x!=0)
                    x=false;
                else
                    x=true;
                break;

            case 27:
                return 0;

            case '\n':
                return x;
        }

    }while(input!=27);
}



void InterfaceNcurses::printEndGame(GameOverFlags& gameOverFlags)
{
    gameOver_win = derwin(newGame_win, 1, 28, yBoard+12, xBoard);

    wattron(gameOver_win, COLOR_PAIR(3) | A_BOLD);

    if(gameOverFlags.draw)
            mvwprintw(gameOver_win, 0, 0, "         DRAW GAME          ");

    if(gameOverFlags.checkMate)
        if(playerTurn)
            mvwprintw(gameOver_win, 0, 0, "  BLACK WINS BY CHECKMATE   ");
        else
            mvwprintw(gameOver_win, 0, 0, "  WHITE WINS BY CHECKMATE   ");

    if(gameOverFlags.resign)
        if(!playerTurn)
            mvwprintw(gameOver_win, 0, 0, "    WHITE WINS BY RESIGN    ");
        else
            mvwprintw(gameOver_win, 0, 0, "    BLACK WINS BY RESIGN    ");

    if(gameOverFlags.timesUp)
        if(!playerTurn)
            mvwprintw(gameOver_win, 0, 0, "     WHITE WINS BY TIME     ");
        else
            mvwprintw(gameOver_win, 0, 0, "     BLACK WINS BY TIME     ");

    if(gameOverFlags.aborted)
            mvwprintw(gameOver_win, 0, 0, "        GAME ABORTED        ");

    wrefresh(gameOver_win);
    delwin(gameOver_win);
}

void InterfaceNcurses::printError(const std::string errorMessage)
{
    if(!menu_win && !newGame_win && !settings_win)
        createMenuWindow();

    wattron(error_win, COLOR_PAIR(11) | A_BOLD);
    mvwprintw(error_win, 0, 0, "%s", errorMessage.c_str());
    wattroff(error_win, COLOR_PAIR(11) | A_BOLD);

    wrefresh(newGame_win);
    wrefresh(error_win);
}


void InterfaceNcurses::printMenuWindow()
{
    wrefresh(menu_win);
    werase(menu_win);

    printMenuHeader();


    Board* boardMenu = new Board(false, false, false, false);
    printBoard(boardMenu);
    delete boardMenu;

    printMenuItems();
}

void InterfaceNcurses::printNewGameWindow(Board* board, MoveLog* moveLog, GameOverFlags& gameOverFlags)
{
    wrefresh(newGame_win);

    printBoard(board);
    printMoveLog(moveLog);
    printBlackCaptured(board);
    printWhiteCaptured(board);
    printInstructions();

    if(gameOverFlags.timesUp || gameOverFlags.checkMate || gameOverFlags.resign ||  gameOverFlags.draw || gameOverFlags.aborted)
        printEndGame(gameOverFlags);
}

void InterfaceNcurses::printSettingsWindow()
{
    wrefresh(settings_win);
    werase(settings_win);

    printMenuItems();
    printInstructions();
}

void InterfaceNcurses::printMenuHeader()
{
    wattron(menuHeader_win, COLOR_PAIR(8) | A_BOLD);

          mvwprintw(menuHeader_win, 0, 4,"_              ____   _");
        mvwprintw(menuHeader_win, 1, 3,"/ \\     _ __   / ___| | |__     ___   ___   ___");
       mvwprintw(menuHeader_win, 2, 2,"/ _ \\   | '__| | |     | '_ \\   / _ \\ \/ __| / __|");
      mvwprintw(menuHeader_win, 3, 1,"/ ___ \\  | |    | |___  | | | | |  __/ \\__ \\ \\__ \\");
     mvwprintw(menuHeader_win, 4, 0,"/_/   \\_\\ |_|     \\____| |_| |_|  \\___| |___/ |___/");
     mvwprintw(menuHeader_win, 6, 0," A simple, lightweight chess emulator.");


        mvwprintw(menuHeader_win, 0, 57,"__");
       mvwprintw(menuHeader_win, 1, 56,"(  )");
        mvwprintw(menuHeader_win, 2, 57,"><");
       mvwprintw(menuHeader_win, 3, 56,"|  |");
      mvwprintw(menuHeader_win, 4, 55,"/    \\");
     mvwprintw(menuHeader_win, 5, 54,"|______|");

     wattroff(menuHeader_win, COLOR_PAIR(8) | A_BOLD);

    wrefresh(menuHeader_win);

}

void InterfaceNcurses::printMenuItems()
{

        for(int i=0; i<menuItems.size(); ++i)
        {
            mvwprintw(list_win, (i==0? i:i+i), 2,"%s", menuItems[i].c_str());
        }

        mvwprintw(list_win, (menuSelecter==0? menuSelecter:menuSelecter+menuSelecter), 0, ">");

        wrefresh(list_win);

}

void InterfaceNcurses::printBoard(Board* board)
{
    int row=0,
        column=NULL_INT;

    char piece,
         columnBegin = (boardPerspective)? Board::COLUMN_MIN : Board::COLUMN_MAX,
         columnEnd = (boardPerspective)? Board::COLUMN_MAX : Board::COLUMN_MIN,
         columnStep = (boardPerspective)? 1 : -1;

    Pieces* ptrPiece;


    /*MAKING THE BOX OF THE BOARD*/
    {
    wattron(board_win, COLOR_PAIR(3) | A_BOLD);

    mvwprintw(board_win, 0, 0, "                            ");

    for(int i=0; i<=9; ++i)
    {
        mvwprintw(board_win, i, 0, " ");
    }


    for(int i=1, k=(boardPerspective? 8 : 1); i<=9; ++i, k+=(boardPerspective? -1 : 1))
    {
        mvwprintw(board_win, i, 1, "%d", k);
    }

    for(int i=0; i<=9; ++i)
    {
        mvwprintw(board_win, i, 26, " ");
    }


    for(int i=0; i<=9; ++i)
    {
        mvwprintw(board_win, i, 27, " ");
    }

    if(boardPerspective)
        mvwprintw(board_win, 9, 0, "   a  b  c  d  e  f  g  h   ");
    else
        mvwprintw(board_win, 9, 0, "   h  g  f  e  d  c  b  a   ");
    }


    /*PRINTING THE SQUARES E SHOWING THE PIECES*/
    for (char k=columnBegin; (boardPerspective? k<=columnEnd : k>=columnEnd); k+=columnStep, row=row+3)
    {
       column = (boardPerspective)? 1 : 8;

       for (int j=1, l=8; j<9, l>0; ++j, l--)
        {
            if(playerTurn)
            {
                if(convertX(xWhiteCursor)==k && convertY(yWhiteCursor)==l)
                    wattron(board_win, A_UNDERLINE);
            }
            else
            {
                if(convertX(xBlackCursor)==k && convertY(yBlackCursor)==l)
                    wattron(board_win, A_UNDERLINE);
            }

            ptrPiece = board->getPiece(l, k);

            if(ptrPiece)
            {
                if(ptrPiece->getSelected()&&ptrPiece->getColor()==playerTurn)
                    wattron(board_win, A_BLINK);
            }

            //DEFINES SQUARE COLOR
            if (board->getColor(j, k))
            {
                wattron(board_win, COLOR_PAIR(1) | A_BOLD);

                if (settingValues.allowedMoves && board->getAllowedMove(l, k))
                    wattron(board_win, COLOR_PAIR(4) | A_BOLD);

                if(ptrPiece)
                    if(settingValues.checkColor&&(ptrPiece->getNotation()=='k'||ptrPiece->getNotation()=='K')&&board->isCheck(ptrPiece->getColor()))
                        wattron(board_win, COLOR_PAIR(12) | A_BOLD);
            }

            else
            {
                wattron(board_win, COLOR_PAIR(2) | A_BOLD);

                if (settingValues.allowedMoves && board->getAllowedMove(l, k))
                    wattron(board_win, COLOR_PAIR(5) | A_BOLD);

                if(ptrPiece)
                    if(settingValues.checkColor&&(ptrPiece->getNotation()=='k'||ptrPiece->getNotation()=='K')&&board->isCheck(ptrPiece->getColor()))
                        wattron(board_win, COLOR_PAIR(13) | A_BOLD);
            }


            if (settingValues.allowedMoves && ptrPiece==nullptr && board->getAllowedMove(l, k))
            {
                piece='o';
            }
            else
            {
                if(ptrPiece!=nullptr)//If it's not NULL, the variable to be displayed receives the piece.
                    piece = ptrPiece->getNotation();
                else
                    piece= ' ';
            }


            //Displays the square with the piece and formatting
            mvwprintw(board_win, column, row+2, " ");

            if(has_colors()&&settingValues.useUnicodes)
            {
                if(ptrPiece && ptrPiece->getColor())
                {
                    if (board->getColor(j, k))
                    {
                        wattron(board_win, COLOR_PAIR(9) | A_BOLD);

                        if (settingValues.allowedMoves && board->getColor(j, k)&&board->getAllowedMove(l, k))
                            wattron(board_win, COLOR_PAIR(4) | A_BOLD);

                        if(ptrPiece)
                            if(settingValues.checkColor&&(ptrPiece->getNotation()=='k'||ptrPiece->getNotation()=='K')&&board->isCheck(ptrPiece->getColor()))
                                wattron(board_win, COLOR_PAIR(12) | A_BOLD);
                    }

                    else
                    {
                        wattron(board_win, COLOR_PAIR(10) | A_BOLD);

                        if (settingValues.allowedMoves && board->getAllowedMove(l, k))
                            wattron(board_win, COLOR_PAIR(5) | A_BOLD);

                        if(ptrPiece)
                            if(settingValues.checkColor&&(ptrPiece->getNotation()=='k'||ptrPiece->getNotation()=='K')&&board->isCheck(ptrPiece->getColor()))
                                wattron(board_win, COLOR_PAIR(13) | A_BOLD);
                    }
                }

                mvwprintw(board_win, column, row+3, "%s", convertToUnicode(piece).c_str());

            }
            else
                mvwprintw(board_win, column, row+3, "%c", piece);

            if(board->getColor(j, k))
                wattron(board_win, COLOR_PAIR(1) | A_BOLD);
            else
                wattron(board_win, COLOR_PAIR(2) | A_BOLD);

            mvwprintw(board_win, column, row+4, " ");

            wattroff(board_win, A_UNDERLINE);
            wattroff(board_win, A_BLINK);

            column += boardPerspective? 1 : -1;

        }

    }

    wrefresh(board_win);
}

void InterfaceNcurses::printClock(Clock* clock)
{

    werase(white_clock);
    werase(black_clock);


    if(clock->getWhiteMinutes().count()==0 && clock->getWhiteSeconds().count()<20)
    {
        wattron(white_clock, COLOR_PAIR(11) | A_BOLD);
        mvwprintw(white_clock, 0, 0,"%02d:%02d:%02d:%03d",
                 clock->getWhiteHours().count(),
                 clock->getWhiteMinutes().count(),
                 clock->getWhiteSeconds().count(),
                 clock->getWhiteMilliseconds().count());

    }
    else
    {
        wattron(white_clock, COLOR_PAIR(0) | A_BOLD);
        mvwprintw(white_clock, 0, 0,"    %02d:%02d:%02d",
                 clock->getWhiteHours().count(),
                 clock->getWhiteMinutes().count(),
                 clock->getWhiteSeconds().count());
                 //clock->getWhiteMilliseconds().count());
    }


    if(clock->getBlackMinutes().count()==0 && clock->getBlackSeconds().count()<20)
    {
        wattron(black_clock, COLOR_PAIR(11) | A_BOLD);
        mvwprintw(black_clock, 0, 0,"%02d:%02d:%02d:%03d",
                 clock->getBlackHours().count(),
                 clock->getBlackMinutes().count(),
                 clock->getBlackSeconds().count(),
                 clock->getBlackMilliseconds().count());

    }
    else
    {
        wattron(black_clock, COLOR_PAIR(0) | A_BOLD);
        mvwprintw(black_clock, 0, 0,"    %02d:%02d:%02d",
                 clock->getBlackHours().count(),
                 clock->getBlackMinutes().count(),
                 clock->getBlackSeconds().count());
                 //clock->getWhiteMilliseconds().count());
    }

    wrefresh(white_clock);
    wrefresh(black_clock);

}

void InterfaceNcurses::printMoveLog(MoveLog* moveLog)
{
    int j=2,
        movesToShow=moveLogHeight-4;

    int init = (moveLog->getWhiteMoves().size()>movesToShow)? (moveLog->getWhiteMoves().size()-movesToShow) : 0;

    werase(moveLog_win);

    box(moveLog_win, 0, 0);

    mvwprintw(moveLog_win, 0, 3, " Nª | White Moves | Black Moves ");

    for(int i=1; i<moveLogHeight-1; ++i)
        mvwprintw(moveLog_win, i, 4, "   |             |");

    for(int i= init; i<moveLog->getWhiteMoves().size(); ++i)
    {
        mvwprintw(moveLog_win, j, 3, "%d.", i+1);
        mvwprintw(moveLog_win, j, 10, "      ");
        mvwprintw(moveLog_win, j, 11, "%s", moveLog->getWhiteMoves()[i].c_str());
        ++j;
    }


    j=2;
    if(moveLog->getWhiteMoves().size()>moveLog->getBlackMoves().size())
    {
        init = (moveLog->getBlackMoves().size()>(movesToShow-1))? (moveLog->getBlackMoves().size()-(movesToShow-1)) : 0;

        if(moveLog->getBlackMoves().size()>0)
            for(int i= init; i<moveLog->getBlackMoves().size(); ++i)
            {
                mvwprintw(moveLog_win, j, 24, "      ");
                mvwprintw(moveLog_win, j, 25, "%s", moveLog->getBlackMoves()[i].c_str());
                ++j;
            }

            mvwprintw(moveLog_win, j, 26, "          ");

    }
    else
    {
        init = (moveLog->getBlackMoves().size()>movesToShow)? (moveLog->getBlackMoves().size()-movesToShow) : 0;

            for(int i= init; i<moveLog->getBlackMoves().size(); ++i)
            {
                mvwprintw(moveLog_win, j, 26, "      ");
                mvwprintw(moveLog_win, j, 26, "%s", moveLog->getBlackMoves()[i].c_str());
                ++j;
            }
    }

    wrefresh(moveLog_win);

}

void InterfaceNcurses::printBlackCaptured(Board* board)
{
    Pieces* ptrPiece=nullptr;

    werase(blackCaptured_win);


    for(int i=0; i<board->getBlackCaptured().size(); ++i)
    {
        ptrPiece = board->getBlackCaptured()[i];

        if(settingValues.useUnicodes)
            mvwprintw(blackCaptured_win, 0, i, "%s", convertToUnicode(ptrPiece->getNotation()).c_str());
        else
            mvwprintw(blackCaptured_win, 0, i, "%s", ptrPiece->getNotation());

    }

    wrefresh(blackCaptured_win);
}

void InterfaceNcurses::printWhiteCaptured(Board* board)
{
    Pieces* ptrPiece=nullptr;

    werase(whiteCaptured_win);

    for(int i=0; i<board->getWhiteCaptured().size(); ++i)
    {
        ptrPiece = board->getWhiteCaptured()[i];

        if(settingValues.useUnicodes)
            mvwprintw(whiteCaptured_win, 0, i, "%s", convertToUnicode(ptrPiece->getNotation()).c_str());
        else
            mvwprintw(whiteCaptured_win, 0, i, "%s", ptrPiece->getNotation());
    }

    wrefresh(whiteCaptured_win);
}

void InterfaceNcurses::printInstructions()
{
    int j = 0;

    for(int i=0; i<key.size(); ++i)
    {

        wattrset(instructions_win, A_NORMAL);
        wattron(instructions_win, A_BOLD);
        mvwprintw(instructions_win, 0, j, "%s", key[i].c_str());
        wattron(instructions_win, COLOR_PAIR(3) | A_BOLD);
        mvwprintw(instructions_win, 0, j+key[i].size(), " %s ", instructions[i].c_str());

        j += key[i].size()+instructions[i].size()+5;
    }

    wrefresh(instructions_win);

}



void InterfaceNcurses::createMenuWindow()
{
    menuSelecter = 0;
    menuItems = {"New Game", "Settings", "Exit"};

    menu_win = newwin(LINES, COLS, 0, 0);

    list_win = derwin(menu_win, menuItems.size()*2, 10, 11, 45);
    menuHeader_win = derwin(menu_win, 7, 65, 1, 3);

    xBoard=6;
    yBoard=10;
    board_win = derwin(menu_win, 10, 28, yBoard, xBoard);

    error_win = derwin(menu_win, 3, 40, 18, 37);
}

void InterfaceNcurses::destroyMenuWindow()
{
    if (error_win) delwin(error_win);

    if (board_win) delwin(board_win);

    if (menuHeader_win) delwin(menuHeader_win);
    if (list_win) delwin(list_win);

    if (menu_win) delwin(menu_win);

    error_win=nullptr;

    board_win=nullptr;

    menuHeader_win=nullptr;
    list_win=nullptr;

    menu_win=nullptr;
}


void InterfaceNcurses::createNewGameWindow()
{
    playerTurn=WHITE;

    yBoard=2;
    xBoard=4;

    moveLogHeight=18;

    yWhiteCursor = yBoard+7;
    xWhiteCursor = xBoard+15;

    yBlackCursor = yBoard+2;
    xBlackCursor = xBoard+15;

    key = {"F2", "F4"};
    instructions = {"Flip board", "Resign"};


    if(boardPerspective)
    {
        yWhiteClock=10;
        yBlackClock=-1;

        yBlackCaptured=10;
        yWhiteCaptured=-1;
    }
    else
    {
        yWhiteClock=-1;
        yBlackClock=10;

        yBlackCaptured=-1;
        yWhiteCaptured=10;
    }


    newGame_win = newwin(LINES, COLS, 0, 0);

    board_win = derwin(newGame_win, 10, 28, yBoard, xBoard);
    white_clock = derwin(newGame_win, 1, 12, yBoard+yWhiteClock, xBoard+16);
    black_clock = derwin(newGame_win, 1, 12, yBoard+yBlackClock, xBoard+16);
    moveLog_win = derwin(newGame_win, moveLogHeight, 38, yBoard-1, xBoard+34);
    whiteCaptured_win = derwin(newGame_win, 1, 15, yBoard+yWhiteCaptured, xBoard);
    blackCaptured_win = derwin(newGame_win, 1, 15, yBoard+yBlackCaptured, xBoard);
    promotion_win = derwin(newGame_win, 1, 29, yBoard+12, xBoard);

    instructions_win = derwin(newGame_win, 1, 70, yBoard+19, xBoard);

    confirmation_win = derwin(newGame_win, 3, 30, yBoard+14, xBoard);
    error_win = derwin(newGame_win, 4, 30, yBoard+14, xBoard);

}

void InterfaceNcurses::destroyNewGameWindow()
{
    if(error_win) delwin(error_win);
    if(confirmation_win) delwin(confirmation_win);

    if(instructions_win) delwin(instructions_win);

    if (newGame_win) delwin(newGame_win);
    if (promotion_win) delwin(promotion_win);
    if (blackCaptured_win) delwin(blackCaptured_win);
    if (whiteCaptured_win) delwin(whiteCaptured_win);
    if (moveLog_win) delwin(moveLog_win);
    if (black_clock) delwin(black_clock);
    if (white_clock) delwin(white_clock);
    if (board_win) delwin(board_win);

    if (newGame_win) delwin(newGame_win);


    error_win=nullptr;
    confirmation_win=nullptr;

    instructions_win=nullptr;

    board_win=nullptr;
    white_clock=nullptr;
    black_clock=nullptr;
    moveLog_win=nullptr;
    whiteCaptured_win=nullptr;
    blackCaptured_win=nullptr;
    promotion_win=nullptr;
    newGame_win=nullptr;

    newGame_win=nullptr;
}


void InterfaceNcurses::createSettingsWindow()
{
    menuSelecter = 0;
    menuItems = {"Clock time", "Flip board", "Use unicodes", "Sounds", "Check color", "Allowed Moves", "Exit"};
    instructions = {"Restore default values"};
    key = {"F2"};

    settings_win = newwin(LINES, COLS, 0, 0);

    list_win = derwin(settings_win, menuItems.size()*2, 20, 4, 15);
    instructions_win = derwin(settings_win, 1, 70, 21, 5);

    confirmation_win = derwin(settings_win, 1, 45, 18, 15);
    error_win = derwin(settings_win, 2, 40, 18, 15);
}

void InterfaceNcurses::destroySettingsWindow()
{
    if(error_win) delwin(error_win);
    if(confirmation_win) delwin(confirmation_win);

    if(instructions_win) delwin(instructions_win);
    if(list_win) delwin(list_win);

    if(settings_win) delwin(settings_win);

    error_win=nullptr;
    confirmation_win=nullptr;

    instructions_win=nullptr;
    list_win=nullptr;

    settings_win=nullptr;
}


bool InterfaceNcurses::checkClickButton(Button button, int mouseY, int mouseX)
{
    return mouseX >= button.x && mouseX < (button.x + button.width) &&
           mouseY >= button.y && mouseY < (button.y + button.height);
}


char InterfaceNcurses::convertX(int x)
{

    char column=(0.3333*x+284/3)+1;

    return column;

    /*

            8x8 matrix formed by x and y limited by the board area:

         ^ y
         |
         | 3   a8  b8   c8   d8   e8   f8   g8   h8              a (97)  = 7       1 = 10
         | 4   a7  b7   c7   d7   e7   f7   g7   h7              b (98)  = 10      2 = 9
         | 5   a6  b6   c6   d6   e6   f6   g6   h6              c (99)  = 13      3 = 8
         | 6   a5  b5   c5   d5   e5   f5   g5   h5              d (100) = 16      4 = 7
         | 7   a4  b4   c4   d4   e4   f4   g4   h4              e (101) = 19      5 = 6
         | 8   a3  b3   c3   d3   e3   f3   g3   h3              f (102) = 22      6 = 5
         | 9   a2  b2   c2   d2   e2   f2   g2   h2              g (103) = 25      7 = 4
         |10   a1  b1   c1   d1   e1   f1   g1   h1              h (104) = 28      8 = 3
         |
         |     7   10   13   16   19   22   25   28
        _|_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _> x
         |

        */

}

int InterfaceNcurses::convertY(int y)
{

    int line=-y+11;

    return line;


    /*

            8x8 matrix formed by x and y limited by the board area:

         ^ y
         |
         | 3   a8  b8   c8   d8   e8   f8   g8   h8              a (97)  = 7       1 = 10
         | 4   a7  b7   c7   d7   e7   f7   g7   h7              b (98)  = 10      2 = 9
         | 5   a6  b6   c6   d6   e6   f6   g6   h6              c (99)  = 13      3 = 8
         | 6   a5  b5   c5   d5   e5   f5   g5   h5              d (100) = 16      4 = 7
         | 7   a4  b4   c4   d4   e4   f4   g4   h4              e (101) = 19      5 = 6
         | 8   a3  b3   c3   d3   e3   f3   g3   h3              f (102) = 22      6 = 5
         | 9   a2  b2   c2   d2   e2   f2   g2   h2              g (103) = 25      7 = 4
         |10   a1  b1   c1   d1   e1   f1   g1   h1              h (104) = 28      8 = 3
         |
         |     7   10   13   16   19   22   25   28
        _|_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _> x
         |

        */

}

std::string InterfaceNcurses::convertToUnicode(char pieceNotation)
{

   std::string notationConverted;

    switch (pieceNotation)
    {

    /*
        King: ♔ (\u2654), ♚ (\u265A)
        Queen: ♕ (\u2655), ♛ (\u265B)
        Rook: ♖ (\u2656), ♜ (\u265C)
        Bishop: ♗ (\u2657), ♝ (\u265D)
        Knight: ♘ (\u2658), ♞ (\u265E)
        Pawn: ♙ (\u2659), ♟ (\u265F)
    */

        case 'p':
            notationConverted = u8"\u2659";
            break;

        case 'P':
            notationConverted = u8"\u265F";
            break;

        case 'n':
            notationConverted = u8"\u2658";
            break;

        case 'N':
            notationConverted = u8"\u265E";
            break;

        case 'b':
            notationConverted = u8"\u2657";
            break;

        case 'B':
            notationConverted = u8"\u265D";
            break;


        case 'r':
            notationConverted = u8"\u2656";
            break;

        case 'R':
            notationConverted = u8"\u265C";
            break;

        case 'k':
            notationConverted = u8"\u2654";
            break;

        case 'K':
            notationConverted = u8"\u265A";
            break;

        case 'q':
            notationConverted = u8"\u2655";
            break;

        case 'Q':
            notationConverted = u8"\u265B";
            break;


        case ' ':
            notationConverted = ' ';
            break;

        case 'o':
            notationConverted = 'o';
            break;
    }

    return notationConverted;
}






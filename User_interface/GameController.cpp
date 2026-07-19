
#include "GameController.h"

GameController::GameController()

{
    //ctor
}

GameController::~GameController()
{
    //dtor
}



int GameController::getMenu()
{

    int x=43,
        y=12,
        input;

    erase();

    initscr();
    cbreak();
    keypad(stdscr, TRUE);

    start_color();

    attron(A_BOLD);

      mvprintw(1, 5,"_              ____   _");
    mvprintw(2, 4,"/ \\     _ __   / ___| | |__     ___   ___   ___");
   mvprintw(3, 3,"/ _ \\   | '__| | |     | '_ \\   / _ \\ \/ __| / __|");
  mvprintw(4, 2,"/ ___ \\  | |    | |___  | | | | |  __/ \\__ \\ \\__ \\");
 mvprintw(5, 1,"/_/   \\_\\ |_|     \\____| |_| |_|  \\___| |___/ |___/");
 mvprintw(7, 1," A simple, lightweight chess emulator.");


    mvprintw(1, 58,"__");
   mvprintw(2, 57,"(  )");
    mvprintw(3, 58,"><");
   mvprintw(4, 57,"|  |");
  mvprintw(5, 56,"/    \\");
 mvprintw(6, 55,"|______|");

    mvprintw(12, 45,"New Game");
    mvprintw(15, 45,"Settings ");

    Board* board = new Board;
    getBoard(board, 6, 10);

    do
    {
        move(y, x);

        refresh();

        input=getch();

        switch (input)
        { //Controls the cursor based on the arrow keys pressed and formatting.

            case KEY_UP:
                if(y==15)
                    y=12;
                break;

            case KEY_DOWN:
                if(y==12)
                    y=15;
                break;

            case '\n':
                if(y==12)
                {
                    erase();
                    return 1;
                }
                break;
        }

    } while (input!=27);

}




void GameController::getBoard(Board* board, int x, int y)
{

        char piece;
        int
        row=x,
        linePosition=y;

        Pieces* ptrPiece;

        initscr();
        start_color();
        setlocale(LC_ALL, "");

        //Sets the colors of the squares on the board. (Usable: 260, 500)
        init_color(COLOR_YELLOW, 260, 260, 260);
        init_color(COLOR_WHITE, 800, 800, 800);
        init_color(COLOR_RED, 500, 500, 500);
        init_pair(1, COLOR_BLACK, COLOR_YELLOW);
        init_pair(2, COLOR_BLACK, COLOR_RED);
        init_pair(3, COLOR_BLACK, COLOR_WHITE);
        init_pair(4, COLOR_CYAN, COLOR_YELLOW);
        init_pair(5, COLOR_CYAN, COLOR_RED);

        int line = 8; //Variable to display the rows beside the board.

        attron(COLOR_PAIR(3) | A_BOLD);
        mvprintw(y, x, "                            ");

        for (char k=97; k<105; k++, row=row+3)
        {

            mvprintw(linePosition+1, x, " %d", line);

           int column=y+1;

           for (int j=1, l=8; j<9, l>0; j++, l--)
            {

                if (board->getColor(j, k))
                {
                    attron(COLOR_PAIR(1) | A_BOLD);

                    if (board->getColor(j, k)&&board->getAllowedMove(l, k))
                        attron(COLOR_PAIR(4) | A_BOLD);
                }

                else
                {
                    attron(COLOR_PAIR(2) | A_BOLD);

                    if (board->getAllowedMove(l, k))
                    attron(COLOR_PAIR(5) | A_BOLD);
                }

                ptrPiece = board->getPiece(l, k);

                if (board->getAllowedMove(l, k) && ptrPiece==nullptr)
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

                //Displays the square with the piece and formatting.
                mvprintw(column, row+2, " ");
                mvprintw(column, row+3, "%c", piece);
                mvprintw(column, row+4, " ");

                column++;

                attron(COLOR_PAIR(3) | A_BOLD);
                mvprintw(column-1, row+5, "  ");
            }

            linePosition++;
            line--;
        }

        mvprintw(y+9, row-24, "   a  b  c  d  e  f  g  h   ");

        attroff(COLOR_PAIR(3) | A_BOLD);
        refresh();

}



/*void GameController::getNewGame(Board* board)
{


    erase();

    int input,
        x=7,
        y=10,
        line;

    //char //column,
         //piece;

    bool turn=true;


    do{
        getBoard(board, 4, 2);
        refresh();


        move(y,x);

        input=getch();

        switch (input){ //Controls the cursor based on the arrow keys pressed and formatting.

            case KEY_UP:
                if(y>3)
                    y--;

                //mvprintw(12, 0, "%d", convertY(y));
                break;

            case KEY_DOWN:
                if(y<10)
                    y++;

                //mvprintw(12, 0, "%d", convertY(y));
                break;

            case KEY_LEFT:
                if (x>7)
                    x=x-3;

                //mvprintw(12, 4, "%c", convertX(x));
                break;

            case KEY_RIGHT:
                if (x<28)
                    x=x+3;

                //mvprintw(12, 4, "%c", convertX(x));
                break;

            case '\n':
                Pieces* ptrPiece = board->getPiece(convertY(y), convertX(x));

                if(ptrPiece!=nullptr&&ptrPiece->getColor()==turn)
                    getMove(board, ptrPiece, x, y, &turn);

                break;

        }

    }while(input!=27);



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



}



void GameController::getMove(Board* board, Pieces* ptrPiece, int x, int y, bool* turn)
{
    erase();

    ptrPiece->setMove(board);

    if(board->checkAllowedMoves())
    {

        int xPosition=x,
            yPosition=y,
            input;

        //Pieces* verifyPromotion;

        do{
            getBoard(board, 4, 2);

            move(yPosition, xPosition);

            input=getch();

            switch (input)
            { //Controls the cursor based on the arrow keys pressed and formatting.

                case KEY_UP:
                    if(yPosition>3)
                        yPosition--;

                    mvprintw(17, 0, "%d", convertY(yPosition));
                    break;

                case KEY_DOWN:
                    if(yPosition<10)
                        yPosition++;

                    mvprintw(17, 0, "%d", convertY(yPosition));
                    break;

                case KEY_LEFT:
                    if (xPosition>7)
                        xPosition-=3;

                   //vprintw(12, 4, "%c", convertX(xPosition));
                    break;

                case KEY_RIGHT:
                    if (xPosition<28)
                        xPosition+=3;

                    //mvprintw(12, 4, "%c", convertX(xPosition));
                    break;

                case '\n':
                    if(ptrPiece!=NULL)
                    {
                        int row=ptrPiece->getRow();

                        board->setPiece(convertY(yPosition), convertX(xPosition), ptrPiece);

                        if(ptrPiece->getNotation()=='P'||ptrPiece->getNotation()=='p')
                            if (Pawn* ptrPawn = dynamic_cast<Pawn*>(ptrPiece))
                            {
                                /*if (ptrPawn->getRow()==8||ptrPawn->getRow()==1)
                                    ptrPawn->setPromotion(getPromotion(4, 13), board);

                                if(row+2!=convertY(yPosition)&&row-2!=convertY(yPosition))
                                {
                                    board->setLeftPassant(NULL_CHAR);
                                    board->setRightPassant(NULL_CHAR);
                                }

                            }
                    }

                    *turn = !*turn;
                    input=27;
                    break;
            }



        }while(input!=27);

        board->removeAllowedMoves();


    }

}



char GameController::getPromotion(int x, int y)
{

    attron(COLOR_PAIR(3) | A_BOLD);

    int
    cursorXPosition=x+15,
    input;

    char promovedTo;

    do{

        mvprintw(y, x, " Select piece: Q R B N ");

        move(y,cursorXPosition);

        input=getch();

        switch (input)
        { //Controls the cursor based on the arrow keys pressed and formatting.

            case KEY_RIGHT:
                    if(cursorXPosition<25)
                        cursorXPosition+=2;

                    //mvprintw(15, 4, "%d", cursorXPosition);
                    break;

            case KEY_LEFT:
                    if(cursorXPosition>19)
                        cursorXPosition-=2;

                //mvprintw(15, 4, "%d", cursorXPosition);
                break;


            case '\n':
                promovedTo = static_cast<char>(mvinch(y, cursorXPosition) & A_CHARTEXT);
                erase();
                refresh();
                input=27;
                return promovedTo;
        }


    }while(input!=27);
}




char GameController::convertX(int x){

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




}

int GameController::convertY(int y){

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



}*/

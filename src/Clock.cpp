#include "Clock.h"

Clock::Clock()
{
    //ctor

}

Clock::~Clock()
{
    //dtor
}


void Clock::getTimer() const
{
    system("clear");

    std::cout << "White: "
         << std::setw(2) << std::setfill('0') << whiteHours.count() << ":"
         << std::setw(2) << std::setfill('0') << whiteMinutes.count() << ":"
         << std::setw(2) << std::setfill('0') << whiteSeconds.count() << ":"
         << std::setw(3) << std::setfill('0') << whiteMilliseconds.count() << std::endl;

    std::cout << std::endl;

    std::cout << "Black: "
         << std::setw(2) << std::setfill('0') << blackHours.count() << ":"
         << std::setw(2) << std::setfill('0') << blackMinutes.count() << ":"
         << std::setw(2) << std::setfill('0') << blackSeconds.count() << ":"
         << std::setw(3) << std::setfill('0') << blackMilliseconds.count() << std::endl;
}


void Clock::timer(int whiteTime, int blackTime, GameOverFlags& gameOverFlags, std::atomic<bool>& interruption)
{

    std::chrono::seconds whiteDuration(whiteTime);
    std::chrono::seconds blackDuration(blackTime);

    /*Defines four points in time.*/
    std::chrono::time_point<std::chrono::steady_clock> whiteStart,
                                             blackStart,
                                             whiteEnd,
                                             blackEnd;


    std::chrono::milliseconds whiteElapsed_ms(0),
                         blackElapsed_ms(0);


    /*Calculates the remaining time for each player by
    subtracting the elapsed time (represented by the variables whiteElapsed_ms and blackElapsed_ms)
    from the total available time for each player (represented by the variables whiteDuration and blackDuration).*/
    std::chrono::milliseconds whiteRemaining = whiteDuration-whiteElapsed_ms,
                         blackRemaining = blackDuration-blackElapsed_ms;


    /*Calculates the elapsed time since the start of the timer for each player.*/
    std::chrono::duration<double> whiteElapsed = std::chrono::steady_clock::now() - whiteStart,
                             blackElapsed = std::chrono::steady_clock::now() - blackStart;


    /*Converts of the remaining time duration for the black player into hours, minutes, seconds, and milliseconds
    to be displayed during the game.*/
    blackHours = std::chrono::duration_cast<std::chrono::hours>(blackRemaining);
    blackMinutes = std::chrono::duration_cast<std::chrono::minutes>(blackRemaining - blackHours);
    blackSeconds = std::chrono::duration_cast<std::chrono::seconds>(blackRemaining - blackHours - blackMinutes);
    blackMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(blackRemaining - blackHours - blackMinutes - blackSeconds);



    if(whiteDuration.count()==0)
        running=false; /*If the clock is set to Off in settings, the timer will not start.*/

    while (running)
    {
        if (playerTurn)
        {
            /*Initializes the white player's timer,
            setting the moment when the timer starts (whiteStart) and the moment when it should stop (whiteEnd).*/
            whiteStart = std::chrono::steady_clock::now() - whiteElapsed_ms;
            whiteEnd = whiteStart + whiteDuration;

            while (std::chrono::steady_clock::now() < whiteEnd && playerTurn)
            {
                /*Measures the elapsed time for the white player.*/
                whiteElapsed = std::chrono::steady_clock::now() - whiteStart;

                /*Converts to milliseconds.*/
                whiteElapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(whiteElapsed);

                /*Calculates the remaining time for the white player.*/
                whiteRemaining = whiteDuration - whiteElapsed_ms;

                /*Converts of the remaining time duration for the white player into hours, minutes, seconds, and milliseconds
                to be displayed during the game.*/
                whiteHours = std::chrono::duration_cast<std::chrono::hours>(whiteRemaining);
                whiteMinutes = std::chrono::duration_cast<std::chrono::minutes>(whiteRemaining - whiteHours);
                whiteSeconds = std::chrono::duration_cast<std::chrono::seconds>(whiteRemaining - whiteHours - whiteMinutes);
                whiteMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(whiteRemaining - whiteHours - whiteMinutes - whiteSeconds);

                /*getTimer();*/

                if(whiteRemaining < std::chrono::milliseconds(20000))
                    gameOverFlags.whiteTimeWarning=true;

                /*Checks if time is up.*/
                if(whiteRemaining <= std::chrono::milliseconds(1))
                {
                    running=false;
                    gameOverFlags.timesUp=true;
                    whiteMilliseconds = std::chrono::milliseconds(0);
                    break;
                }

                /*Checks if the game is over.*/
                if(gameOverFlags.checkMate || gameOverFlags.draw || gameOverFlags.resign || gameOverFlags.aborted || interruption)
                {
                    running=false;
                    break;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }


        }
        else /*The same logic above is implemented for the black player if the variable turn is false.*/
        {
            blackStart = std::chrono::steady_clock::now() - blackElapsed_ms;
            blackEnd = blackStart + blackDuration;

            while (std::chrono::steady_clock::now() < blackEnd && !playerTurn)
            {
                blackElapsed = std::chrono::steady_clock::now() - blackStart;

                blackElapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(blackElapsed);

                blackRemaining = blackDuration - blackElapsed_ms;

                blackHours = std::chrono::duration_cast<std::chrono::hours>(blackRemaining);
                blackMinutes = std::chrono::duration_cast<std::chrono::minutes>(blackRemaining - blackHours);
                blackSeconds = std::chrono::duration_cast<std::chrono::seconds>(blackRemaining - blackHours - blackMinutes);
                blackMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(blackRemaining - blackHours - blackMinutes - blackSeconds);


                if(blackRemaining < std::chrono::milliseconds(20000))
                    gameOverFlags.blackTimeWarning=true;

                if(blackRemaining <= std::chrono::milliseconds(1))
                {
                    running=false;
                    gameOverFlags.timesUp=true;
                    blackMilliseconds = std::chrono::milliseconds(0);
                    break;
                }

                if(gameOverFlags.checkMate || gameOverFlags.draw || gameOverFlags.resign || gameOverFlags.aborted || interruption)
                {
                    running=false;
                    break;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }
    }
}



void Clock::setTurn(bool playerColor)
{
    playerTurn = playerColor;
}



std::chrono::hours Clock::getWhiteHours() const
{
    return whiteHours;
}

std::chrono::minutes Clock::getWhiteMinutes() const
{
    return whiteMinutes;
}

std::chrono::seconds Clock::getWhiteSeconds() const
{
    return whiteSeconds;
}

std::chrono::milliseconds Clock::getWhiteMilliseconds() const
{
    return whiteMilliseconds;
}


std::chrono::hours Clock::getBlackHours() const
{
    return blackHours;
}

std::chrono::minutes Clock::getBlackMinutes() const
{
    return blackMinutes;
}

std::chrono::seconds Clock::getBlackSeconds() const
{
    return blackSeconds;
}

std::chrono::milliseconds Clock::getBlackMilliseconds() const
{
    return blackMilliseconds;
}



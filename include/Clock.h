/*
*  This class contains methods and variables to simulate the operation of a chess clock
*  using the Chrono library.
*  The clock is updated in another thread.
*  Hence, this class also utilizes the Thread and Atomic libraries for concurrency control.
*/

#ifndef CLOCK_H
#define CLOCK_H

#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <atomic>

#include "Constants.h"

#include "Coordinates.h"

#include "SoundManager.h"


class Clock
{
    public:
        Clock();
        ~Clock();

        /*Manages time according to each player's turn. Receives variables to stop the time counting when the game ends.*/
        void timer(int whiteTime, int blacktime, GameOverFlags& gameOverflag, std::atomic<bool>& interruption);

        void setTurn(const bool playerColor);

        void getTimer() const;

        std::chrono::hours getWhiteHours() const;
        std::chrono::minutes getWhiteMinutes() const;
        std::chrono::seconds getWhiteSeconds() const;
        std::chrono::milliseconds getWhiteMilliseconds() const;

        std::chrono::hours getBlackHours() const;
        std::chrono::minutes getBlackMinutes() const;
        std::chrono::seconds getBlackSeconds() const;
        std::chrono::milliseconds getBlackMilliseconds() const;



    protected:
        bool running{true};/*"Controls the start or stop of the clock."*/

        std::atomic<bool> playerTurn{WHITE}; /*Manages the time according to each player's turn.*/

        std::chrono::hours whiteHours,
                      blackHours;

        std::chrono::minutes whiteMinutes,
                        blackMinutes;

        std::chrono::seconds whiteSeconds,
                        blackSeconds;

        std::chrono::milliseconds whiteMilliseconds,
                             blackMilliseconds;


    private:
};

#endif // CLOCK_H

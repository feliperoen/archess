/*
 * This class has methods to be implemented by other classes that will be used as an interface,
 * utilizing polymorphism,
 * to display screens and handle user inputs.
*/


#ifndef INTERFACE_H
#define INTERFACE_H

#include "Board.h"
#include "Clock.h"
#include "MoveLog.h"
#include "Settings.h"

#include "Coordinates.h"


class Interface
{
    public:
        Interface();

        virtual ~Interface();

        virtual int getMenuSelection(const std::atomic<bool>& interruption) = 0; /*Returns an integer corresponding to the user's selection in the menu.*/
        /* 1 = new game
           2 = settings
           3 = exit and close application
        */

        /*This method updates the game screen and returns a struct with the coordinates of the chosen square for the move.*/
        virtual Board::Coordinates getSelectedSquare(Board* board, MoveLog* moveLog, Clock* clock,
            GameOverFlags& gameOverFlags, std::atomic<bool>& interruption) = 0;

        /*Gets the user's promotion selection.*/
        virtual char getPromotion(Clock*, const std::atomic<bool>& timesUp, const std::atomic<bool>& interruption) = 0;

        /*This method can be called whenever it's necessary to confirm or request permission from the user.
        It receives a string with the question to be confirmed
        and returns a bool that is true for 'yes' and false for 'no.'*/
        virtual bool getConfirmation(const std::string confirmQuery, const std::atomic<bool>& interruption) = 0;

        /*Displays the error message received as a parameter.*/
        virtual void printError(const std::string errorMessage) = 0;

        /*Returns a SettingValues struct with the configuration values saved by the user.*/
        virtual Settings::SettingValues getSettings(const std::atomic<bool>& interruption) = 0;

        void setTurn(const bool playerTurn);
        void setConfigurations(const bool autoFlipBoard, const bool useUnicodes, const bool sound, const bool checkColor, const bool allowedMoves, const int clockTime);

    protected:
        bool playerTurn; /*Indicates the current player's turn by color.*/


        /*Variables to validate configurations.*/
        Settings::SettingValues settingValues;

    private:
};

#endif // INTERFACE_H

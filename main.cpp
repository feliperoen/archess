#include "Interface.h"
#include "InterfaceNcurses.h"
#include "GameController.h"

int main()
{
    Interface* interface = new InterfaceNcurses();

    GameController* gameController = new GameController(interface);

    gameController->mainMenu(interface);

    delete gameController;

    
    return 0;
}

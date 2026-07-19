
#include "Interface.h"

Interface::Interface()
{
    //ctor
}

Interface::~Interface()
{
    //dtor
}

void Interface::setTurn(bool colorTurn)
{
    playerTurn=colorTurn;
}

void Interface::setConfigurations(bool autoFlipBoard, bool useUnicodes, bool sound, bool checkColor, bool allowedMoves, int clockTime)
{
    settingValues.clockTime=clockTime;
    settingValues.autoFlipBoard=autoFlipBoard;
    settingValues.useUnicodes=useUnicodes;
    settingValues.sound=sound;
    settingValues.checkColor=checkColor;
    settingValues.allowedMoves=allowedMoves;
}


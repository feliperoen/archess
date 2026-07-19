#include "Settings.h"

Settings::Settings(const std::string& fileName) : fileName_(fileName)
{
    //ctor

    try
    {
        setConfigs(); /*ATTEMPTS TO LOAD THE SETTINGS FROM THE FILE.*/
    }
    catch (const std::exception& e)
    {
        setDefaultValues();
        setConfigFile(); /*IF THE FILE DOES NOT EXIST, IT CREATES IT WITH DEFAULT SETTING VALUES.*/
    }
}

Settings::~Settings()
{
    //dtor
}



void Settings::setDefaultValues()
{
    settingValues.clockTime = 3;
    settingValues.autoFlipBoard = false;
    settingValues.useUnicodes = true;
    settingValues.sound = true;
    settingValues.checkColor = true;
    settingValues.allowedMoves = true;
}



void Settings::setConfigFile()
{

    try
    {/*ATTEMPTS TO CREATES THE CONFIGURATION FILE WITH THE DEFAULTS VALUES.*/
        std::ofstream file(fileName_);

        /*CONFIGURES THE OBJECT OFSTREAM TO THROW EXCEPTIONS.*/
        file.exceptions(std::ofstream::failbit | std::ofstream::badbit);


        /*WRITES THE CURRENT SETTINGS TO THE FILE.*/
        file << "clockTime=" << settingValues.clockTime << "\n";
        file << "flipBoard=" << (settingValues.autoFlipBoard ? "true" : "false") << "\n";
        file << "useUnicodes=" << (settingValues.useUnicodes ? "true" : "false") << "\n";
        file << "sound=" << (settingValues.sound ? "true" : "false") << "\n";
        file << "checkColor=" << (settingValues.checkColor ? "true" : "false") << "\n";
        file << "allowedMoves=" << (settingValues.allowedMoves ? "true" : "false") << "\n";

        file.close();

    /*CHECKS THE INTEGRITY OF THE FILE.*/
    if (!file.good())
        throw std::runtime_error("Failed to write to config file.");

    }
    catch(const std::ofstream::failure& e) /*PROPAGATES ANY EXCEPTIONS THAT ARE THROWN.*/
    {
        throw;
    }
    catch(std::exception& e)
    {
        throw;
    }
}




void Settings::setConfigs()
{
    /*CHECKS IF THE CONFIGURATION FILE EXISTS AND THROW AN EXCEPTION IF DOESN'T.*/
    if (!std::ifstream(fileName_))
        throw std::ios_base::failure("The config file does't exist.");

    /*TRIES TO READ THE CONFIGURATION FILE..*/
    std::ifstream file(fileName_);

    /*CONFIGURES THE OBJECT ODSTREAM TO THROW EXCEPTIONS.*/
    file.exceptions(std::ofstream::badbit);

    std::string line;

    /*READS A LINE FROM THE OBJECT FILE AND STORES THE CONTENT IN THE VARIABLE LINE.
      THE WHILE LOOP CONTINUES AS LONG AS THERE ARE LINES TO BE READ FROM THE FILE.*/
    while (getline(file, line))
    {
        /*DECLARES A ISTRINGSTREAM CALLED LINE_STREAM
        INITIALIZED WITH THE CONTENT OF THE READ LINE.
        LINE_STREAM ALLOWS TREATING THE LINE AS AN INPUT STREAM,
        MAKING IT EASIER TO SEPARATE PARTS OF THE LINE.*/
        std::istringstream line_stream(line);
        std::string key;

        /*READS UP TO THE = CHARACTER FROM LINE_STREAM
        AND STORES THE TEXT BEFORE THE = IN THE VARIABLE KEY.
        IF THE READ IS SUCCESSFUL (THERE IS A = IN THE LINE), THE GETLINE WILL RETURN TRUE.*/
        if (getline(line_stream, key, '='))
        {
            std::string value;
            if (getline(line_stream, value))
            {

                /*COMPARING THE KEY AND ASSIGNING THE VALUE.*/
                if (key == "clockTime")
                {
                    /*IF THE VALUE FOR `CLOCKTIME` IN THE FILE IS BETWEEN 0 AND 6, `
                    CLOCKTIME_` WILL BE UPDATED.*/
                    int tempClockTime = std::stoi(value);
                    if (tempClockTime >= 0 && tempClockTime <= 6)
                        settingValues.clockTime = tempClockTime;
                }
                else if (key == "flipBoard") settingValues.autoFlipBoard = (value == "true");
                else if (key == "useUnicodes") settingValues.useUnicodes = (value == "true");
                else if (key == "sound") settingValues.sound = (value == "true");
                else if (key == "checkColor") settingValues.checkColor = (value == "true");
                else if (key == "allowedMoves") settingValues.allowedMoves = (value == "true");
            }
        }
    }

    file.close();
}





void Settings::setClockTimeConfig(const int playersTime)
{
    settingValues.clockTime=playersTime;
}

void Settings::setAutoFlipBoardConfig(const bool isEnable)
{
    settingValues.autoFlipBoard=isEnable;
}

void Settings::setUseUnicodeConfig(const bool isEnable)
{
    settingValues.useUnicodes=isEnable;
}

void Settings::setSoundConfig(const bool isEnable)
{
    settingValues.sound=isEnable;
}

void Settings::setCheckColorConfig(const bool isEnable)
{
    settingValues.checkColor=isEnable;
}

void Settings::setAllowedMovesConfig(const bool isEnable)
{
    settingValues.allowedMoves=isEnable;
}



int Settings::getClockTimeConfig() const
{
    return settingValues.clockTime;
}

bool Settings::getFlipBoardConfig() const
{
    return settingValues.autoFlipBoard;
}

bool Settings::getUseUnicodeConfig() const
{
    return settingValues.useUnicodes;
}

bool Settings::getSoundConfig() const
{
    return settingValues.sound;
}

bool Settings::getCheckColorConfig() const
{
    return settingValues.checkColor;
}

bool Settings::getAllowedMovesConfig() const
{
    return settingValues.allowedMoves;
}




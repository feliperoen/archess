/*
*  This class implements methods and variables to save the user-selected settings.
*  It creates a .ini file from which the settings will be read and saved.
*/

#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <fstream>
#include <sstream>

class Settings
{
    public:
        Settings(const std::string& filName);

        ~Settings();

        void setDefaultValues();
        void setConfigFile(); /*Save settings to the config file. If the file does not exist, creates it with default values.*/
        void setConfigs(); /*Loads the settings from the file.*/

        void setClockTimeConfig(const int playersTime);
        void setAutoFlipBoardConfig(const bool isEnable);
        void setUseUnicodeConfig(const bool isEnable);
        void setSoundConfig(const bool isEnable);
        void setCheckColorConfig(const bool isEnable);
        void setAllowedMovesConfig(const bool isEnable);

        int getClockTimeConfig() const;
        bool getFlipBoardConfig() const;
        bool getUseUnicodeConfig() const;
        bool getSoundConfig() const;
        bool getCheckColorConfig() const;
        bool getAllowedMovesConfig() const;

        /*Variables that will be used by other classes to define the settings.*/
        struct SettingValues
        {
            int clockTime=3;

            bool autoFlipBoard=false,
                 useUnicodes=true,
                 sound=true,
                 checkColor=true,
                 allowedMoves=true;
        };
    protected:

    private:
        std::string fileName_;

        SettingValues settingValues;
};

#endif // SETTINGS_H

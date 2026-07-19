#include "SoundManager.h"

#include "chrono"
#include "thread"



SoundManager::SoundManager() : m_sampleRate(44100), m_amplitude(16), m_phase(0)
{
    //ctor

    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        throw std::runtime_error("Error initializing audio. " + std::string(SDL_GetError()));

    SDL_AudioSpec audioSpec = {};
    audioSpec.freq = m_sampleRate;
    audioSpec.format = AUDIO_U8;
    audioSpec.channels = 1;
    audioSpec.samples = 512;
    //audioSpec.callback
    audioSpec.userdata = this;

    if (SDL_OpenAudio(&audioSpec, nullptr) < 0)
    {
        SDL_Quit();
        throw std::runtime_error("Error opening audio device. " + std::string(SDL_GetError()));
    }
}

SoundManager::~SoundManager()
{
    //dtor

    SDL_CloseAudio();
    SDL_Quit();
}


/*
    C	523.25
    C#	554.37
    D	587.33
    D#	622.25
    E	659.25
    F	698.46
    F#	739.99
    G	783.99
    G#  830.61
    A	880.00
    A#	932.33
    B	987.77
*/

void SoundManager::playMoveSound()
{
    playBeep(440, 100);
}

void SoundManager::playCaptureSound()
{
    playBeep(440, 100);

    SDL_Delay(50);

    playBeep(660, 100);
}

void SoundManager::playCheckSound()
{
    playBeep(587, 100);

    SDL_Delay(150);

    playBeep(587, 100);

}

void SoundManager::playVictoryTune()
{

    SDL_Delay(150);
    playBeep(660, 100);

    SDL_Delay(150);
    playBeep(880, 150);

}

void SoundManager::playTimeWarningSound()
{
    playBeep(880, 100);

    SDL_Delay(150);

    playBeep(880, 100);

    SDL_Delay(150);

    playBeep(880, 100);

    SDL_Delay(100);
}

void SoundManager::gameSoundController(std::atomic<bool>& isMove, std::atomic<bool>& isCapture, std::atomic<bool>& isCheck,
    std::atomic<bool>& mute, GameOverFlags& gameOverflag, std::atomic<bool>& interruption)
{
    bool whiteTimeAlert=false,
         blackTimeAlert=false;

    while(!interruption && !gameOverflag.draw && !gameOverflag.aborted)
    {
        if(isCheck)
        {
            if(!mute)
                playCheckSound();

            isCheck=false;
        }

        if(isCapture)
        {
            if(!mute)
                playCaptureSound();

            isCapture=false;
        }

        if(isMove)
        {
            if(!mute)
                playMoveSound();

            isMove=false;
        }

        if(gameOverflag.checkMate || gameOverflag.resign || gameOverflag.timesUp)
        {
            if(!mute)
            {
                playCheckSound();
                playVictoryTune();
            }

            break;
        }

        if(gameOverflag.whiteTimeWarning && !whiteTimeAlert)
        {
            if(!mute)
                playTimeWarningSound();

            whiteTimeAlert=true;
        }

        if(gameOverflag.blackTimeWarning && !blackTimeAlert)
        {
            if(!mute)
                playTimeWarningSound();

            blackTimeAlert=true;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void SoundManager::playBeep(int frequencyHz, int durationMs)
{
    int len = m_sampleRate * durationMs / 1000;
    Uint8* stream = new Uint8[len];

    int period = m_sampleRate / frequencyHz;

    for (int i = 0; i < len; ++i)
    {
        stream[i] = (i % period < period / 2) ? 128 + m_amplitude : 128 - m_amplitude;
    }

    SDL_ClearQueuedAudio(1);
    SDL_QueueAudio(1, stream, len);
    SDL_PauseAudio(0);

    //SDL_Delay(durationMs+50);

    delete[] stream;
}

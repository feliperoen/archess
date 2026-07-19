#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <iostream>
#include <SDL2/SDL.h>
#include <thread>
#include <atomic>

#include "Coordinates.h"

class SoundManager
{
    public:
        SoundManager();
        ~SoundManager();

        void playMoveSound();
        void playCaptureSound();
        void playCheckSound();
        void playVictoryTune();
        void playTimeWarningSound();

        void gameSoundController(std::atomic<bool>& isMove, std::atomic<bool>& isCapture, std::atomic<bool>& isCheck, std::atomic<bool>& mute, GameOverFlags& gameOverflag, std::atomic<bool>& interruption);

    protected:

    private:
        int m_sampleRate,
            m_amplitude,
            m_phase;

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

        void playBeep(int frequencyHz, int durationMs);
};

#endif // SOUNDMANAGER_H

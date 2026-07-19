#ifndef GAMEOVERFLAGS_H
#define GAMEOVERFLAGS_H

struct GameOverFlags
{
    std::atomic<bool> checkMate,
                      resign,
                      timesUp,
                      draw,
                      aborted,
                      whiteTimeWarning,
                      blackTimeWarning;

    GameOverFlags() : checkMate(false), resign(false), timesUp(false), draw(false), aborted(false),
    whiteTimeWarning(false), blackTimeWarning(false) {}
};

#endif // GAMEOVERFLAGS_H

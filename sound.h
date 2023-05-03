#pragma once

#include <SDL_mixer.h>
#include <string>
#include "lib.h"

using namespace std;

class sound:LTexture
{
public:
    bool init();

    void Free();

    void playBreath();

    void playHit();

    void playclick();

    void playmusic();

    void renderSound();

    void renderSound2();

    void musicstatus(bool x);

    bool checkSound();
    bool checkSound2();

private:
    const short int POS_X = 100;
    const short int POS_Y = 260;
    const short int POS_Y2 = 315;
    bool isPlay = 0; bool isPlaymusic = 0;
    Mix_Chunk* breath = NULL;
    Mix_Chunk* hit = NULL;
    Mix_Chunk* drop = NULL;
    Mix_Chunk* click = NULL;

    Mix_Music* music = NULL;
    SDL_Rect Mute ;
    SDL_Rect Active ;
};

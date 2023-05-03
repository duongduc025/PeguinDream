#pragma once

#include "peguin.h"
#include "pipe.h"
#include "land.h"
#include "sound.h"
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class game:LTexture
{
public:
    struct input
    {
        enum type { QUIT, PLAY, NONE, PAUSE, SETTING, TUTOR};
        type Type;
    };
    input userInput;

    bool clicked = false;
    int x_mouse = 0;
    int y_mouse = 0;

    peguin bird;
    pipe pipe;
    sound sound;
    land land;
public:
    game();

    ~game();

    bool initGraphic();

    bool isQuit()
    {
        return quit;
    }

    bool isDie()
    {
        return die;
    }

    int getPipeWidth()
    {
        return pipe.width();
    }

    int getPipeHeight()
    {
        return pipe.height();
    }

    void takeInput();

    void display();

    void releaseGraphic();

    void renderScoreSmall();

    void renderScoreLarge();

    void rendermenu();

    void renderBestScore();

    void renderMessage();

    void renderBackground();

    void renderBackgroundNight();

    void sunbackgr();

    void moonbackgr();

    bool changebackgr();

    void renderLand();

    bool tutorbutton();

    void rendertutortab();

    void resume();

    void pause();

    void renderPauseTab();

    void renderSettingTab();

    void lightskin();

    void darkskin();

    void peace();

    void devil();

    bool changelevel();

    void nextButton();

    bool okbutton();

    bool outbutton();

    bool buttonplay();

    bool menubutton();

    bool buttonsetting();

    bool buttonquit();

    bool changeskin();

    void renderGameOver();

    void rendertrophy();

    void replay();

    bool checkReplay();

    void Restart();

    short int bestScore;
};

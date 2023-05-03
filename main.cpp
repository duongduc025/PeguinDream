#include "game.h"
#include "peguin.h"
#include "lib.h"
#include <time.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
const int FPS = 60;
const int frameDelay = 1000 / FPS;

using namespace std;

int main(int argc, char** argv)
{
    Uint32 frameStart;
    short int frameTime;
    game g;
    bool ready = 0;
    bool isPause = 0;
    bool isSetting = 0;
    bool isSound = 1;
    bool skin = 0;
    short int frame = 0;
    bool isDark = 0;
    int idx = 0;
    bool menu = 1;
    bool quit = 0;
    bool rep = 1;
    bool music = 1;
    bool istutor = 0;
    bool hard = 0;

    while(!g.isQuit() && !quit)
    {
        frameStart = SDL_GetTicks();
        if (g.isDie())
        {
            if (ready)
            {
                g.sound.playHit();
                g.bird.render(0);

            }
            g.userInput.Type = game::input::NONE;
            while(g.isDie() && !g.isQuit() && !quit)
            {
                g.takeInput();
                if (ready == 1 && g.userInput.Type == game::input::PLAY)
                {
                    if (g.checkReplay())
                    {
                        g.sound.playclick();
                        ready = 0;
                    }
                    g.userInput.Type = game::input::NONE;
                }
                if(!isDark) g.renderBackground();
                else g.renderBackgroundNight();
                g.pipe.render();
                g.land.render();
                if (ready)
                {
                    g.bird.render(0);
                    g.bird.fall();
                    g.renderGameOver();
                    if(g.menubutton())
                    {
                        menu = 1;
                        ready = 0;
                        g.sound.playclick();
                    }
                    g.rendertrophy();
                    g.renderScoreSmall();
                    g.renderBestScore();
                    g.replay();
                }
                else
                {
                    if (menu)
                    {
                        g.rendermenu();
                        if(rep)
                        {
                            g.sound.playmusic();
                            rep = 0;
                        }
                        if(g.buttonplay()) {
                                if(!isSetting) {
                                    menu = 0;
                            g.sound.playclick();
                        }}
                        if(g.buttonquit()) quit = 1;
                        if(g.tutorbutton())
                        {
                            g.sound.playclick();
                            istutor = 1;
                        }
                        if(g.buttonsetting())
                        {
                            if(!isSetting)g.sound.playclick();
                            isSetting = 1;
                        }
                        if(istutor)
                        {
                            g.rendertutortab();
                            if(g.outbutton())
                            {
                                g.sound.playclick();
                                istutor = 0;

                            }
                        }
                        if(isSetting)
                        {
                            g.renderSettingTab();
                            if(g.outbutton())
                            {
                                g.sound.playclick();
                                isSetting = 0;

                            }
                                g.sound.musicstatus(music);
                                g.sound.renderSound();
                                g.sound.renderSound2();
                                g.nextButton();
                                if (!skin) g.lightskin();
                                else g.darkskin();
                                if(!hard) g.peace();
                                else g.devil();

                                if(!isDark) g.sunbackgr();
                                else g.moonbackgr();

                                if (g.userInput.Type == game::input::PLAY)
                                {
                                    if (g.sound.checkSound())
                                    {
                                        isSound = abs(1 - isSound);
                                    }
                                    if(g.sound.checkSound2())
                                    {
                                        music = abs(1 - music);
                                    }
                                    if (g.changeskin())
                                    {
                                        skin = abs(1 - skin);

                                    }
                                    if(g.changelevel())
                                    {
                                        hard = abs(1-hard);
                                    }
                                    if(g.changebackgr())
                                    {
                                        isDark = abs(1 - isDark);
                                    }
                                    g.userInput.Type = game::input::NONE;
                                }
                            }


                        g.userInput.Type = game::input::NONE;
                    }
                    else
                    {
                        if(g.userInput.Type == game::input::SETTING)
                        {
                            isSetting = abs(1-isSetting);
                            g.userInput.Type = game::input::NONE;
                        }
                        g.pipe.init();
                        g.sound.musicstatus(music);
                        if(hard) g.bird.init(skin,1);
                        else g.bird.init(skin, 0.8);
                        g.bird.render(0);
                        g.renderMessage();
                        if(isSetting)
                        {
                            g.renderSettingTab();
                            g.sound.renderSound();
                            g.sound.renderSound2();
                            g.nextButton();
                            if (!skin) g.lightskin();
                            else g.darkskin();

                            if(!isDark) g.sunbackgr();
                            else g.moonbackgr();

                            if (g.userInput.Type == game::input::PLAY)
                            {
                                if (g.sound.checkSound())
                                {
                                    isSound = abs(1 - isSound);
                                }
                                else if(g.sound.checkSound2())
                                {
                                    music = abs(1 - music);
                                }
                                else if (g.changeskin())
                                {
                                    skin = abs(1 - skin);

                                }
                                else if(g.changebackgr())
                                {
                                    isDark = abs(1 - isDark);
                                }
                                g.userInput.Type = game::input::NONE;
                            }
                        }
                        if (g.userInput.Type == game::input::PLAY)
                        {
                            g.Restart();
                            ready = 1;
                            g.userInput.Type = game::input::NONE;
                        }
                        g.land.update();
                    }
                }
                g.display();
            }
            g.pipe.init();
        }
        else
        {

            g.takeInput();
            if (g.userInput.Type == game::input::PAUSE)
            {
                isPause = abs(1 - isPause);
                g.userInput.Type = game::input::NONE;
            }

            if (isPause == 0 && g.userInput.Type == game::input::PLAY)
            {
                if (isSound) g.sound.playBreath();
                g.bird.resetTime();
                g.userInput.Type = game::input::NONE;
            }
            if(!isDark) g.renderBackground();
            else g.renderBackgroundNight();
            g.pipe.render();
            g.land.render();
            if(isPause == 0)
            {
                frame++;
                if (frame == 20)
                {
                    idx++;
                    if (idx == 3)
                        idx = 0;
                    frame = 0;
                }
            }
            g.bird.render(idx);
            g.renderScoreLarge();

            if (!isPause)
            {
                if(hard) g.bird.update(g.getPipeWidth(), g.getPipeHeight(),1);
                else g.bird.update(g.getPipeWidth(), g.getPipeHeight(), 0.8);
                g.pipe.update();
                g.land.update();
                g.pause();
            }
            else
            {
                g.resume();
                g.renderPauseTab();
                g.sound.renderSound();
                g.sound.renderSound2();
                g.sound.musicstatus(music);
                g.renderScoreSmall();
                g.renderBestScore();
                g.replay();
                g.sound.renderSound();
                if (g.userInput.Type == game::input::PLAY)
                {
                    if (g.checkReplay())
                    {
                        isPause = 0;
                    }
                    if (g.sound.checkSound())
                    {
                        isSound = abs(1 - isSound);
                    }
                    if(g.sound.checkSound2())
                    {
                        music = abs(1 - music);
                    }
                    g.userInput.Type = game::input::NONE;
                }
            }
            g.display();
        }

        //Limit FPS
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    return 0;
}

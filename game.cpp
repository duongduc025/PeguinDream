#include "game.h"
#include "lib.h"
#include <iostream>

void game::takeInput()
{
    while(SDL_PollEvent(&event) != 0)
    {
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			clicked = true;
			x_mouse = event.motion.x; y_mouse = event.motion.y;
		} else {
			clicked = false;
		}
        if (event.type == SDL_QUIT)
        {
            userInput.Type = input::QUIT;
        }
        else if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q && event.key.repeat == 0)
        {
            userInput.Type = input::SETTING;
        }
        else if ( event.type == SDL_MOUSEBUTTONDOWN || (event.type == SDL_KEYDOWN &&
		(event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_UP) && event.key.repeat == 0) )
        {
            userInput.Type = input::PLAY;
        }
		else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p && event.key.repeat == 0)
		{
			userInput.Type = input::PAUSE;
		}
    }
}

game::game()
{
    initGraphic();
    pipe.init();
    land.init();
    sound.init();
}

game::~game()
{
    bird.Free();
    pipe.Free();
    land.Free();
    sound.Free();
    free();
    releaseGraphic();
}

void game::releaseGraphic()
{
    //clean
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    SDL_DestroyRenderer( gRenderer );
    gRenderer = NULL;
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

bool game::initGraphic()
{
	bool success = true;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		gWindow = SDL_CreateWindow( "Peguin's dream", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

void game::display()
{
    SDL_RenderPresent(gRenderer);
    SDL_RenderClear(gRenderer);
}

void game::renderScoreSmall()
{
	string s = to_string(score);
	signed char len = s.length();
	LTexture image;

	for (signed char i = len - 1; i >= 0; i--)
	{
		signed char number = s[i] - '0';
		if (number == 1)
		{
			image.Load("res/number/1.png", 0.45);
		}
		else if (number == 2)
		{
			image.Load("res/number/2.png", 0.45);
		}
		else if (number == 3)
		{
			image.Load("res/number/3.png", 0.45);
		}
		else if (number == 4)
		{
			image.Load("res/number/4.png", 0.45);
		}
		else if (number == 5)
		{
			image.Load("res/number/5.png", 0.45);
		}
		else if (number == 6)
		{
			image.Load("res/number/6.png", 0.45);
		}
		else if (number == 7)
		{
			image.Load("res/number/7.png", 0.45);
		}
		else if (number == 8)
		{
			image.Load("res/number/8.png", 0.45);
		}
		else if (number == 9)
		{
			image.Load("res/number/9.png", 0.45);
		}
		else
		{
			image.Load("res/number/0.png", 0.45);
		}
		image.Render(245 - image.getWidth() * (len - i - 1) * 0.75 - 5 * (len - i - 1), 262);
	}
	image.free();
}

void game::renderScoreLarge()
{
    string s = to_string(score);
	signed char len = s.length();
	LTexture image;

	for (signed char i = 0; i < len; i++)
	{
		signed char number = s[i] - '0';
		if (number == 1)
		{
			image.Load("res/number/1.png", 1);
		}
		else if (number == 2)
		{
			image.Load("res/number/2.png", 1);
		}
		else if (number == 3)
		{
			image.Load("res/number/3.png", 1);
		}
		else if (number == 4)
		{
			image.Load("res/number/4.png", 1);
		}
		else if (number == 5)
		{
			image.Load("res/number/5.png", 1);
		}
		else if (number == 6)
		{
			image.Load("res/number/6.png", 1);
		}
		else if (number == 7)
		{
			image.Load("res/number/7.png", 1);
		}
		else if (number == 8)
		{
			image.Load("res/number/8.png", 1);
		}
		else if (number == 9)
		{
			image.Load("res/number/9.png", 1);
		}
		else
		{
			image.Load("res/number/0.png", 1);
		}
		image.Render((SCREEN_WIDTH - (image.getWidth() * len + (len - 1) * 10)) / 2 + (i + 30) * i, 100);
	}
	image.free();
}

void game::renderBestScore()
{
	ifstream fileIn("res/data/bestScore.txt");
	fileIn >> bestScore;
	ofstream fileOut("res/data/bestScore.txt", ios::trunc);

	if (score > bestScore)
	{
		bestScore = score;
	}
	string s = to_string(bestScore);
	signed char len = s.length();
	LTexture image;

	for (signed char i = len-1; i >= 0; i--)
	{
		signed char number = s[i] - '0';
		if (number == 1)
		{
			image.Load("res/number/1.png", 0.45);
		}
		else if (number == 2)
		{
			image.Load("res/number/2.png", 0.45);
		}
		else if (number == 3)
		{
			image.Load("res/number/3.png", 0.45);
		}
		else if (number == 4)
		{
			image.Load("res/number/4.png", 0.45);
		}
		else if (number == 5)
		{
			image.Load("res/number/5.png", 0.45);
		}
		else if (number == 6)
		{
			image.Load("res/number/6.png", 0.45);
		}
		else if (number == 7)
		{
			image.Load("res/number/7.png", 0.45);
		}
		else if (number == 8)
		{
			image.Load("res/number/8.png", 0.45);
		}
		else if (number == 9)
		{
			image.Load("res/number/9.png", 0.45);
		}
		else
		{
			image.Load("res/number/0.png", 0.45);
		}
		image.Render(245 - image.getWidth()*(len-i-1)*0.75 - 5*(len - i - 1), 315);
	}
	image.free();

	fileOut << bestScore;
	fileIn.close();
	fileOut.close();
}

void game::renderMessage()
{
	LTexture image;
	image.Load("res/image/message.png", 1);
	image.Render((SCREEN_WIDTH - image.getWidth()) / 2, 180);
	image.free();
}
void game::rendermenu()
{
    LTexture image;
    image.Load("res/image/sprites/menu_bg.png");
    image.Render(0,0);
    image.free();
}

bool IsWithinBounds(bool clicked, int x, int y, int x_button, int y_button, int w_button, int h_button) {
  if (clicked == false) return false;

  if (x < x_button) return false;

  if (x > x_button + w_button) return false;

  if (y < y_button) return false;

  if (y > y_button + h_button) return false;

  return true;
}

bool game::buttonplay()
{
    LTexture image;
    image.Load("res/image/sprites/play_button.png");
	int x = 125;
	int y = 300;
	int w = image.getWidth();
	int h = image.getHeight();
    image.Render(x,y);
    image.free();
	return IsWithinBounds(clicked, x_mouse, y_mouse, x, y, w, h);
}
bool game::outbutton()
{
    LTexture image;
    image.Load("res/image/sprites/out.png",0.9);
	int x = 275;
	int y = 215;
	int w = image.getWidth();
	int h = image.getHeight();
    image.Render(x,y);
    image.free();
	return IsWithinBounds(clicked, x_mouse, y_mouse, x, y, w, h);
}
bool game::tutorbutton()
{
    LTexture image;
    image.Load("res/image/sprites/tutor.png");
	int x = 10;
	int y = SCREEN_HEIGHT/3*2+image.getHeight();
	int w = image.getWidth();
	int h = image.getHeight();
    image.Render(x,y);
    image.free();
	return IsWithinBounds(clicked, x_mouse, y_mouse, x, y, w, h);
}

bool game::buttonquit()
{
     LTexture image;
    image.Load("res/image/sprites/quit_button.png");
    int x = 125;
	int y = 400;
	int w = image.getWidth();
	int h = image.getHeight();
    image.Render(x,y);
    image.free();
	return IsWithinBounds(clicked, x_mouse, y_mouse, x, y, w, h);

}

bool game::buttonsetting()
{
    LTexture image;
    image.Load("res/image/sprites/option_button.png");
    int x = 125;
	int y = 350;
	int w = image.getWidth();
	int h = image.getHeight();
    image.Render(x,y);
    image.free();
	return IsWithinBounds(clicked, x_mouse, y_mouse, x, y, w, h);
}


void game::renderBackground()
{
	LTexture image;
	image.Load("res/image/background.png", 1);
	image.Render(0, 0);
	image.free();
}

void game::renderBackgroundNight()
{
	LTexture image;
	image.Load("res/image/background-night.png", 1);
	image.Render(0, 0);
	image.free();
}

void game::renderLand()
{
	LTexture image;
	image.Load("res/image/land.png", 1);
	image.Render((SCREEN_WIDTH - image.getWidth()) / 2, SCREEN_HEIGHT- image.getHeight());
	image.free();
}

void game::resume()
{
	LTexture image;
	image.Load("res/image/resume.png", 1);
	image.Render(SCREEN_WIDTH - 50, 20);
	image.free();
}

void game::pause()
{
	LTexture image;
	image.Load("res/image/pause.png", 1);
	image.Render(SCREEN_WIDTH - 50, 20);
	image.free();
}

bool game::menubutton()
{
    LTexture image;
	image.Load("res/image/sprites/menu_button.png", 1);
	int x = (SCREEN_WIDTH - image.getWidth()) / 2;
	int y = 450;
	int w = image.getWidth();
	int h = image.getHeight();
	image.Render(x,y);
	image.free();
	return IsWithinBounds(clicked, x_mouse, y_mouse, x, y, w, h);
}

void game::renderPauseTab()
{
	LTexture image;
	image.Load("res/image/pausetabice.png", 1);
	image.Render((SCREEN_WIDTH - image.getWidth()) / 2, 225);
	image.free();
}
void game::rendertutortab()
{
	LTexture image;
	image.Load("res/image/tutor.png", 1);
	image.Render((SCREEN_WIDTH - image.getWidth()) / 2, 225);
	image.free();
}
void game::renderSettingTab()
{
	LTexture image;
	image.Load("res/image/setting.png", 1);
	image.Render((SCREEN_WIDTH - image.getWidth()) / 2, 225);
	image.free();
}

void game::lightskin()
{
	LTexture image;
	image.Load("res/image/rsz_xanh.png", 0.8);
	image.Render(105+115, 315-55);
	image.free();
}

void game::darkskin()
{
	LTexture image;
	image.Load("res/image/rsz_hong.png", 0.8);
	image.Render(105+115, 315-55);
	image.free();
}
void game::peace()
{
	LTexture image;
	image.Load("res/image/peace.png", 0.8);
	image.Render(105+58, 315);
	image.free();
}
void game::devil()
{
	LTexture image;
	image.Load("res/image/devil.png", 0.8);
	image.Render(105+58, 312);
	image.free();
}
bool game::changelevel()
{
    int x, y;
	SDL_GetMouseState(&x, &y);
	if ((x > 163 && x < 163+40) && (y > 315 && y < 315 + 40))
	{
		return true;
	}
	return false;
}

void game::sunbackgr()
{
    LTexture image;
	image.Load("res/image/sunicon.png", 0.8);
	image.Render(223, 310);
	image.free();
}
void game::moonbackgr()
{
    LTexture image;
	image.Load("res/image/moonicon.png", 1.2);
	image.Render(213, 288);
	image.free();
}
bool game::changebackgr()
{
    int x, y;
	SDL_GetMouseState(&x, &y);
	if ((x > 220 && x < 300) && (y > 322 && y < 322 + 20))
	{
		return true;
	}
	return false;

}

void game::nextButton()
{
	LTexture image;
	image.Load("res/image/nextRight.png", 1);
	image.Render(149+110, 322-55);
	image.Load("res/image/nextLeft.png", 1);
	image.Render(88+120, 322-55);
	image.free();
}

bool game::changeskin()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	if (((x > 149+110 && x < 149+13+110)  || (x > 88+120 && x < 88 + 120 + 13)) && (y > 322-55 && y < 322 + 16-55))
	{
		return true;
	}
	return false;
}

void game::renderGameOver()
{
	LTexture image;
	image.Load("res/image/gameover.png", 1);
	image.Render((SCREEN_WIDTH - image.getWidth()) / 2, 150);
	image.free();
}

void game::rendertrophy()
{
	LTexture image;

	if (score >= 20 && score <= 50)
	{
		image.Load("res/image/trophy/silver.png", 0.5);
	}
	else if (score > 50)
	{
		image.Load("res/image/trophy/gold.png", 0.5);
	}
	else
	{
		image.Load("res/image/trophy/cu.png", 0.5);
	}
	image.Render(100, 275);

	image.free();
}

void game::replay()
{
	LTexture image;
	image.Load("res/image/replay.png", 1);
	image.Render((SCREEN_WIDTH - image.getWidth()) / 2, 380);
	image.free();
}

bool game::checkReplay()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	if (x > (SCREEN_WIDTH - 100)/2 && x < (SCREEN_WIDTH + 100) / 2 && y > 380 && y < 380 + 60) //cai nut
	{
		return true;
	}
	return false;
}

void game::Restart()
{
    die = false;
    score = 0;
    bird.resetTime();
}

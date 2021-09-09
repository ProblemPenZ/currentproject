#ifndef GAME_H
#define GAME_H

#include <math.h>

#include <iostream>
using namespace std;
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <fstream>
#include <vector>

#include "object.h"

#define WIDTH 720
#define HEIGHT 480


class Game {
	public:
	Game();
	~Game();
	void loop();
	void update();
	void input();
	void render();
	void draw(Object o);
	void draw(const char* msg, int x, int y, int r, int g, int b);
	void drawcubeCirc(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius);
	void drawellipseline(SDL_Renderer* r, int x0, int y0, int radiusX, int radiusY);
	void playeranim();
	int drawCircle(SDL_Renderer * renderer, int x, int y, int radius);
	int fillCircle(SDL_Renderer * renderer, int x, int y, int radius);
	bool checkCollision( SDL_Rect a, SDL_Rect b );
	bool objectCollision(Object a, Object b);
	private:
	SDL_Renderer* ren;
	SDL_Window* win;
	TTF_Font *font;
	bool running;
	int count;
	int frameCount, timerFPS, lastFrame;
	int mousex, mousey;
    int bSpeedx = 10;
    int bSpeedy = 10;
    int pSpeed=0;
    int oSpeed=0;
    int pY, oY; 
    int pongX, pongY;
};




#endif //Game_H
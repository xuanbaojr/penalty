#include"Common.h" 

#ifndef BUTTON_H
#define BUTTON_H

class Button
{
public:
	SDL_Rect a;
	bool live;
	Button(int x, int y,int h, int w);
	void handle(SDL_Event *e);

};

#endif
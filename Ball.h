#ifndef BALL_H
#define BALL_H
#include"Common.h"

class Dot
{
    public:
		//The dimensions of the dot
		SDL_Rect B;
		//Initializes the variables
		Dot();
		void dir(int a);
		void dir1(int a);
		//Moves the dot
		bool move();
		bool move1();
		bool move2();
		void home();
		
		float xVel, yVel;
};
#endif
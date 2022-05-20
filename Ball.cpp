#include"Ball.h"

Dot::Dot()
{
	B.x = 630;
	B.y = 500;
	B.w = 40;
	B.h = 40;
}

void Dot::dir(int a)
{
	
	if(a==1)
	{
		xVel = 0;
		yVel = -15;
		
		
	}
	if(a==0)
	{
		xVel = -10;
		yVel = -15;
		
	}
	if(a==2)
	{
		xVel = 10;
		yVel = -15;
		
	}

	if(a==3)
	{
		xVel = 0;
		yVel = 0;
	}

}
void Dot::dir1(int a)
{
	
	if(a==1)
	{
		xVel = 0;
		yVel = -15;
		
		
	}
	if(a==0)
	{
		xVel = -15;
		yVel = -12.14;
		
	}
	if(a==2)
	{
		xVel = 15;
		yVel = -12.5;
		
	}

	if(a==3)
	{
		xVel = 0;
		yVel = 0;
	}

}
bool Dot::move()
{
	bool success = false;
	B.x += xVel;
	B.y += yVel;
	if((B.x < 400)|| (B.y < 210) || (B.x > 860))
	{
			B.x -= xVel;
			B.y -= yVel;
			success = true;
	}
	return success;

}

void Dot::home()
{
	B.x = 630;
	B.y = 500;
}
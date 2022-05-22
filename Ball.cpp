#include"Ball.h"

Dot::Dot()
{
	B.x = 630;
	B.y = 510;
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
		xVel = -11.5;
		yVel = -14.5;
		
	}
	if(a==2)
	{
		xVel = 11;
		yVel = -14.5;
		
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
		xVel = -0.9;
		yVel = -15;
		
		
	}
	if(a==0)
	{
		xVel = -13;
		yVel = -11;
		
	}
	if(a==2)
	{
		xVel = 15;
		yVel = -12.14;
		
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
	if((B.x < 410)|| (B.y < 250) || (B.x > 840))
	{
			B.x -= xVel;
			B.y -= yVel;
			success = true;
	}
	return success;
}

bool Dot::move1()
{
	bool success = false;
	B.x += xVel;
	B.y += yVel;
	if((B.x < 420)|| (B.y < 210) || (B.x > 840))
	{
			B.x -= xVel;
			B.y -= yVel;
			success = true;
	}
	return success;
}

bool Dot::move2()
{
	bool success = false;
	B.x += xVel;
	B.y += yVel;
	if( B.y < 310 )
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
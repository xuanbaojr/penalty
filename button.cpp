#include"button.h"

Button :: Button(int x, int y,int h, int w)
{ 
	live = false;
	a.x = x;
	a.y = y;
	a.h = h;
	a.w = w;
}

void Button::handle(SDL_Event *e)
{
	 if( e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );
		 //Check if mouse is in button
        bool inside = true;

        //Mouse is left of the button
        if( x < a.x )
        {
            inside = false;
        }
        //Mouse is right of the button
        else if( x > a.x + a.w )
        {
            inside = false;
        }
        //Mouse above the button
        else if( y < a.y )
        {
            inside = false;
        }
        //Mouse below the button
        else if( y > a.y + a.h )
        {
            inside = false;
        }
		 //Mouse is outside button
        if(inside == true) live = false;
    }
}
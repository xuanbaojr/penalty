
#include"Common.h"
#include"Ball.h"
#include"button.h"

//Texture wrapper class


class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );
		
		#if defined(SDL_TTF_MAJOR_VERSION)
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render( float x, float y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;
LTexture gMenuBack;

Mix_Music *gMusic = NULL;
Mix_Music *mBackground = NULL;
Mix_Music *mGoal = NULL;

Mix_Chunk *gHigh = NULL;
//Scene textures

LTexture gDotTexture;
LTexture background;
LTexture board;
LTexture gPointTexture;
SDL_Rect gSpriteClips2[2];
LTexture win;
LTexture mPlay;
LTexture lPlay;
//Globally used font
TTF_Font *gFont = NULL;

//Rendered texture
LTexture gTextTexture;

SDL_Rect gSpriteClips[ WALKING_ANIMATION_FRAMES ];


LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}


#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	
	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}
		
void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( float x, float y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}


void showball(Dot a)
{
	gDotTexture.render(a.B.x,a.B.y);
}

LTexture gKeeperTexture;
LTexture gButton;
LTexture arrow;

LTexture winL;
LTexture winM;
LTexture loseL;
LTexture loseM;

std::vector<LTexture> vecGL(100);
std::vector<LTexture> vecRL(100);
std::vector<LTexture> vecGM(100);
std::vector<LTexture> vecRM(100);


bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO  | SDL_INIT_AUDIO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "FootBall", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                    success = false;
                }
				 //Initialize SDL_ttf
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

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	gMusic = Mix_LoadMUS( "music/menu.wav" );
    if( gMusic == NULL )
    {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

	mBackground = Mix_LoadMUS( "music/background.wav" );
    if( mBackground == NULL )
    {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    //Load sound effects  
    gHigh = Mix_LoadWAV( "music/goal.wav" );
    if( gHigh == NULL )
    {
        printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

	for(int i=0;i<100;i++)
		{
			if( !vecGL[i].loadFromFile( "img/green.png" ) )
			{
				printf( "Failed to load Point texture!\n" );
				success = false;
			}
			if( !vecRL[i].loadFromFile( "img/red.png" ) )
			{
				printf( "Failed to load Point texture!\n" );
				success = false;
			}
			if( !vecGM[i].loadFromFile( "img/green.png" ) )
			{
				printf( "Failed to load Point texture!\n" );
				success = false;
			}
			if( !vecRM[i].loadFromFile( "img/red.png" ) )
			{
				printf( "Failed to load Point texture!\n" );
				success = false;
			}
		}


	if( !gMenuBack.loadFromFile( "img/menu.png" ) )
	{
		printf( "Failed to load Menu texture!\n" );
		success = false;
	}
	if( !winL.loadFromFile( "img/winL.png" ) )
	{
		printf( "Failed to load Menu texture!\n" );
		success = false;
	}if( !winM.loadFromFile( "img/winM.png" ) )
	{
		printf( "Failed to load Menu texture!\n" );
		success = false;
	}if( !loseL.loadFromFile( "img/loseL.png" ) )
	{
		printf( "Failed to load Menu texture!\n" );
		success = false;
	}if( !loseM.loadFromFile( "img/loseM.png" ) )
	{
		printf( "Failed to load Menu texture!\n" );
		success = false;
	}
	if( !mPlay.loadFromFile( "img/mplay.png" ) )
	{
		printf( "Failed to load Menu Button texture!\n" );
		success = false;
	}

	if( !lPlay.loadFromFile( "img/lplay.png" ) )
	{
		printf( "Failed to load Menu Button texture!\n" );
		success = false;
	}

	/*if( !win.loadFromFile( "img/win.png" ) )
	{
		printf( "Failed to load Win texture!\n" );
		success = false;
	}*/

	//Load dot texture
	if( !gDotTexture.loadFromFile( "img/ball1.png" ) )
	{
		printf( "Failed to load ball texture!\n" );
		success = false;
	}
	//Load Background
	if( !background.loadFromFile( "img/background1.png" ) )
	{
		printf( "Failed to load Background texture!\n" );
		success = false;
	}
	if( !arrow.loadFromFile( "img/arrow.png" ) )
	{
		printf( "Failed to load arrow texture!\n" );
		success = false;
	}

	if( !gKeeperTexture.loadFromFile( "img/character1.png"  ))
	{
		printf( "Failed to load goalkeeper animation texture!\n" );
		success = false;
	}
	else
	{
		//Set sprite clips
		gSpriteClips[ 0 ].x =   0;
		gSpriteClips[ 0 ].y =   0;
		gSpriteClips[ 0 ].w =  110;
		gSpriteClips[ 0 ].h = 182;

		gSpriteClips[ 1 ].x =  110;
		gSpriteClips[ 1 ].y =   0;
		gSpriteClips[ 1 ].w =  191;
		gSpriteClips[ 1 ].h = 182;
		
		gSpriteClips[ 2 ].x = 301;
		gSpriteClips[ 2 ].y =   0;
		gSpriteClips[ 2 ].w =  214;
		gSpriteClips[ 2 ].h = 182;

		gSpriteClips[ 3 ].x = 515;
		gSpriteClips[ 3 ].y =   0;
		gSpriteClips[ 3 ].w =  131;
		gSpriteClips[ 3 ].h = 182;

		gSpriteClips[ 4 ].x = 646;
		gSpriteClips[ 4 ].y =   0;
		gSpriteClips[ 4 ].w =  129;
		gSpriteClips[ 4 ].h = 182;

		gSpriteClips[ 5 ].x = 775;
		gSpriteClips[ 5 ].y =   0;
		gSpriteClips[ 5 ].w =  215;
		gSpriteClips[ 5 ].h = 182;

		gSpriteClips[ 6 ].x = 990;
		gSpriteClips[ 6 ].y =   0;
		gSpriteClips[ 6 ].w =  195;
		gSpriteClips[ 6 ].h = 182;

		gSpriteClips[ 7 ].x = 1185;
		gSpriteClips[ 7 ].y =   0;
		gSpriteClips[ 7 ].w =  105;
		gSpriteClips[ 7 ].h = 182;
	}
	
	return success;
}

void close()
{
	//Free loaded images
	gDotTexture.free();
	background.free();
	gKeeperTexture.free();
	gPointTexture.free();
	mPlay.free();
	lPlay.free();
	win.free();
	gButton.free();
	winL.free();
	winM.free();
	loseL.free();
	loseM.free();
	arrow.free();
	gMenuBack.free();
	TTF_CloseFont( gFont );
	for(int i=0;i<100;i++)
	{
		vecGL[i].free();
		vecRL[i].free();
		vecGM[i].free();
		vecRM[i].free();
	}

    gFont = NULL;
    Mix_FreeChunk( gHigh );
    gHigh = NULL;
    
    //Free the music
    Mix_FreeMusic( gMusic );
    gMusic = NULL;
	Mix_FreeMusic( mBackground );
    mBackground = NULL;
	Mix_FreeMusic( mGoal );
    mGoal = NULL;
	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	Mix_Quit();
}

int main( int argc, char* args[] )
{
	srand(time(NULL));
	bool pointL = false;
	bool pointM = false;

	int check = 0;
	int choose = -1;
	int cB = 0, cM = 0;
	
	int scoreL = 0; int scoreL1 = 5;
	int scoreM = 0; int scoreM1 = 5;

	bool winL1 = false;
	bool winM1 = false;

	int a = rand()%3;
	int b = -1; int k = -1;
	bool run = false;
	bool start = false;
	bool mMenu = true;

	std::vector<bool> shotGL(100);
	std::vector<bool> shotRL(100);
	std::vector<bool> shotGM(100);
	std::vector<bool> shotRM(100);
	for(int i = 0; i < 100; i++)
	{
		shotGL[i] = false;
		shotRL[i] = false;
		shotGM[i] = false;
		shotRM[i] = false;
	}

	
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	

			bool quit = false;
			SDL_Event e;
			static int kx;
			Dot dot;
			Button button(880,465,70,85);
			Button buttonL (1060,465,70,85);

			button.live = false;
			buttonL.live = false;
			
			//Mix_PlayMusic( gMusic, -1 );

			while( !quit  )
			{	
				
				if(check == 0){
					
					b = a;
					while( SDL_PollEvent( &e ) != 0 )
					{
						if( e.type == SDL_QUIT ) quit = true;
					
						else if( e.type == SDL_KEYDOWN )
						{
						switch( e.key.keysym.sym )
							{
								case SDLK_w:
									k = 1; run = true; break;
								case SDLK_a:	
									k = 0; run = true; break;
								case SDLK_d:	
									k = 2; run = true; break;
								case SDLK_SPACE:
									k = 3; run = true; break;
								case SDLK_p :
									start = true;  Mix_HaltMusic(); break;
							}
						}
						button.handle(&e);
						buttonL.handle(&e);
					}
					
					
					SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
					SDL_RenderClear( gRenderer );

					background.render(1,1);	
					if( cB == 0 && cM == 0) arrow.render(635,180);
					if( k==3 && cM > 0) arrow.render(635,180);

					SDL_Rect* currentClip = &gSpriteClips[frame];
	    
					gKeeperTexture.render( ( SCREEN_WIDTH - currentClip->w ) / 2 - kx, ( SCREEN_HEIGHT - currentClip->h ) /2 - 70, currentClip );					
					showball(dot);

					//if( Mix_PlayingMusic() == 0 )
     //                       {
     //                           //Play the music
     //                           Mix_PlayMusic( mBackground, -1 );
     //                       }
					if(start == false)
					{						
						gMenuBack.render(1,1);
\
						SDL_RenderPresent( gRenderer );
						button.live = true;
						buttonL.live = true;
						button.handle(&e);
						buttonL.handle(&e);
						
						if(buttonL.live == false) {check = 1; choose = 1;}
						if(button.live == false)  { check = 0; choose = 0; }

						if(button.live == false || buttonL.live == false) 
						{
							start = true;
							//Mix_HaltMusic();
						}
					}
					
					if(run == true && start == true)
					{
						dot.dir(b);	
						if(k==1)
						{
							if(dot.move2() == false )
							{
								dot.move2();
								frame = 3;
							}
							else 
							{	
								if(choose == 1)
								{
									cM++;
									run = false;
									pointM = true;
									if(b!=k) {
									shotGM[cM] = true;
									scoreM ++;
							//		scoreM1 = 5 - cM + scoreM;
								}
								else
								{
									shotRM[cM] = true;
							//		scoreM1 = 5 - cM + scoreM;
								}
								}
								else{
								cB++;
								run = false;
								pointL = true;
								if(b!=k) {
									shotGL[cB] = true;
									scoreL ++;
							//		scoreL1 = 5 - cB + scoreL;
								}
								else
								{
									shotRL[cB] = true;
							//		scoreL1 = 5 - cB + scoreL;
								}
								}
								
								Mix_PlayChannel( -1, gHigh, 0 );
								a = rand()%3;
								//cM++;
								check = 1;

							}	
						}
						
						if(k==0)
						{
							if(dot.move() == false )
							{
								dot.move();
								frame = 6;
								kx = 130;
							}
							else 
							{	
								if(choose == 1)
								{
									cM++;
									run = false;
									pointM = true;
									if(b!=k) {
									shotGM[cM] = true;
									scoreM ++;
							//		scoreM1 = 5 - cM + scoreM;
								}
								else
								{
									shotRM[cM] = true;
							//		scoreM1 = 5 - cM + scoreM;
								}
								}
								else{
								cB++;
								run = false;
								pointL = true;
								if(b!=k) {
									shotGL[cB] = true;
									scoreL ++;
							//		scoreL1 = 5 - cB + scoreL;
								}
								else
								{
									shotRL[cB] = true;
							//		scoreL1 = 5 - cB + scoreL;
								}
								}
								Mix_PlayChannel( -1, gHigh, 0 );
								a = rand()%3;
								//cM++;
								check = 1;
							
							}	
						}

						if(k==2)
						{
							if(dot.move() == false )
							{
								dot.move();
								frame = 1;
								kx = -130;
							}
							else 
							{	
								if(choose == 1)
								{
									cM++;
									run = false;
									pointM = true;
									if(b!=k) {
									shotGM[cM] = true;
									scoreM ++;
								//	scoreM1 = 5 - cM + scoreM;
								}
								else
								{
									shotRM[cM] = true;
								//	scoreM1 = 5 - cM + scoreM;
								}
								}
								else{
								cB++;
								run = false;
								pointL = true;
								if(b!=k) {
									shotGL[cB] = true;
									scoreL ++;
								//	scoreL1 = 5 - cB + scoreL;
								}
								else
								{
									shotRL[cB] = true;
								//	scoreL1 = 5 - cB + scoreL;
								}
								}
								Mix_PlayChannel( -1, gHigh, 0 );
								a = rand()%3;
								//cM++;
								check = 1;
							}	
						}

						if(k==3)
						{	
			
								dot.home();
								frame = 0;
								kx = 0;
								run = false;
								std::cout<<cB<<" "<<scoreL<<" "<<scoreL1;

						}
					}

					scoreM1 = 5 - cM + scoreM;
					scoreL1 = 5 - cB + scoreL;
					if(pointL == true || pointM  == true){
																
								for(int i=0;i<100;i++){
																	
									if(shotGL[i]== true)  vecGL[i].render(92 + i*44 ,650);									
								
									if(shotRL[i] == true) vecRL[i].render(92 + i*44 ,650);
									
									if(shotGM[i] == true) vecGM[i].render(429 + i*44 ,650);
																
									if(shotRM[i] == true) vecRM[i].render(429 + i*44 ,650);		
															
								}
							//	std::cout<<"a";
					}					
					
					SDL_RenderPresent( gRenderer );
					}
					if( cB <= 5 )
					{
						if(scoreL > scoreM1  ){
							winL1 = true;
						}

					}
					if(cB>5)
					{	
						if(choose == 1 && cB ==cM){
						if(shotGM[cB] == true && shotGL[cB] == false ) winM1 = true ;
						if(shotGM[cB] == false && shotGL[cB] == true )  winL1 = true; 
						}
					}
					while(winL1 == true )
					{
						std::cout<<"l";
						if(choose == 1) winL.render(1,1);
						if(choose == 0) loseM.render(1,1);
						SDL_RenderPresent( gRenderer );
						
						while( SDL_PollEvent( &e ) != 0 ){
					

						if( e.type == SDL_QUIT ) quit = true;
					
						else if( e.type == SDL_KEYDOWN ){
						
						// button.handle(&e)
						switch( e.key.keysym.sym )
							{
								case SDLK_m:
									cB = 0; cM = 0; scoreL = 0; scoreM = 0; scoreL1 = 5; scoreM1 = 5;
									for(int i = 0; i < 100; i++)
									{
										shotGL[i] = false;
										shotRL[i] = false;
										shotGM[i] = false;
										shotRM[i] = false;
									}
									winL1 = false;
									winM1 = false;
								//	check = 0;
									kx = 0;
									dot.home();
									frame = 0;
									start = false;
									break;						
							}

						}
					}

					}
					
				if(check == 1){

					k = a;
					while( SDL_PollEvent( &e ) != 0 )
					{
					//User requests quit
						if( e.type == SDL_QUIT )
						{
							quit = true;
						}
						else if( e.type == SDL_KEYDOWN )
						{
                        //Select surfaces based on key press
						switch( e.key.keysym.sym )
						{
							case SDLK_UP:
								b = 1; run = true; break;
							
							case SDLK_LEFT:
								b = 0; run = true; break;

							case SDLK_RIGHT:
								b = 2; run = true; break;

							case SDLK_SPACE:
								b = 3; run = true; break;
							}
						}
						button.handle(&e);
						buttonL.handle(&e);
					}
				
					//Clear screen
					SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
					SDL_RenderClear( gRenderer );

					background.render(1,1);
					if( cM == 0 && cB == 0) arrow.render(635,460);
					if( b==3 ) arrow.render(635,460);

						
					SDL_Rect* currentClip = &gSpriteClips[frame];

	                gKeeperTexture.render( ( SCREEN_WIDTH - currentClip->w ) / 2 - kx , ( SCREEN_HEIGHT - currentClip->h ) /2 - 70, currentClip );
					
					showball(dot);
					if( Mix_PlayingMusic() == 0 )
                            {
                                //Play the music
                                Mix_PlayMusic( mBackground, -1 );
                            }
					if(start == false)
					{						
						gMenuBack.render(1,1);
\
						SDL_RenderPresent( gRenderer );
						button.live = true;
						buttonL.live = true;
						button.handle(&e);
						buttonL.handle(&e);
						
						if(buttonL.live == false) check = 1; 
						if(button.live == false)  check = 0;

						if(button.live == false || buttonL.live == false) 
						{
							start = true;
							Mix_HaltMusic();
						}
					}
				if(run == true && start == true)
					{
						dot.dir1(b);	
						if(k==1)
						{
							if(dot.move2() == false )
							{
								dot.move2();
								frame = 4;
							}
							else 
							{	
								if(choose == 1)
								{
									cB++;
									run = false;
									pointL = true;
									if(b!=k) {
									shotGL[cB] = true;
									scoreL ++;
								//	scoreL1 = 5 - cB + scoreL;
								}
								else
								{
									shotRL[cB] = true;
								//	scoreL1 = 5 - cB + scoreL;
								}
								}
								else{
								cM++;
								run = false;
								pointM = true;
								if(b!=k) {
									shotGM[cM] = true;
									scoreM ++;
								//	scoreM1 = 5 - cM + scoreM;
								}
								else
								{
									shotRM[cM] = true;
								//	scoreM1 = 5 - cM + scoreM;
								}
								}
								
								Mix_PlayChannel( -1, gHigh, 0 );
								a = rand()%3;
								//cM++;
								check = 0;

							}	
						}
						
						if(k==0)
						{
							if(dot.move1() == false )
							{
								dot.move1();
								frame = 5;
								kx = 130;
							}
							else 
							{	
								if(choose == 1)
								{
									cB++;
									run = false;
									pointL = true;
									if(b!=k) {
									shotGL[cB] = true;
									scoreL ++;
								//	scoreL1 = 5 - cB + scoreL;
								}
								else
								{
									shotRL[cB] = true;
								//	scoreL1 = 5 - cB + scoreL;
								}
								}
								else{
								cM++;
								run = false;
								pointM = true;
								if(b!=k) {
									shotGM[cM] = true;
									scoreM ++;
								//	scoreM1 = 5 - cM + scoreM;
								}
								else
								{
									shotRM[cM] = true;
								//	scoreM1 = 5 - cM + scoreM;
								}
								}
								
								Mix_PlayChannel( -1, gHigh, 0 );
								a = rand()%3;
								//cM++;
								check = 0;
							}	
						}

						if(k==2)
						{
							if(dot.move1() == false )
							{
								dot.move1();
								frame = 2;
								kx = -130;
							}
							else 
							{	
								if(choose == 1)
								{
									cB++;
									run = false;
									pointL = true;
									if(b!=k) {
									shotGL[cB] = true;
									scoreL ++;
								//	scoreL1 = 5 - cB + scoreL;
								}
								else
								{
									shotRL[cB] = true;
								//	scoreL1 = 5 - cB + scoreL;
								}
								}
								else{
								cM++;
								run = false;
								pointM = true;
								if(b!=k) {
									shotGM[cM] = true;
									scoreM ++;
								//	scoreM1 = 5 - cM + scoreM;
								}
								else
								{
									shotRM[cM] = true;
								//	scoreM1 = 5 - cM + scoreM;
								}
								}
								
								Mix_PlayChannel( -1, gHigh, 0 );
								a = rand()%3;
								//cM++;
								check = 0;
							}	
						}

						if(b==3)
						{	
			
								dot.home();
								frame = 7;
								kx = 0;
								run = false;
								std::cout<<cB<<" "<<scoreL<<" "<<scoreL1;

						}
					}
					scoreM1 = 5 - cM + scoreM;
					scoreL1 = 5 - cB + scoreL;
					if(pointM == true || pointL == true ){					
								for(int i=0;i<100;i++){

									if(shotGL[i]== true)  vecGL[i].render(92 + i*44 ,650);									
								
									if(shotRL[i] == true) vecRL[i].render(92 + i*44 ,650);
									
									if(shotGM[i] == true) vecGM[i].render(429 + i*44 ,650);
																
									if(shotRM[i] == true) vecRM[i].render(429 + i*44 ,650);	
																									
								}
							//	std::cout<<"b";
					}					
					
					SDL_RenderPresent( gRenderer );
					}
					
					if( cM <= 5 )
					{
						 if(scoreM > scoreL1  )  winM1 = true;
					}

					else
					{	
						if(choose == 0 && cB == cM){
						if(shotGM[cM] == true && shotGL[cM] == false ) winM1 = true ;
						if(shotGM[cM] == false && shotGL[cM] == true )  winL1 = true; 
						}
						
					}
					while(winL1 == true )
					{
						std::cout<<"l";
						if(choose == 1) winL.render(1,1);
						if(choose == 0) loseM.render(1,1);
						SDL_RenderPresent( gRenderer );
						
						while( SDL_PollEvent( &e ) != 0 ){
						
						if( e.type == SDL_QUIT ) quit = true;
					
						else if( e.type == SDL_KEYDOWN ){
						
						switch( e.key.keysym.sym )
							{
								case SDLK_m:
									cB = 0; cM = 0; scoreL = 0; scoreM = 0; scoreL1 = 5; scoreM1 = 5;
									for(int i = 0; i < 100; i++)
									{
										shotGL[i] = false;
										shotRL[i] = false;
										shotGM[i] = false;
										shotRM[i] = false;
									}
									winL1 = false;
									winM1 = false;
									check = 0;
									kx = 0;
									dot.home();
									frame = 0;
									start = false;
									break;						
							}
						}
					}

					}
					while(winM1 == true )
					{
						std::cout<<"l";
					//	showButton(button, mPlay);
						if(choose == 1) loseL.render(1,1);
						if(choose == 0) winM.render(1,1);
					
						SDL_RenderPresent( gRenderer );

						while( SDL_PollEvent( &e ) != 0 ){				
						
						if( e.type == SDL_QUIT ) quit = true;
					
						else if( e.type == SDL_KEYDOWN ){
						
						switch( e.key.keysym.sym )
							{
								case SDLK_m:
									cB = 0; cM = 0; scoreL = 0; scoreM = 0; scoreL1 = 5; scoreM1 = 5;
									for(int i = 0; i < 100; i++)
									{
										shotGL[i] = false;
										shotRL[i] = false;
										shotGM[i] = false;
										shotRM[i] = false;
									}
									winL1 = false;
									winM1 = false;
									check = 0;
									kx = 0;
									dot.home();
									frame = 0;
									start = false;
									break;						
							}
						}
					}

					}
					
			}	
		}
	}
	close();
	
	return 0;
}
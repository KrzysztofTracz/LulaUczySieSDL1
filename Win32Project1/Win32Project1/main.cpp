#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

const int PADDLE_WIDTH  = 25;
const int PADDLE_HEIGHT = 200;
const int BALL_WIDTH = 50;
const int BALL_HEIGHT = 50;
const float PADDLE_SPEED = 300.0;
const float BALL_SPEED = 500.0;

int fun(int width)
{
	return SCREEN_WIDTH - width;
}


int main(int argc, char* args[])
{

	SDL_Window* gWindow = NULL;

	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	//Create window
	gWindow = SDL_CreateWindow("LULA UCZY SIE ZYC", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	//Get window surface
	screenSurface = SDL_GetWindowSurface(gWindow);

	//Fill the surface white
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255,0,255));

	//Update the surface


	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	//The window renderer
	SDL_Renderer* gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

	IMG_Init(IMG_INIT_JPG);

	SDL_Surface* loadedSurface = IMG_Load("lula.jpg");
	SDL_Texture* gTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
	SDL_FreeSurface(loadedSurface);

	loadedSurface = IMG_Load("lula.jpg");
	SDL_Texture* gTexture2 = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
	SDL_FreeSurface(loadedSurface);

	SDL_Rect p1, p2, b;
	p1.x = 0;
	p1.y = 0;
	p1.w = PADDLE_WIDTH;
	p1.h = PADDLE_HEIGHT;

	p2.x = SCREEN_WIDTH - PADDLE_WIDTH;
	p2.y = 0;
	p2.w = PADDLE_WIDTH;
	p2.h = PADDLE_HEIGHT;

	b.x = SCREEN_WIDTH / 2;
	b.y = 0;
	b.w = BALL_WIDTH;
	b.h = BALL_HEIGHT;


	float p1x = 0.0;
	float p1y = 0.0;
	float p2x = p2.x;
	float p2y = 0.0;

	float bx = b.x;
	float by = b.y;
	
	struct wektor2
	{
		float x;
		float y;
	};

	wektor2 kierunek;
	kierunek.x = 0.5f;
	kierunek.y = 0.5f;

	float prevTime = (float)SDL_GetTicks()/1000.0f;
	//While application is running
	while (!quit)
	{
		float currTime = (float)SDL_GetTicks() / 1000.0f;
		float deltaTime = currTime - prevTime;
		prevTime = currTime;

		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		if (currentKeyStates[SDL_SCANCODE_UP])
		{
			p2y -= PADDLE_SPEED * deltaTime;
		}
		else if (currentKeyStates[SDL_SCANCODE_DOWN])
		{
			p2y += PADDLE_SPEED * deltaTime;
		}

		if (currentKeyStates[SDL_SCANCODE_W])
		{
			p1y -= PADDLE_SPEED * deltaTime;
		}
		else if (currentKeyStates[SDL_SCANCODE_S])
		{
			p1y += PADDLE_SPEED * deltaTime;
		}

		if (p1y > SCREEN_HEIGHT-PADDLE_HEIGHT)
		{
			p1y = SCREEN_HEIGHT- PADDLE_HEIGHT;
		}

		if (p2y > SCREEN_HEIGHT - PADDLE_HEIGHT)
		{
			p2y = SCREEN_HEIGHT - PADDLE_HEIGHT;
		}

		if (p1y < 0 )
		{
			p1y = 0;
		}

		if (p2y < 0)
		{
			p2y = 0;
		}

	
		bx += kierunek.x * BALL_SPEED * deltaTime;
		by += kierunek.y * BALL_SPEED * deltaTime;
		
		if (bx > SCREEN_WIDTH - BALL_WIDTH)
		{
			//bx = SCREEN_WIDTH - BALL_WIDTH;
			//kierunek.x = -kierunek.x;
			bx = (SCREEN_WIDTH / 2.0f) - (BALL_WIDTH / 2.0f);
			by = (SCREEN_HEIGHT / 2.0f) - (BALL_HEIGHT / 2.0f);
		}

		if (by > SCREEN_HEIGHT - BALL_WIDTH)
		{
			by = SCREEN_HEIGHT - BALL_WIDTH;
			kierunek.y = -kierunek.y;
		}
		
		if (by < 0)
		{
			by = 0;
			kierunek.y = -kierunek.y;
		}
		
		if (bx < 0)
		{
			//bx = 0;
			//kierunek.x = -kierunek.x;

			bx = (SCREEN_WIDTH / 2.0f) - (BALL_WIDTH / 2.0f);
			by = (SCREEN_HEIGHT / 2.0f) - (BALL_HEIGHT/ 2.0f);
		}
		
	

		

		if (by + BALL_HEIGHT > p1y )
		{
			if (by < p1y + PADDLE_HEIGHT )
			{
				if (bx < 0 + PADDLE_WIDTH)
				{
					bx = 0 + PADDLE_WIDTH;
					kierunek.x = -kierunek.x;
				}
			}
		}


		if (by + BALL_HEIGHT > p2y)
		{
			if (by < p2y + PADDLE_HEIGHT)
			{
				if (bx > SCREEN_WIDTH - PADDLE_WIDTH- BALL_WIDTH)
				{
					bx = SCREEN_WIDTH - PADDLE_WIDTH- BALL_WIDTH;
					kierunek.x = -kierunek.x;
				}
			}
		}




		b.x = bx;
		b.y = by;
		p1.y = p1y;
		p1.x = p1x;
		p2.y = p2y;
		p2.x = p2x;

		//Clear screen
		SDL_RenderClear(gRenderer);

		//Render texture to screen
		SDL_RenderCopy(gRenderer, gTexture, NULL, &p1);
		SDL_RenderCopy(gRenderer, gTexture, NULL, &p2);
		SDL_RenderCopy(gRenderer, gTexture, NULL, &b);

		//Update screen
		SDL_RenderPresent(gRenderer);
	}

	SDL_DestroyTexture(gTexture);

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();


	return 0;
}


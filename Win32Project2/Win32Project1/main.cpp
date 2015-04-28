#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <ctime>

const int SCREEN_WIDTH = 768;
const int SCREEN_HEIGHT = 768;
const int LEVEL_SIZE = 20;

const int FIELD_SIZE = SCREEN_HEIGHT / LEVEL_SIZE;
int tab[LEVEL_SIZE][LEVEL_SIZE];
int kierunekX = 0;
int kierunekY = 0;

void PobierzGlowe(int& x, int& y)
{
	for (int i = 0; i < LEVEL_SIZE; i++)
	{
		for (int j = 0; j < LEVEL_SIZE; j++)
		{
			if (tab[i][j] == 1)
			{
				x = i;
				y = j;
			}
		}
	}
}

void PostawWisnie()
{
	int x = 0;
	int y = 0;
	do
	{
		x = rand() % LEVEL_SIZE;
		y = rand() % LEVEL_SIZE;
	} while (tab[x][y] != 0);

	tab[x][y] = -1;
}

void Restart()
{
	system("CLS");

	for (int i = 0; i <LEVEL_SIZE; i++)
	{
		for (int j = 0; j < LEVEL_SIZE; j++)
		{
			tab[i][j] = 0;
		}
	}

	tab[rand() % LEVEL_SIZE][rand() % LEVEL_SIZE] = 1;

	kierunekX = 0;
	kierunekY = 1;

	PostawWisnie();

	for (int i = 0; i <LEVEL_SIZE; i++)
	{
		for (int j = 0; j < LEVEL_SIZE; j++)
		{
			std::cout << tab[j][i];
		}
		std::cout << std::endl;
	}
}

struct wektor2
{
	float x;
	float y;
};

int main(int argc, char* args[])
{
#pragma region INICJALIZACJA OKNA

	SDL_Window* gWindow = NULL;

	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	//Create window
	gWindow = SDL_CreateWindow("LULA UCZY SIE ZYC 2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	//Get window surface
	screenSurface = SDL_GetWindowSurface(gWindow);

	//Fill the surface white
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255,0,255));

	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	//The window renderer
	SDL_Renderer* gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	SDL_Surface* loadedSurface = IMG_Load("lula.jpg");
	SDL_Texture* gTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
	SDL_FreeSurface(loadedSurface);

	loadedSurface = IMG_Load("cherry.png");
	SDL_Texture* cherryTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
	SDL_FreeSurface(loadedSurface);
#pragma endregion

	//////////////////////////////////////////////////////
	/// INICJALIZACJA

	srand(time(NULL));
	Restart();

	bool up = false, down = false, left = false, right = false, r = false;

	//////////////////////////////////////////////////////

	float prevTime = (float)SDL_GetTicks()/1000.0f;
	//While application is running
	float czas = 0;
	
	while (!quit)
	{

#pragma region
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

#pragma endregion
		//////////////////////////////////////////////////////
		/// POBIERAMY INPUT

		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		
		if (currentKeyStates[SDL_SCANCODE_R] && !r)
		{
			r = true;

			Restart();
			czas = 0.0f;
		}
		else if (!currentKeyStates[SDL_SCANCODE_R])
		{
			r = false;
		}

		if (currentKeyStates[SDL_SCANCODE_UP] && !up)
		{
			up = true;
			kierunekY = -1;
			kierunekX = 0;
		}
		else if (!currentKeyStates[SDL_SCANCODE_UP])
		{
			up = false;
		}
		
		if (currentKeyStates[SDL_SCANCODE_DOWN] && !down)
		{
			down = true;
			kierunekY = 1;
			kierunekX = 0;
		}
		else if (!currentKeyStates[SDL_SCANCODE_DOWN])
		{
			down = false;
		}
		
		if (currentKeyStates[SDL_SCANCODE_LEFT] && !left)
		{
			left = true;
			kierunekY = 0;
			kierunekX = -1;
		}
		else if (!currentKeyStates[SDL_SCANCODE_LEFT])
		{
			left = false;
		}
		
		if (currentKeyStates[SDL_SCANCODE_RIGHT] && !right)
		{
			right = true;
			kierunekY = 0;
			kierunekX = 1;
		}
		else if (!currentKeyStates[SDL_SCANCODE_RIGHT])
		{
			right = false;
		}
		//////////////////////////////////////////////////////
		/// LOGIKA GRY CO KLATKE
		czas += deltaTime;

		if (czas >= 0.5f)
		{
			czas = 0;
			int x, y;
			PobierzGlowe(x, y);

			if (x + kierunekX == LEVEL_SIZE ||
				x + kierunekX == -1 ||
				y + kierunekY == LEVEL_SIZE ||
				y + kierunekY == -1)
			{
				Restart();
			}
			else
			{
				tab[x + kierunekX][y + kierunekY] = 1;
				tab[x][y] = 0;
			}
		}
		
		//////////////////////////////////////////////////////
		/// RYSOWANIE NA EKRAN
		SDL_RenderClear(gRenderer);

		SDL_Rect dst;
		dst.w = FIELD_SIZE;
		dst.h = FIELD_SIZE;

		for (int i = 0; i < LEVEL_SIZE; i++)
		{
			for (int j = 0; j < LEVEL_SIZE; j++)
			{
				dst.x = i*FIELD_SIZE;
				dst.y = j*FIELD_SIZE;

				if (tab[i][j]==1)
				{
					SDL_RenderCopy(gRenderer, gTexture, NULL, &dst);
				}
				else if (tab[i][j] == -1)
				{
					SDL_RenderCopy(gRenderer, cherryTexture, NULL, &dst);
				}
			}

		}
		
		
		SDL_RenderPresent(gRenderer);
	}

#pragma region ZAMKNIECIE APLIKACJI

	SDL_DestroyTexture(gTexture);

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
#pragma endregion

	return 0;
}


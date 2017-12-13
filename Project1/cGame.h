#ifndef _GAME_H
#define _GAME_H

/*
==================================================================================
cGame.h
==================================================================================
*/

#include <SDL.h>

// Game specific includes
#include "asteroidsGame.h"


using namespace std;

class cGame
{
public:
	cGame();

	void initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void cleanUp(SDL_Window* theSDLWND);
	void render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre);
	void update();
	void update(double deltaTime);
	bool getInput(bool theLoop);
	double getElapsedSeconds();

	static cGame* getInstance();

private:

	static cGame* pInstance;
	// for framerates
	time_point< high_resolution_clock > m_lastTime;
	time_point< high_resolution_clock > m_CurrentTime;
	duration< double > deltaTime;
	bool loop;
	// Sprites for displaying background and rocket textures
	cSprite spriteBkgd;
	cPlayer1 Player1;
	cAsteroid theAsteroid;
	cBullet theBullet;
	//bool loop; // might need changed
	// game related variables
	vector<LPCSTR> textureName;
	vector<LPCSTR> textName;
	vector<LPCSTR> texturesToUse;
	vector<cAsteroid*> theAsteroids;
	vector<cBullet*> theBullets;
	// Fonts to use
	vector<LPCSTR> fontList;
	vector<LPCSTR> fontsToUse;
	// Text for Game
	vector<LPCSTR> gameTextList;
	// Game Sounds
	vector<LPCSTR> soundList;
	vector<soundType> soundTypes;
	vector<LPCSTR> soundsToUse;

	//Creat vector array of button textures
	vector<LPCSTR> btnNameList;//needed for menu
	vector<LPCSTR> btnTexturesToUse;
	vector<SDL_Point> btnPos;
	vector<cButton> theButtons;

	SDL_Rect pos;
	FPoint scale;

	// Game objects
	// Define the elements and there position in/on the array/map
	int renderWidth, renderHeight;
	gameState theGameState;
	btnTypes theBtnType; //------------
	cTexture* tempTextTexture;
	SDL_Point theAreaClicked; //-----------
	int score;
	int spawnAmount;
	string scoreAsString;
	bool scoreChanged;
	SDL_Point theTileClicked;
	SDL_Point mapTileCliked;
	SDL_Rect aRect;
	
	void createAsteroid();
};

#endif

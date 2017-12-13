/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();
static cButtonMgr* theButtonMgr = cButtonMgr::getInstance(); //used to find buttons (may have to go)


/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}


void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);
	
	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();
	score = 0;
	spawnAmount = 0;
	
	/*
	theButtonMgr->getBtn("menu_btn")->setSpritePos({ 500, 500 });
	theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
	theButtonMgr->getBtn("exit_btn")->setSpritePos({ 500, 575 });
	theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
	*/


	// Store the textures
	textureName = { "asteroid1","bullet","Player1","theBackground"};
	texturesToUse = { "Images\\alan.png", "Images\\bulletNew.png", "Images\\rocket.png", "Images\\background.png"};
	for (int tCount = 0; tCount < textureName.size(); tCount++)
	{	
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}
	// Create textures for Game Dialogue (text)
	fontList = { "digital", "spaceAge","Starjout","Optimal"};
	fontsToUse = { "Fonts/digital-7.ttf", "Fonts/space age.ttf", "Fonts/Starjout.ttf", "Fonts/Optimal.ttf" };
	for (int fonts = 0; fonts < fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 36);
	}
	gameTextList = { "SPACE ATTACK", "score = 0" };
	
	theTextureMgr->addTexture("Title", theFontMgr->getFont("Optimal")->createTextTexture(theRenderer, gameTextList[0], SOLID, {255,255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Score", theFontMgr->getFont("Optimal")->createTextTexture(theRenderer, gameTextList[1], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));

	// Store the textures
	btnNameList = { "exit_btn", "instructions_btn", "load_btn", "menu_btn", "play_btn", "save_btn", "settings_btn" };
	btnTexturesToUse = { "Images/Buttons/button_exit.png", "Images/Buttons/button_instructions.png", "Images/Buttons/button_load.png", "Images/Buttons/button_menu.png", "Images/Buttons/button_play.png", "Images/Buttons/button_save.png", "Images/Buttons/button_settings.png" };
	btnPos = { { 400, 375 },{ 400, 300 },{ 400, 300 },{ 500, 500 },{ 400, 300 },{ 740, 500 },{ 400, 300 } };
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		theTextureMgr->addTexture(btnNameList[bCount], btnTexturesToUse[bCount]);
	}
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		cButton * newBtn = new cButton();
		newBtn->setTexture(theTextureMgr->getTexture(btnNameList[bCount]));
		newBtn->setSpritePos(btnPos[bCount]);
		newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[bCount])->getTWidth(), theTextureMgr->getTexture(btnNameList[bCount])->getTHeight());
		theButtonMgr->add(btnNameList[bCount], newBtn);
	}
	theGameState = MENU;
	theBtnType = EXIT;

	// Load game sounds
	soundList = { "theme", "shot", "explosion" };
	soundTypes = { SFX, SFX, SFX };
	soundsToUse = { "Audio/Traveler.wav", "Audio/shot007.wav", "Audio/explosion2.wav" };
	for (int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	theSoundMgr->getSnd("theme")->play(-1);

	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());

	Player1.setSpritePos({425 ,600});
	Player1.setTexture(theTextureMgr->getTexture("Player1"));
	Player1.setSpriteDimensions(theTextureMgr->getTexture("Player1")->getTWidth(), theTextureMgr->getTexture("Player1")->getTHeight());
	Player1.setPlayer1Velocity({ 5, 5 });


	// Create vector array of textures

	

}

void cGame::createAsteroid()
{
	theAsteroids.push_back(new cAsteroid);
	int numberOrAsteroids = theAsteroids.size();
	int lastIndex = numberOrAsteroids - 1;

	theAsteroids[lastIndex]->setSpritePos({ 100 * (rand() % 5 + 1), 50 * (rand() % 5 + 1) });
	theAsteroids[lastIndex]->setSpriteTranslation({ 3,3 });
	int randAsteroid = rand() % 4;
	theAsteroids[lastIndex]->setTexture(theTextureMgr->getTexture(textureName[0]));
	theAsteroids[lastIndex]->setSpriteDimensions(theTextureMgr->getTexture(textureName[0])->getTWidth(), theTextureMgr->getTexture(textureName[0])->getTHeight());
	theAsteroids[lastIndex]->setAsteroidVelocity({ 3, 3 });
	theAsteroids[lastIndex]->setActive(true);
}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	//bool loop = true;
	loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
	}
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);
	switch (theGameState)
	{
	case MENU:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		// Render the Title
		tempTextTexture = theTextureMgr->getTexture("Title");
		pos = { 325, 30, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 50, 0 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		/*tempTextTexture = theTextureMgr->getTexture("CreateTxt");
		pos = { 300, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("DragDropTxt");
		pos = { 300, 75, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		// Render Button*/
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 400, 375 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());

		score = 0;
	}
	break;
	
	
	case PLAYING:
	{
		SDL_RenderClear(theRenderer);
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		
		// Render each asteroid in the vector array
		for (int draw = 0; draw < theAsteroids.size(); draw++)
		{
			theAsteroids[draw]->render(theRenderer, &theAsteroids[draw]->getSpriteDimensions(), &theAsteroids[draw]->getSpritePos(), theAsteroids[draw]->getSpriteRotAngle(), &theAsteroids[draw]->getSpriteCentre(), theAsteroids[draw]->getSpriteScale());
		}
		// Render each bullet in the vector array
		for (int draw = 0; draw < theBullets.size(); draw++)
		{
			theBullets[draw]->render(theRenderer, &theBullets[draw]->getSpriteDimensions(), &theBullets[draw]->getSpritePos(), theBullets[draw]->getSpriteRotAngle(), &theBullets[draw]->getSpriteCentre(), theBullets[draw]->getSpriteScale());
		}
		// Render the Title
		cTexture* tempTextTexture = theTextureMgr->getTexture("Title");
		SDL_Rect pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		// Render the score
		if (scoreChanged)
		{
			gameTextList[1] = scoreAsString.c_str();
			theTextureMgr->addTexture("Score", theFontMgr->getFont("Optimal")->createTextTexture(theRenderer, gameTextList[1], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
			scoreChanged = false;
		}

		tempTextTexture = theTextureMgr->getTexture("Score");
		pos = { 700, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		// render the rocket
		Player1.render(theRenderer, &Player1.getSpriteDimensions(), &Player1.getSpritePos(), Player1.getSpriteRotAngle(), &Player1.getSpriteCentre(), Player1.getSpriteScale());
		SDL_RenderPresent(theRenderer);
		/*tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		theTileMap.render(theSDLWND, theRenderer, theTextureMgr, textureName);
		theTilePicker.render(theSDLWND, theRenderer, theTextureMgr, textureName);
		theTileMap.renderGridLines(theRenderer, aRect, aColour);*/
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 740, 650 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
		theButtonMgr->getBtn("load_btn")->setSpritePos({ 740, 500 });
		theButtonMgr->getBtn("load_btn")->render(theRenderer, &theButtonMgr->getBtn("load_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("load_btn")->getSpritePos(), theButtonMgr->getBtn("load_btn")->getSpriteScale());
		theButtonMgr->getBtn("save_btn")->setSpritePos({ 740, 575 });
		theButtonMgr->getBtn("save_btn")->render(theRenderer, &theButtonMgr->getBtn("save_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("save_btn")->getSpritePos(), theButtonMgr->getBtn("save_btn")->getSpriteScale());

	}
	break;
	//SDL_RenderClear(theRenderer);

	case END:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 500, 500 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 500, 575 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
		/*
		// Render each asteroid in the vector array
		for (int draw = 0; draw < theAsteroids.size(); draw++)
		{
			theAsteroids[draw]->render(theRenderer, &theAsteroids[draw]->getSpriteDimensions(), &theAsteroids[draw]->getSpritePos(), theAsteroids[draw]->getSpriteRotAngle(), &theAsteroids[draw]->getSpriteCentre(), theAsteroids[draw]->getSpriteScale());
		}
		// Render each bullet in the vector array
		for (int draw = 0; draw < theBullets.size(); draw++)
		{
			theBullets[draw]->render(theRenderer, &theBullets[draw]->getSpriteDimensions(), &theBullets[draw]->getSpritePos(), theBullets[draw]->getSpriteRotAngle(), &theBullets[draw]->getSpriteCentre(), theBullets[draw]->getSpriteScale());
		}
		// Render the Title
		cTexture* tempTextTexture = theTextureMgr->getTexture("Title");
		SDL_Rect pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		// Render the score
		if (scoreChanged)
		{
			gameTextList[1] = scoreAsString.c_str();
			theTextureMgr->addTexture("Score", theFontMgr->getFont("Optimal")->createTextTexture(theRenderer, gameTextList[1], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
			scoreChanged = false;
		}

		tempTextTexture = theTextureMgr->getTexture("Score");
		pos = { 700, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		// render the rocket
		Player1.render(theRenderer, &Player1.getSpriteDimensions(), &Player1.getSpritePos(), Player1.getSpriteRotAngle(), &Player1.getSpriteCentre(), Player1.getSpriteScale());
		SDL_RenderPresent(theRenderer);
		*/
	}
	break;

	case QUIT:
	{
		loop = false;
	}
	break;
	default:
		break;
	}

	SDL_RenderPresent(theRenderer);
	
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{

	//SDL_RenderPresent(theRenderer);
}

void cGame::update()
{

}

void cGame::update(double deltaTime)

{
	switch (theGameState)
	{
	case MENU:
	{
		theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, PLAYING, theAreaClicked);
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);

		if (theGameState == PLAYING)
		{
			vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin();
			while (asteroidIterator != theAsteroids.end())
			{
				asteroidIterator = theAsteroids.erase(asteroidIterator);
			}
			for (int astro = 0; astro < 5; astro++)
			{
				//createAsteroid();
				theAsteroids.push_back(new cAsteroid);
				theAsteroids[astro]->setSpritePos({ 100 * (rand() % 5 + 1), 50 * (rand() % 5 + 1) });
				theAsteroids[astro]->setSpriteTranslation({ 3,3 });
				int randAsteroid = rand() % 4;
				theAsteroids[astro]->setTexture(theTextureMgr->getTexture(textureName[0]));
				theAsteroids[astro]->setSpriteDimensions(theTextureMgr->getTexture(textureName[0])->getTWidth(), theTextureMgr->getTexture(textureName[0])->getTHeight());
				theAsteroids[astro]->setAsteroidVelocity({ 1,1 });
				theAsteroids[astro]->setActive(true);
			}
		}
	}
	break;
	case PLAYING:
	{
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);

		int asteroidsDestroyed = 0;
		// Update the visibility and position of each asteriod
		/*if (play == true) // test
		{*/


		vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin();
		while (asteroidIterator != theAsteroids.end())
		{
			if ((*asteroidIterator)->isActive() == false)
			{
				asteroidIterator = theAsteroids.erase(asteroidIterator);
				cout << "Not active " << endl;
				++asteroidsDestroyed;

			}
			else
			{
				(*asteroidIterator)->update(deltaTime);
				++asteroidIterator;
			}
			/*}*/ //test
		}

		for (int alin = 0; alin < asteroidsDestroyed; ++alin)
		{
			cout << "Destroyed : " << asteroidsDestroyed << endl;
			//createAsteroid();
			++spawnAmount;

			for (int astro = 0; astro < spawnAmount; astro++)
			{
				if (spawnAmount >= 5)
				{
					spawnAmount--;
				}
				else
				{
					if (theAsteroids.size() <= 25)
					{
						createAsteroid();
					}
				}

			}

		}

		// Update the visibility and position of each bullet
		vector<cBullet*>::iterator bulletIterartor = theBullets.begin();
		while (bulletIterartor != theBullets.end())
		{
			if ((*bulletIterartor)->isActive() == false)
			{
				bulletIterartor = theBullets.erase(bulletIterartor);
			}
			else
			{
				(*bulletIterartor)->update(deltaTime);
				++bulletIterartor;
			}

		}

		if (theGameState == PLAYING && score >= 400)
		{
			theGameState = END;
		}


		/*
		==============================================================
		| Check for collisions
		==============================================================
		*/
		for (vector<cBullet*>::iterator bulletIterartor = theBullets.begin(); bulletIterartor != theBullets.end(); ++bulletIterartor)
		{
			//(*bulletIterartor)->update(deltaTime);
			for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
			{
				if ((*asteroidIterator)->collidedWith(&(*asteroidIterator)->getBoundingRect(), &(*bulletIterartor)->getBoundingRect()))
				{
					// if a collision set the bullet and asteroid to false

					score += 10;

					if (theTextureMgr->getTexture("Score") != NULL)
					{
						theTextureMgr->deleteTexture("Score");
					}
					string thescore = to_string(score);

					scoreAsString = "S+core: " + thescore;

					scoreChanged = true;

					(*asteroidIterator)->setActive(false);
					(*bulletIterartor)->setActive(false);
					theSoundMgr->getSnd("explosion")->play(0);

				}
			}
		}

		// Update the Rockets position
		Player1.update(deltaTime);
	}
	break;
	case END:
	{
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
		theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, MENU, theAreaClicked);

	}
	break;
	}
}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		switch (event.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
			{
				theAreaClicked = { event.motion.x, event.motion.y };
			}
				break;
			case SDL_BUTTON_RIGHT:
				break;
			default:
				break;
			}
			break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEMOTION:
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					theLoop = false;
					break;
					/*case SDLK_DOWN:
					{
						Player1.setPlayer1Velocity({ 0,200 });
					}
					break;

					case SDLK_UP:
					{
						Player1.setPlayer1Velocity({ 0,-200 });
					}
					break;*/
				case SDLK_RIGHT:
				{
					Player1.setPlayer1Velocity({ 200,0 });
				}
				break;

				case SDLK_LEFT:
				{
					Player1.setPlayer1Velocity({ -200,0 });
				}
				break;
				case SDLK_SPACE:
				{
					theBullets.push_back(new cBullet);
					int numBullets = theBullets.size() - 1;
					theBullets[numBullets]->setSpritePos({ Player1.getBoundingRect().x + Player1.getSpriteCentre().x, Player1.getBoundingRect().y + Player1.getSpriteCentre().y });
					theBullets[numBullets]->setSpriteTranslation({ 2, 2 });
					theBullets[numBullets]->setTexture(theTextureMgr->getTexture("bullet"));
					theBullets[numBullets]->setSpriteDimensions(theTextureMgr->getTexture("bullet")->getTWidth(), theTextureMgr->getTexture("bullet")->getTHeight());
					theBullets[numBullets]->setBulletVelocity({ 2, 2 });
					theBullets[numBullets]->setSpriteRotAngle(Player1.getSpriteRotAngle());
					theBullets[numBullets]->setActive(true);
					cout << "Bullet added to Vector at position - x: " << Player1.getBoundingRect().x << " y: " << Player1.getBoundingRect().y << endl;
				}
				theSoundMgr->getSnd("shot")->play(0);
				break;
				default:
					break;
				}
			
			default:
				break;
		}

		
	}
		return theLoop;
}



double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}


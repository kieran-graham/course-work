/*
=================
cRocket.h
- Header file for class definition - SPECIFICATION
- Header file for the Player1 class which is a child of cSprite class
=================
*/
#ifndef _CPlayer1_H
#define _CPlayer1_H
#include "cSprite.h"

class cPlayer1 : public cSprite
{
private:
	SDL_Point Player1Velocity;

public:
	cPlayer1();
	void update(double deltaTime);		// Player1 update method
	void setPlayer1Velocity(SDL_Point rocketVel);   // Sets the velocity for the Player1
	SDL_Point getPlayer1Velocity();				 // Gets Player1 velocity
};
#endif
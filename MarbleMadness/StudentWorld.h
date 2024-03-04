#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	~StudentWorld();
	  // given current level of the GameWorld, loads the maze if possible. populates ACTORS and AVATOR of the game for the current level
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	  /* check if the provided Actor can move to the provided position
	     if Actor is marble, only allows movement to Pit or Empty
	     if Actor is agent, movement allowed anywhere but wall, pit, robot, or robotfactory */
	bool canActorMoveHere(Actor* a, int x, int y) const;
	  // check if a marble could move here - only if Empty or Pit
	bool allowsMarble(int x, int y) const;
	  // assuming not avator, adds actor to array
	void addActor(Actor* a);
	  // pea will call this to try to damage any objects at its current location
	bool tryToDamageLocation(Actor* a, int peaX, int peaY);
	  // if marble is on the same square as a pit, returns a pointer to it (otherwise, nullptr)
	Actor* isMarbleHere(Actor* a, int pitX, int pitY);
	  // if avator is on the same square as the provided position, returns true. (otherwise, false)
	bool isPlayerHere(int x, int y);

private:
	  // update score/lives/level text at screen time
	void updateDisplayText();

	std::vector<Actor*> actors; // empty actors vector, 0 elems
	Avator* player; // pointer to the player object
	int bonus;
};

#endif // STUDENTWORLD_H_

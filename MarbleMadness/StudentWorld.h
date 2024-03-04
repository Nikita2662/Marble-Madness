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
	void completeLevel();
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
	Actor* isMarbleHere(Actor* a, int pitX, int pitY) const;
	  // if avator is on the same square as the provided position, returns true. (otherwise, false)
	bool isPlayerHere(int x, int y) const;
	Actor* getPlayer();
	bool allCrystalsCollected() const;
	void restorePlayerToFullHealth();
	void addAmmoToPlayer(int amt);
	  /* If a pea were at x, y moving in direction dx, dy, could it hit the
         player without encountering any obstructions? */
    bool existsClearShotToPlayer(int x, int y, int dx, int dy) const;
	  // returns if bot is allowed to move to this position
	bool allowsBot(int x, int y) const;
	  // returns pointer to goodie if ThiefBot is on the same square as a goodie
	Actor* isGoodieHere(Actor* a, int thiefX, int thiefY) const;


private:
	  // update score/lives/level text at screen time
	void updateDisplayText();

	std::vector<Actor*> actors; // empty actors vector, 0 elems
	Avator* player; // pointer to the player object
	int bonus;
	bool levelCompleted;
};

#endif // STUDENTWORLD_H_

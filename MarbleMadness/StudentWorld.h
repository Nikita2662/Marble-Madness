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
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  bool checkIfCanMoveHere(int x, int y) const;

private:
	void updateDisplayText();

	std::vector<Actor*> actors; // empty actors vector, 0 elems
	Avator* player; // pointer to the player object
	int bonus;
};

#endif // STUDENTWORLD_H_

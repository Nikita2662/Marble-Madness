#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(StudentWorld* ptr, int ID, int x, int y, int startDir=none);
	virtual ~Actor();
	virtual void doSomething() = 0; // contains nothing rn
	StudentWorld* getWorld();
	virtual bool canMoveHere() = 0;
private:
	StudentWorld* myWorld;
};

class Wall : public Actor
{
public:
	Wall(int x, int y, StudentWorld* ptr); // contains nothing rn
	virtual void doSomething();
	virtual bool canMoveHere();
private:
};

class Avator : public Actor
{
public:
	Avator(int x, int y, StudentWorld* ptr); // does nothing rn
	int getHealth();
	int getAmmo();
	virtual void doSomething();
	virtual bool canMoveHere();
private:
	int hitPoints;
	int numPeas;
};

#endif // ACTOR_H_

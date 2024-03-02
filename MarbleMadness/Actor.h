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
	StudentWorld* getWorld() const;
	virtual bool canMoveHere() const = 0;
	bool isAlive() const;
	void setDead();
private:
	StudentWorld* myWorld;
	bool alive;
};

class Wall : public Actor
{
public:
	Wall(int x, int y, StudentWorld* ptr); // contains nothing rn
	virtual void doSomething();
	virtual bool canMoveHere() const;
private:
};

class Avator : public Actor
{
public:
	Avator(int x, int y, StudentWorld* ptr); // does nothing rn
	int getHealth() const;
	int getAmmo() const;
	virtual void doSomething();
	virtual bool canMoveHere() const;
private:
	int hitPoints;
	int numPeas;
};

#endif // ACTOR_H_

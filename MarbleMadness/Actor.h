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
	virtual bool allowsColocationBy(Actor* a) const = 0;
	  // returns if Actor can be damaged by pea
	virtual bool isDamageable() const = 0;
	  // when attacked by pea, suffer damage
	virtual void damageBy(int damageAmt) = 0;
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
	virtual bool allowsColocationBy(Actor* a) const;
		// returns if Wall can be damaged by pea
	virtual bool isDamageable() const;
		// when attacked by pea, suffer damage
	virtual void damageBy(int damageAmt); private:
};

class Avator : public Actor
{
public:
	Avator(int x, int y, StudentWorld* ptr); // does nothing rn
	int getHealth() const;
	int getAmmo() const;
	virtual void doSomething();
	virtual bool allowsColocationBy(Actor* a) const;
	  // returns if Avator can be damaged by pea
	virtual bool isDamageable() const;
	  // when attacked by pea, suffer damage
	virtual void damageBy(int damageAmt);
      // move to the adjacent square in the direction avator currently faces, if not blocked
	bool moveIfPossible();
private:
	int hitPoints;
	int numPeas;
};

class Pea : public Actor
{
public:
	Pea(int x, int y, StudentWorld* ptr, int dir);
	virtual void doSomething(); // empty rn
	virtual bool allowsColocationBy(Actor* a) const; // default rn
	  // returns if Actor can be damaged by pea
	virtual bool isDamageable() const;
	  // when attacked by pea, suffer damage
	virtual void damageBy(int damageAmt);
private:
};

#endif // ACTOR_H_

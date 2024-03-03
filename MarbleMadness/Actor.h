#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(StudentWorld* ptr, int ID, int x, int y, int startDir=none);
	bool isAlive() const;
	void setDead();
	void setHP(int amt);
	int getHP() const;
	StudentWorld* getWorld() const;
	virtual ~Actor();
	virtual void doSomething() = 0; // contains nothing rn
	  // only returns true for Pit or Empty
	virtual bool canContainMarblePush();
	  // returns if this kind of Actor could possibly be pushed by an Avator (only true for marble)
	virtual bool mayBePushedByPlayer();
	  // avator pushes Marble to given position if possible, returns false otherwise
	virtual bool pushTo(int x, int y);
	virtual bool allowsAgentColocationBy(Actor* a) const = 0;
	  // returns if Actor can be damaged by pea
	virtual bool isDamageable() const = 0;
	  // when attacked by pea, suffer damage
	virtual void damageBy(int damageAmt) = 0;
private:
	StudentWorld* myWorld;
	bool alive;
	int hitPoints;
};

class Wall : public Actor
{
public:
	Wall(int x, int y, StudentWorld* ptr); // contains nothing rn
	virtual void doSomething();
	virtual bool allowsAgentColocationBy(Actor* a) const;
		// returns if Wall can be damaged by pea
	virtual bool isDamageable() const;
		// when attacked by pea, suffer damage
	virtual void damageBy(int damageAmt); 
private:
};

class Avator : public Actor
{
public:
	Avator(int x, int y, StudentWorld* ptr); // does nothing rn
	int getHealth() const;
	int getAmmo() const;
	bool moveIfPossible();
	virtual void doSomething();
	virtual bool allowsAgentColocationBy(Actor* a) const;
	  // returns if Avator can be damaged by pea
	virtual bool isDamageable() const;
	  // when attacked by pea, suffer damage
	virtual void damageBy(int damageAmt);
      // move to the adjacent square in the direction avator currently faces, if not blocked
private:
	int numPeas;
};

class Pea : public Actor
{
public:
	Pea(int x, int y, StudentWorld* ptr, int dir);
	virtual void doSomething(); // empty rn
	virtual bool allowsAgentColocationBy(Actor* a) const; // default rn
	  // returns if Actor can be damaged by pea
	virtual bool isDamageable() const;
	  // when attacked by pea, suffer damage
	virtual void damageBy(int damageAmt);
private:
};

class Marble : public Actor
{
public:
	Marble(int x, int y, StudentWorld* ptr);
	virtual void doSomething(); // contains nothing rn
	  // avator may push a marble object (depending on position)
	virtual bool mayBePushedByPlayer();
	virtual bool allowsAgentColocationBy(Actor* a) const;
	  // returns if Marble can be damaged by pea
	virtual bool isDamageable() const;
	  // when attacked by pea, suffer damage
	virtual void damageBy(int damageAmt);
	  // avator pushes Marble to given position if possible, returns false otherwise
	bool pushTo(int x, int y);
private:
};

#endif // ACTOR_H_

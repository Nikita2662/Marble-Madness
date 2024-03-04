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
	virtual bool isCrystal() const;
	virtual bool allowsAgentColocationBy(Actor* a) const = 0;
	  // returns if Actor can be hit by pea
	virtual bool isHittable() const = 0;
	  // returns true if actor can be stolen by ThiefBot
	virtual bool isStealable() const;
	  // when attacked by pea, suffer damage
	virtual void damageBy(int damageAmt) = 0;
	virtual void changeStealable();
	virtual bool countsInFactoryCensus();
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
		// returns if Wall can be hit by pea
	virtual bool isHittable() const;
		// when attacked by pea, suffer damage
	virtual void damageBy(int damageAmt); 
private:
};

class Avator : public Actor
{
public:
	Avator(int x, int y, StudentWorld* ptr);
	int getHealth() const;
	void incAmmo(int amt);
	int getAmmo() const;
	bool moveIfPossible();
	virtual void doSomething();
	virtual bool allowsAgentColocationBy(Actor* a) const;
	  // returns if Avator can be hit by pea
	virtual bool isHittable() const;
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
	  // returns if Actor can be hit by pea - Marble, Robot, Player, Wall, RobotFactory return true
	virtual bool isHittable() const;
	  // when attacked by pea, suffer damage
	virtual void damageBy(int damageAmt);
private:
};

class Marble : public Actor
{
public:
	Marble(int x, int y, StudentWorld* ptr);
	virtual void doSomething();
	  // avator may push a marble object (depending on position)
	virtual bool mayBePushedByPlayer();
	virtual bool allowsAgentColocationBy(Actor* a) const;
	  // returns if Marble can be hit by pea
	virtual bool isHittable() const;
	  // when attacked by pea, suffer damage
	virtual void damageBy(int damageAmt);
	  // avator pushes Marble to given position if possible, returns false otherwise
	bool pushTo(int x, int y);
private:
};

class Pit : public Actor
{
public:
	Pit(int x, int y, StudentWorld* ptr);
	virtual void doSomething();
	  // only returns true for Pit or Empty
	virtual bool canContainMarblePush();
	virtual bool allowsAgentColocationBy(Actor* a) const;
	  // returns if Actor can be hit by pea
	virtual bool isHittable() const;
	  // when attacked by pea, suffer damage
	virtual void damageBy(int damageAmt);
private:
};

class Exit : public Actor
{
public:
	Exit(int x, int y, StudentWorld* ptr);
	virtual void doSomething();
	virtual bool allowsAgentColocationBy(Actor* a) const;
	  // returns if Actor can be hit by pea
	virtual bool isHittable() const;
	  // when attacked by pea, suffer damage
	virtual void damageBy(int damageAmt);

private:
};

class PickupableItem : public Actor
{
public:
	PickupableItem(int x, int y, int ID, StudentWorld* ptr);
	virtual void changeStealable();
	virtual void specialized() = 0;
	virtual void doSomething();
	  // returns true if actor can be stolen by ThiefBot (only Goodies)
	virtual bool isStealable() const;
	virtual bool allowsAgentColocationBy(Actor* a) const;
	  // returns if Actor can be hit by pea
	virtual bool isHittable() const;
	  // when attacked by pea, suffer damage
	virtual void damageBy(int damageAmt);
private:
	bool canPickUp;
};

class Crystal : public PickupableItem
{
public:
	Crystal(int x, int y, StudentWorld* ptr);
	virtual void specialized();
	  // only true for Crystal
	virtual bool isCrystal() const;
};

class ExtraLifeGoodie : public PickupableItem
{
public:
	ExtraLifeGoodie(int x, int y, StudentWorld* ptr);
	virtual void specialized();
};

class RestoreHealthGoodie : public PickupableItem
{
public:
	RestoreHealthGoodie(int x, int y, StudentWorld* ptr);
	virtual void specialized();
};

class AmmoGoodie : public PickupableItem
{
public:
	AmmoGoodie(int x, int y, StudentWorld* ptr);
	virtual void specialized();
};

class Robot : public Actor
{
public:
	Robot(int x, int y, int ID, int dir, StudentWorld* ptr);
	void determineNumTicks();
	int getTickCount();
	int getNumTicks();
	virtual void act() = 0;
	virtual void doSomething();
	virtual bool allowsAgentColocationBy(Actor* a) const;
	  // returns if Actor can be hit by pea
	virtual bool isHittable() const;
	  // when attacked by pea, suffer damage
	virtual void damageBy(int damageAmt);
	virtual void damageSpecial() = 0;
private:
	int tickCount;
	int numTicks;
};

class RageBot : public Robot
{
public:
	RageBot(int x, int y, int dir, StudentWorld* ptr);
	virtual void act();
	virtual void damageSpecial();
};

class ThiefBot : public Robot
{
public:
	ThiefBot(int x, int y, int ID, StudentWorld* ptr);
	virtual void act();
	virtual void damageSpecial();
	virtual bool countsInFactoryCensus();
private:
	int distanceBeforeTurning;
	bool holding;
	Actor* p;
	int distMoved;
};

class MeanThiefBot : public ThiefBot
{
public:
	MeanThiefBot(int x, int y, StudentWorld* ptr);
	virtual void act();
	virtual void damageSpecial();
};

class ThiefBotFactory : public Actor
{
public:
	ThiefBotFactory(int x, int y, StudentWorld* ptr, int botID);
	virtual void doSomething();
	virtual bool allowsAgentColocationBy(Actor* a) const;
	  // returns if TBF can be hit by pea
	virtual bool isHittable() const;
	  // when attacked by pea, suffer damage
	virtual void damageBy(int damageAmt);
private:
	int botID;
};
#endif // ACTOR_H_

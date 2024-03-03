#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include "Actor.h"
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath) { return new StudentWorld(assetPath); }

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath), bonus(-1), player(nullptr)
{}

StudentWorld::~StudentWorld() { cleanUp(); }

  // assuming not avator, adds actor to array
void StudentWorld::addActor(Actor* a) { actors.push_back(a);  }

  // check if the provided Actor can move to the provided position
bool StudentWorld::canAgentMoveHere(int x, int y, Actor* a) const
{
    for (size_t i = 0; i < actors.size(); i++) // iterate through actors by index
    {
        if (actors[i]->getX() == x && actors[i]->getY() == y)
            return actors[i]->allowsAgentColocationBy(a);
    }
    return true; // assuming valid index, this just means it's empty
}

bool StudentWorld::canMarbleMoveHere(int x, int y) const
{
    for (size_t i = 0; i < actors.size(); i++) // iterate through actors by index
    {
        if (actors[i]->getX() == x && actors[i]->getY() == y) // if there is an actor at this position
            return actors[i]->canBePushedByMarble(); // will only return true for Pit
    }
    return true; // empty at this position
}

  // update score/lives/level text at screen time
void StudentWorld::updateDisplayText()
{
    ostringstream oss;
    oss.fill('0'); // leading 0s for score
    oss << "Score: " << setw(7) << getScore() << "  ";
    oss << "Level: " << setw(2) << getLevel() << "  ";
    oss.fill(' ');
    oss << "Lives: " << setw(2) << getLives() << "  ";
    oss << "Health: " << setw(3) << player->getHealth() << "%  ";
    oss << "Ammo: " << setw(3) << player->getAmmo() << "  ";
    oss << "Bonus: " << setw(4) << bonus << "  ";

    string s = oss.str();

    setGameStatText(s); // update the display text with this string
}

  // given current level of the GameWorld, loads the maze if possible. populates ACTORS and AVATOR of the game for the current level
int StudentWorld::init()
{
    bonus = 1000; // whenever new level starts

    // ------ convert level to a string so we can access text file
    int l = getLevel();
    ostringstream oss;
    oss.fill('0');
    oss << "level" << setw(2) << l << ".txt";
    string level = oss.str();

    // ------ load result 
    Level lev(assetPath());
    Level::LoadResult result = lev.loadLevel(level);

    // ------ check if level is a valid level + file is found etc. --> appropriate return for init
    if (result == Level::load_fail_file_not_found || l == 100)
        return GWSTATUS_PLAYER_WON;
    else if (result == Level::load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR;

    Level::MazeEntry curr;
    // ------ load was successful, access contents of the level to populate the game world - ACTORS - accordingly
    for (int i = 0; i < VIEW_HEIGHT; i++) // iterate through rows
        for (int j = 0; j < VIEW_WIDTH; j++) // iterate through cols
        {
            curr = lev.getContentsOf(j, i); // curr stores the entry

            // allocate Actor object based on what entry is, add to vector
            switch (curr)
            {
            case Level::empty:
                break; // move to next pos, nothing to allocate
            case Level::wall:
            {
                Wall* w = new Wall(i, j, this);
                addActor(w);
                break;
            }
            case Level::player:
            {
                player = new Avator(i, j, this);
                addActor(player);
                break;
            }
            case Level::marble:
            {
                Marble* w = new Marble(i, j, this);
                addActor(w);
                break;
            }
            case Level::exit:
            case Level::horiz_ragebot:
            case Level::vert_ragebot:
            case Level::thiefbot_factory:
            case Level::mean_thiefbot_factory:
            case Level::ammo:
            case Level::pit:
            case Level::crystal:
            case Level::restore_health:
            case Level::extra_life:
                break;
            default:
                cerr << "Invalid entry in maze text file";
            }
        }

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    updateDisplayText(); // update game's status line

    for (size_t i = 0; i < actors.size(); i++) // iterate through actors by index i
    {
        if (actors[i]->isAlive())
        {
            actors[i]->doSomething();
            
            if (!player->isAlive()) // if player died during this tick
                return GWSTATUS_PLAYER_DIED;

            // ADD HERE
        }  
    }
    // remove newly-dead actors after each tick
    for (vector<Actor*>::iterator p = actors.begin(); p != actors.end(); )
    {
        if (!(*p)->isAlive()) // if dead (p is iterator to an Actor pointer)
        {
            delete* p; // delete the Actor that the pointer points to
            p = actors.erase(p); // normal erase process since no more objects need to be destructed
        }
        else
            p++; // skip this object
    }

    // reduce current bonus for level by 1 unless already 0
    if (bonus > 0) bonus--; 
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (vector<Actor*>::iterator p = actors.begin(); p != actors.end(); ) {
       delete* p; // delete the Actor that the pointer points to
       p = actors.erase(p); // normal erase process since no more objects need to be destructed
    }
}

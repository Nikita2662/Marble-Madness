#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include "Actor.h"
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath), bonus(-1), player(nullptr)
{
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

bool StudentWorld::checkIfCanMoveHere(int x, int y) const
{
    for (size_t i = 0; i < actors.size(); i++) // iterate through actors by index
    {
        if (actors[i]->getX() == x && actors[i]->getY() == y)
            return actors[i]->canMoveHere();
    }
    return true; // assuming valid index, this just means it's empty
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
    // ------ load was successful, access contents of the level to populate the game world - ACTORS and AVATOR - accordingly
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
                actors.push_back(w);
                break;
            }
            case Level::player:
            {
                player = new Avator(i, j, this);
                break;
            }
            case Level::exit:
            case Level::horiz_ragebot:
            case Level::vert_ragebot:
            case Level::thiefbot_factory:
            case Level::mean_thiefbot_factory:
            case Level::ammo:
            case Level::marble:
            case Level::pit:
            case Level::crystal:
            case Level::restore_health:
            case Level::extra_life:
                break;
            /* case Level::exit:
            {
                actors.push_back(new .......)
                break;
            }
            ........
            */
            default:
                cerr << "Invalid entry in maze text file";
            }
        }

    return GWSTATUS_CONTINUE_GAME;
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

int StudentWorld::move()
{
    updateDisplayText(); // update game's status line

    for (size_t i = 0; i < actors.size(); i++) // iterate through actors by index
    {
        if (actors[i]->isAlive())
        {
            actors[i]->doSomething();
            player->doSomething();
            
            if (!player->isAlive())
        }  
    }

    if (bonus > 0) bonus--; // decrements by 1 unless already 0
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (size_t i = 0; i < actors.size(); i++) // iterate through actors by index
    {
        delete actors[i]; // destroy each active Actor we have stored
    }

    delete player;
}

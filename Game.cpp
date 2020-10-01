#include"globals.h"
#include "Game.h"
#include "utilities.h"
#include "GameObjects.h"
#include"Dungeon.h"
#include"Actors.h"
#include <iostream>
using namespace std;


char helpQuit() /// Helps get the 'q' character.
{
    char c;
    while ((c = getCharacter()) != 'q')
    {
        c = getCharacter();
    }
    return c;
}

/// MEMBER FUNCTION IMPLEMENTATIONS /////////////////////////////////////////////////////////////////////////////////

void Game::play(int dist) 
{
    Player* p = new Player();
    goblinSmellDist = dist;
    do
    {   
        levelChange = false;
        Dungeon* d = new Dungeon(goblinSmellDist, getLevel());
        d->makeDungeon();
        d->insertObjects();
        d->insertPlayer(p);
        d->insertMonsters();
        d->displayDungeon();
        char c = getCharacter();
        bool continuing = true;
        while (continuing)
        {
            switch (c)
            {
            case 'k':
                ///////////////////////////
                p->PlayerMove(NORTH);
                break;
            case 'j':
                p->PlayerMove(SOUTH);
                break;
            case 'h':
                p->PlayerMove(WEST);
                break;
            case 'l':
                p->PlayerMove(EAST);
                break;
            case 'g':
                p->pick();
                break;
            case 'w':
                p->wield();
                break;
            case 'r':
                p->read();
                break;
            case '>':
                GameObjects * ob;
                if (d->isOnObject(p->getPosition(), ob))
                {
                    if (ob->getSymbol() == '>')
                    {
                        continuing = false;
                        levelChange = true;
                        m_level++;
                        delete d;        //d->clearAll(); //Called in destrutor of dungeon
                    }
                }
                break;
            case 'c':
                p->cheat();
                break;
            case 'i':
                p->displayInventory();
                getCharacter();
                break;
            case 'q':
                continuing = false;
                continue;
            default:
                break;
            }
            if (levelChange == false)
            {
                if (p->won())
                {
                    d->updateActionMsg("Congratulations, you won!");
                    d->displayDungeon();
                    cout << "Press q to exit game." << endl;
                    c = helpQuit();
                    continuing = false;
                    continue;
                }
                p->increaseHitPoint();
                d->reduceSleepTimer(); 
                d->monsterMove();
                if (p->lost())
                {
                    d->displayDungeon();
                    cout << "Press q to exit game." << endl;
                    c = helpQuit();
                    continuing = false;
                    continue;
                }
                d->displayDungeon();
                c = getCharacter();
            }
        }
        if (levelChange == false)
        {
            delete p;
            delete d;
        }
    } while (levelChange == true);
            
}


/// END OF CODE /////////////////////////////////////////////////////////////////////////////////////////////////////
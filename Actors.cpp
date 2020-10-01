#include "Actors.h"
#include "Dungeon.h"
#include"globals.h"
#include <cmath>
#include<algorithm>
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ACTORS IMPLMENTATIONS  //////////////////////////////////////////////////////////////////////////////////////////////////

bool Actors::canMove(int r, int c)
{

	if (dungeon()->isMonsterat(r, c))
		return false;
	if (dungeon()->isWallat(r, c))
		return false;
	return true;
}

bool Actors::IfcanMove(int r, int c) ///FOR GOBLIN
{
	
	if (dungeon()->isMonsterat(r, c))
		return false;
	if (dungeon()->isWallat(r, c))
		return false;
	if (dungeon()->isPlayerat(r, c))
		return false;
	setPosition(Coord(r, c));
	return true;
}

void Actors::Attack(Actors* other)
{
	if (HitPoint() <= 0)
	{
		return;
	}

	if (randInt(dexterityPoints() + weapon()->DexBonus()) >= randInt(dexterityPoints() + armorPoints()))
	{
		other->setHitPoints(other->HitPoint() - damage());
		if (other->HitPoint() <= 0)
		{
			dungeon()->updateActionMsg(getName() + " " + weapon()->Action() + " " + weapon()->Name() + " at the " + other->getName() + " dealing a final blow.");
			other->Die();
			return;
		}

		if (weapon()->Name() == "magic fangs of sleep" && trueWithProbability(0.2)) /////////////////////
		{
			dungeon()->updateActionMsg(getName() + " " + weapon()->Action() + " " + weapon()->Name() + " at the " + other->getName() + " and hits, putting " + other->getName() + " to sleep.");
			int sleep = randInt(2, 6);
			other->m_sleepTime = (sleep > other->m_sleepTime) ? sleep : other->m_sleepTime;
		}
		else
		{
			dungeon()->updateActionMsg(getName() + " " + weapon()->Action() + " " + weapon()->Name() + " at the " + other->getName() + " and hits.");
		}
	}
	else
	{
		dungeon()->updateActionMsg(getName() + " " + weapon()->Action() + " " + weapon()->Name() + " at the " + other->getName() + " and misses.");
	}
}

int Actors::damage()
{
	return randInt(strengthPoints() + weapon()->DmgAmt());
}


void Actors::Die()
{
	if (this->getName() != "Player")
	{
		Monsters* m = dynamic_cast<Monsters*>(this);
		if (m != nullptr)
		{
			dungeon()->removeDeadandDropItem(m);
			delete m;
		}
	}
	else
	{
		Player* pp = dynamic_cast<Player*>(this);
		if (pp != nullptr)
		{
			pp->setLose(true);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// PLAYER IMPLMENTATIONS  //////////////////////////////////////////////////////////////////////////////////////////////////

void Player::read()
{
	if (sleepTime() == 0)
	{
		displayInventory();
		char c = getCharacter();
		if (isalpha(tolower(c)))
		{
			size_t index = c - 'a';
			if (index < inventory.size())
			{
				if (inventory[index]->getSymbol() == '?')
				{
					Scroll* s = (Scroll*)inventory[index];
					inventory.erase(inventory.begin() + index);
					s->effectOnPlayer(this);
					dungeon()->updateActionMsg("You read the scroll called " + s->Name());
					dungeon()->updateActionMsg(s->Action());
					delete s;
				}
				else
				{
					dungeon()->updateActionMsg("You can't read a " + inventory[index]->Name());
				}
			}
		}
	}
	return;
}


void Player::cheat()
{
	if (sleepTime() == 0)
	{
		setHitPoints(getMaxHitPoints());
		setStrengthpoints(9);
	}
	return;
}

void Player::pick()
{
	if (sleepTime() == 0)
	{
		GameObjects* o;
		if (dungeon()->isOnObject(getPosition(), o))
		{
			if (inventory.size() >= 26)
			{
				dungeon()->updateActionMsg("Your knapsack is full; you can't pick that up.");
				return;
			}
			if (o->getSymbol() == '>')
			{
				return;
			}
			inventory.push_back(o);
			dungeon()->removeObj(getPosition());
			dungeon()->updateActionMsg("You pick up " + o->Name());
			if (o->getSymbol() == '&')
			{
				setWin(true);
			}
		}
	}
	return;
}

void Player::wield()
{
	if (sleepTime() == 0)
	{
		displayInventory();
		char c = getCharacter();
		if (isalpha(tolower(c)))
		{
			size_t index = c - 'a';
			if (index < inventory.size())
			{
				if (inventory[index]->getSymbol() == ')')
				{
					setWeapon((Weapon*)inventory[index]);
					dungeon()->updateActionMsg("You are wielding " + inventory[index]->Name());
				}
				else
				{
					dungeon()->updateActionMsg("You can't wield " + inventory[index]->Name());
				}
			}
		}
	}
}

void Player::PlayerMove(int Direction)
{
	int r, c;
	if (Direction == NORTH)
	{
		r = getRow() - 1;
		c = getCol();
	}
	 else if (Direction == SOUTH)
	{
		 r = getRow() + 1;
		 c = getCol();
	}
	else if (Direction == EAST)
	{
		 r = getRow();
		 c = getCol() + 1 ;
	}
	else if (Direction == WEST)
	{
		 r = getRow();
	     c = getCol() - 1;
	}
	else
	{
		r = getRow();
		c = getCol();

	}
	if (IfcanMove(r, c) == false && sleepTime() == 0 )
	{
		if (dungeon()->isMonsterat(r, c))
		{
			Attack(dungeon()->whichMonster(r, c));
		}
		else if (dungeon()->isWallat(r, c))
		{
			return;
		}
		else
			return; 
	}

}

void Player::displayInventory()
{
	if (sleepTime() == 0)
	{
		clearScreen();
		cout << "Inventory:" << endl;
		for (size_t i = 0; i < inventory.size(); i++)
		{
			cout << " " << (char)(i + 'a') << ". " << inventory[i]->Name() << endl;
		}
	}
}

Player::~Player()
{
	while (!inventory.empty())
	{
		GameObjects* toDelete = inventory.back();
		inventory.pop_back();
		delete toDelete;
	}
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MONSTERS IMPLMENTATIONS  ////////////////////////////////////////////////////////////////////////////////////////////////
 
void Monsters::moveMonster(Player* p, int steps)
{
	int r = getRow();
	int c = getCol();
	if (steps == 1)
	{
		Attack(p);
		return;
	}
	int rowDiff = abs(r - p->getRow());
	int colDiff = abs(c - p->getCol());

	if (colDiff >= rowDiff)
	{
		if (c > p->getCol())
		{
			if (IfcanMove(r, c - 1))
				return;
		}
		else
		{
			if (IfcanMove(r, c + 1))
				return;
		}
	}
	if (r > p->getRow())
	{
		if (IfcanMove(r - 1, c))
			return;
	}
	else if (IfcanMove(r + 1, c))
		return;
	if (c > p->getCol())
	{
		if (IfcanMove(r, c - 1))
			return;
	}
	else
	{
		if (IfcanMove(r, c + 1))
			return;
	}

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// GOBLINS IMPLMENTATIONS  /////////////////////////////////////////////////////////////////////////////////////////////////

void Goblins::clearqueue()
{
	while (!cQueue.empty())
	{
		cQueue.pop();
	}
}


void Goblins::moveMonster(Player* p, int steps)
{
	
	if (sleepTime() != 0)
	{
		return;
	}	
	if (steps == 1)
	{
		Attack(p);
		return;
	}
	
	int r = getRow();
	int c = getCol();
	Coord playerCoord = dungeon()->getPlayer()->getPosition();

	int minDistUP = smellDist() + 1;
	int minDistDOWN = smellDist() + 1;
	int minDistLEFT = smellDist() + 1;
	int minDistRIGHT = smellDist() + 1;


	if (canMove(r - 1, c))
	{
		clearqueue();
		cQueue.push(placeInfo(Coord(r-1, c), 0));
		dungeon()->setGrid(r-1, c, '!');
		minDistUP =  shortestPathExists(playerCoord, smellDist() - 1);
		dungeon()->removeMarker();

		//cerr << minDistUP << endl;
	}
	
	if (canMove(r + 1, c))
	{
		clearqueue();
		cQueue.push(placeInfo(Coord(r + 1, c), 0));
		dungeon()->setGrid(r + 1, c, '!');
		minDistDOWN = shortestPathExists(playerCoord, smellDist() - 1);
		dungeon()->removeMarker();

		//cerr << minDistDOWN << endl;
	}

	if (canMove(r , c - 1))
	{
		clearqueue();
		cQueue.push(placeInfo(Coord(r, c-1), 0));
		dungeon()->setGrid(r , c - 1, '!');
		minDistLEFT = shortestPathExists(playerCoord, smellDist() - 1);
		dungeon()->removeMarker();

		//cerr<< minDistLEFT << endl;
	}

	if (canMove(r , c + 1))
	{
		clearqueue();
		cQueue.push(placeInfo(Coord(r , c+1), 0));
		dungeon()->setGrid(r , c+1, '!');
		minDistRIGHT = shortestPathExists(playerCoord, smellDist() - 1);
		dungeon()->removeMarker();

		//cerr << minDistRIGHT << endl;
	}

	if (minDistDOWN == -1) { minDistDOWN = smellDist() + 1; }
	if (minDistLEFT == -1) { minDistLEFT = smellDist() + 1; }
	if (minDistUP == -1) { minDistUP = smellDist() + 1; }
	if (minDistRIGHT == -1) { minDistRIGHT = smellDist() + 1; }

	if (minDistUP == min(min((minDistDOWN), (minDistUP)), min((minDistLEFT), (minDistRIGHT))) && minDistUP <= smellDist())
	{
		setPosition(Coord(r - 1, c));
		return;
	}
	if (minDistDOWN == min(min((minDistDOWN), (minDistUP)), min((minDistLEFT), (minDistRIGHT)))  && minDistDOWN <= smellDist())
	{
		setPosition(Coord(r + 1, c));
		return;
	}
	if (minDistLEFT == min(min((minDistDOWN), (minDistUP)), min((minDistLEFT), (minDistRIGHT))) && minDistLEFT <= smellDist())
	{
		setPosition(Coord(r, c - 1));
		return;
	}
	if (minDistRIGHT == min(min((minDistDOWN), (minDistUP)), min((minDistLEFT), (minDistRIGHT))) && minDistRIGHT <= smellDist())
	{
		setPosition(Coord(r, c + 1));
		return;
	}	
	return;
}

int Goblins::shortestPathExists( Coord playerPos , int maxdist) //// Recursive Functions
{
	if (cQueue.empty())
	{
		return -1;
	}
	placeInfo current = cQueue.front();
	cQueue.pop();
	
	if (current.dist == maxdist  && (playerPos.m_r != current.pos.m_r || playerPos.m_c != current.pos.m_c))
	{
		return -1;
	}
	if (playerPos == current.pos)
	{
		return current.dist;
	}

	if (canMove(current.pos.m_r - 1, current.pos.m_c) && dungeon()->getGrid(current.pos.m_r - 1, current.pos.m_c) == ' ')
	{
		dungeon()->setGrid(current.pos.m_r - 1, current.pos.m_c, '!');
		cQueue.push(placeInfo(Coord(current.pos.m_r - 1, current.pos.m_c), current.dist + 1));
	}

	if (canMove(current.pos.m_r + 1, current.pos.m_c) && dungeon()->getGrid(current.pos.m_r + 1, current.pos.m_c) == ' ')
	{
		dungeon()->setGrid(current.pos.m_r + 1, current.pos.m_c, '!');
		cQueue.push(placeInfo(Coord(current.pos.m_r + 1, current.pos.m_c), current.dist + 1));
	}
	
	if (canMove(current.pos.m_r , current.pos.m_c + 1) && dungeon()->getGrid(current.pos.m_r , current.pos.m_c + 1) == ' ')
	{
		dungeon()->setGrid(current.pos.m_r , current.pos.m_c + 1, '!');
		cQueue.push(placeInfo(Coord(current.pos.m_r , current.pos.m_c + 1), current.dist + 1));
	}

	if (canMove(current.pos.m_r, current.pos.m_c - 1) && dungeon()->getGrid(current.pos.m_r, current.pos.m_c - 1) == ' ')
	{
		dungeon()->setGrid(current.pos.m_r, current.pos.m_c - 1, '!');
		cQueue.push(placeInfo(Coord(current.pos.m_r, current.pos.m_c - 1), current.dist + 1));
	}
	return shortestPathExists(playerPos, maxdist);			//Recursive call
}

//// END OF CODE//////////////////////////////////////////////////////////////////////////////////////////////////////
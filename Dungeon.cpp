#include<iostream>
#include <string>
#include <vector>
#include<algorithm>
#include"Dungeon.h"
#include"geometry.h"
#include"utilities.h"
#include"Actors.h" 

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// DUNGEON IMPLEMENTATION ///////////////////////////////////////////////////////////////////////////////////////////////////////

///// MAKING THE DUNGEON SKELETON ///////////////////////////////////////////////////////////////////////////////////////////////////

void Dungeon::makeDungeon()
{
	for (int r = 0; r < MAXROWS; r++)
		for (int c = 0; c < MAXCOLS; c++)
			m_grid[r][c] = '#';
	vRooms.clear();
	makeRooms();
	addRooms();
	connectRooms();
}

void Dungeon::addRooms()
{
	size_t totalRooms = vRooms.size();
	for (size_t i = 0; i < totalRooms; i++)
	{
		int row = vRooms[i].getTopLeft().m_r;
		int col = vRooms[i].getTopLeft().m_c;
		int width = vRooms[i].getWidth();
		int height = vRooms[i].getHeight();
		for (int r = row; r < row + height && r < MAXROWS; r++)
		{
			for (int c = col; c < col + width && c < MAXCOLS; c++)
			{
				m_grid[r][c] = ' ';
			}
		}
	}
}

void Dungeon::makeRooms()
{
	int sectionSkipper = 0; int doubleSection = 0;
	for (int partitionNo = 0; partitionNo < 5; partitionNo++)
	{
		int roomsInPartition = randInt(0, 2);
		int width, height, r, c;
		if (roomsInPartition == 0)
		{
			sectionSkipper++;
			if (sectionSkipper > 1)
			{
				roomsInPartition = randInt(1, 2);
			}
		}
		if (roomsInPartition == 2)
		{
			doubleSection++;
			if (doubleSection > 2)
			{
				roomsInPartition = 1;
			}
			else
			{
				for (int i = 0; i < 2; i++)
				{
					height = randInt(4, 6);
					width = randInt(9, 12);
					c = randInt(13 - width) + (14 * partitionNo) + 1;
					r = randInt(8 - height) + (i * 9) + 1;
					vRooms.push_back(Room(Coord(r, c), width, height));
				}
			}
		}
		if (roomsInPartition == 1)
		{
			height = randInt(4, 10);
			width = randInt(10, 12);
			c = randInt(13 - width) + (14 * partitionNo) + 1;
			r = randInt(17 - height) + 1;
			vRooms.push_back(Room(Coord(r, c), width, height));
		}
	}
}

void Dungeon::connectRooms()
{
	sortRooms();
	int noOfRooms = vRooms.size();
	for (int i = 0; i <  noOfRooms - 1 ; i++)
	{
		makePath(vRooms[i].getCentre(), vRooms[i + 1].getCentre());
	}
}

void Dungeon::makePath(Coord a, Coord b)
{

	bool connected = false;

	while (connected == false)
	{
		if (a.m_c < b.m_c) 
		{
			while (a.m_c != b.m_c)
			{
				a.m_c++;
				m_grid[a.m_r][a.m_c] = ' ';
			}
		}
	
		if (a.m_c == b.m_c) 
		{
			while (!(a == b))
			{
				
				if (a.m_r < b.m_r)
				{
					a.m_r++;
					m_grid[a.m_r][a.m_c] = ' ';
				}
				if (a.m_r > b.m_r)
				{
					a.m_r--;
					m_grid[a.m_r][a.m_c] = ' ';
				}
			}
			connected = true;
		}

	}
	
}

void Room::Centre()
{
	m_centre.m_r = (2*getTopLeft().m_r + m_height ) / 2;
	m_centre.m_c = (2*getTopLeft().m_c + m_width ) / 2;
}

void Dungeon::sortRooms()
{
	sort(vRooms.begin(), vRooms.end()); //Using the overloaded < operator.
}


///// SETTING THE PLAYER ///////////////////////////////////////////////////////////////////////////////////////////

void Dungeon::insertPlayer(Player* p) 
{
	p->setDungeon(this);
	player = p;
	setActorPosition(p);
}

bool Dungeon::isPlayerat(int r, int c)
{
	if (player->getPosition() == Coord(r, c))
		return true;
	else
		return false;
}

void Dungeon::setActorPosition(Actors* a)
{
	int r; int c;
	do
	{
		r = randInt(1, MAXROWS-1);
		c = randInt(1, MAXCOLS-1);
	} while (m_grid[r][c] != ' ' && isMonsterat(r,c) == false);
	
	a->setPosition(Coord(r, c));
}

///// SETTING THE MONSTERS ///////////////////////////////////////////////////////////////////////////////////////////

void Dungeon::insertMonsters() 
{
	int noOfMonsters = randInt(2, 5 * (m_level + 1)+1);
	for (int i = 0; i < noOfMonsters; i++) 
	{
		int d = 0;
		Monsters* m;
		switch (m_level) 
		{
		case 0:
		case 1:
			d = randInt(0,1);
			if (d == 0)
			{
				m = new Goblins();
				m->setDungeon(this);
				m->setSmellDist(m_goblinSmellDist);

			}
			else if (d == 1) 
			{ 
				m = new Snakewomen();
				m->setDungeon(this);
			}
			break;
		case 2:
			d = randInt(0,2);
			if (d == 0)
			{ 
				m = new Goblins();
				m->setDungeon(this);
				m->setSmellDist(m_goblinSmellDist); 
			}
			else if (d == 1) 
			{ 
				m =  new Snakewomen();
				m->setDungeon(this);
			}
			else if (d == 2) 
			{ 
				m = new Bogeymen();
				m->setDungeon(this);
			}
			break;
		case 3:
			d = randInt(0,3);
			if (d == 0) 
			{
				m = new Goblins();
				m->setDungeon(this);
				m->setSmellDist(m_goblinSmellDist); 
			}
			else if (d == 1)
			{ 
				m = new Snakewomen();
				m->setDungeon(this);
			}
			else if (d == 2)
			{ 
				m = new Bogeymen();
				m->setDungeon(this);
			}
			else if (d == 3) 
			{ 
				m = new Dragons(); 
				m->setDungeon(this);
			}
			break;
		case 4:
			d = randInt(0,3);
			if (d == 0) 
			{ 
				m = new Goblins();
				m->setDungeon(this);
				m->setSmellDist(m_goblinSmellDist); 
			}
			else if (d == 1)
			{ 
				m = new Snakewomen(); 
				m->setDungeon(this);
			}
			else if (d == 2)
			{ 
				m = new Bogeymen(); 
				m->setDungeon(this);
			}
			else if (d == 3) 
			{ 
				m = new Dragons();
				m->setDungeon(this);
			}
			break;
		default:
			break;
		}
		monsters.push_back(m);
		setActorPosition(m);
	}
}


bool Dungeon::isMonsterat(int r, int c)
{
	for (size_t i = 0; i < monsters.size(); i++)
	{
		if (monsters[i]->getPosition() == Coord(r, c))
			return true;
	}
	return false;
}

Monsters* Dungeon::whichMonster(int r, int c)
{
	for (size_t i = 0; i < monsters.size(); i++)
	{
		if (monsters[i]->getPosition() == Coord(r, c))
			return monsters[i];
	}
	return nullptr;
}

void Dungeon::monsterMove()
{
	for (size_t i = 0; i < monsters.size(); i++)
	{
		if (player->HitPoint() > 0)
		{
			int stepsFar = monsters[i]->getPosition().steps(this->getPlayer()->getPosition());
			if (stepsFar <= monsters[i]->smellDist())
			{
				monsters[i]->moveMonster(player, stepsFar);
			}

			if (monsters[i]->getName() == "Dragon")
			{
				int prob = trueWithProbability(0.1) ? 1 : 0;
				monsters[i]->setHitPoints(monsters[i]->HitPoint() + prob);
			}
		}
	}
}

///// SETTING THE OBJECTS ///////////////////////////////////////////////////////////////////////////////////////////

GameObjects* Dungeon::whichObject(int r, int c)
{
	for (size_t i = 0; i < obj.size(); i++)
	{
		if (obj[i]->getPos() == Coord(r, c))
			return obj[i];
	}
	return nullptr;
}

bool Dungeon::isObjectat(int r, int c)
{
	for (size_t i = 0; i < obj.size(); i++)
	{
		if (obj[i]->getPos() == Coord(r, c))
		{
			return true;
		}
	}
	return false;
}

void Dungeon::setObjectPosition(GameObjects* o)
{
		int r; int c;
		do
		{
			r = randInt(1, MAXROWS - 1);
			c = randInt(1, MAXCOLS - 1);
		} while (m_grid[r][c] != ' ' && isObjectat(r, c) == false);
		o->setPos(Coord(r, c));
}

void Dungeon::insertObjects()
{
	int max = randInt(2,3);
		while (max != 0) 
		{
			switch (randInt(0,1))
			{
			case 0:
				generateWeapons(1);
				max--;
				break;
			case 1:
				generateScrolls(1);
				max--;
				break;
			default:
				break;
			}
		}
		if (m_level < 4)
		{
			obj.push_back(new Stairs());
		}
		if (m_level == 4)
		{
			obj.push_back(new Gold());
		}
		for (size_t i = 0; i < obj.size(); i++)
		{
			setObjectPosition(obj[i]);
		}

}

void Dungeon::generateWeapons(int max)
{
	for (int i = 0; i < max; i++) 
	{
		switch (randInt(0,2))
		{
		case 0:
			obj.push_back(new Mace());
			break;
		case 1:
			obj.push_back(new ShortSword());
			break;
		case 2:
			obj.push_back(new LongSword());
			break;
		default:
			break;
		}
	}

}

void Dungeon::generateScrolls(int max)
{
	for (int i = 0; i < max; i++)
	{
		switch (randInt(0,3))
		{
		case 0:
			obj.push_back(new StrengthS());
			break;
		case 1:
			obj.push_back(new ArmorS());
			break;
		case 2:
			obj.push_back(new DexterityS());
			break;
		case 3:
			obj.push_back(new HealthS());
			break;
		default:
			break;
		}
	}
}

bool Dungeon::isOnObject(Coord pos, GameObjects* &p)
{
	if (isObjectat(pos.m_r, pos.m_c))
	{
		p = whichObject(pos.m_r, pos.m_c);
		return true;
	}
	return false;
}

void Dungeon::removeObj(Coord pos)
{
	for (size_t i = 0; i < obj.size(); i++)
	{
		if (obj[i]->getPos() == pos)
		{
			obj.erase(obj.begin() + i);
		}
	}
}

///// DISPLAYING STUFF ////////////////////////////////////////////////////////////////////////////////////////////////

void Dungeon::reduceSleepTimer()
{
	int playerSleep = player->sleepTime();
	if (playerSleep > 0)
	{
		player->setSleeptime(playerSleep - 1);
	}

	for (size_t i = 0; i < monsters.size(); i++)
	{
		int monsterSleep = monsters[i]->sleepTime();
		if (monsterSleep > 0)
		{
			monsters[i]->setSleeptime(monsterSleep - 1);
		}
	}
}

void Dungeon::updateActionMsg(string msg)
{
	actionMessages.push_back(msg); 

}

void Dungeon::displayDungeon()
{
	clearScreen();
	for (int r = 0; r < MAXROWS; r++)
	{
		for (int c = 0; c < MAXCOLS; c++)
		{
			
			if (isPlayerat(r, c))
			{
				cout << player->symbol();
			}
			else if (isMonsterat(r, c))
			{
				cout << whichMonster(r, c)->symbol();
			}
			else if (isObjectat(r, c))
			{
				cout << whichObject(r, c)->getSymbol();
			}
			else
			{
				cout << m_grid[r][c];
			}
		}
		cout << endl;
	}
	cout << "Dungeon Level: " << m_level << ", Hit points: " << player->HitPoint() << ", Armor: " << player->armorPoints() << ", Strength: " << player->strengthPoints() << ", Dexterity: " << player->dexterityPoints() << endl;
	cout << endl;
	displayMsg();
}


void Dungeon::displayMsg()
{
	while (!actionMessages.empty())
	{
		string temp = actionMessages.front();
		cout << temp << endl;
		actionMessages.erase(actionMessages.begin());
	}
}


///// CLEANING UP //////////////////////////////////////////////////////////////////////////////////////////////////

void Dungeon::removeDeadandDropItem(Monsters* m)
{
	Coord pos = m->getPosition();
	for (size_t i = 0; i < monsters.size(); i++)
	{
		if (m->getPosition() == monsters[i]->getPosition())
		{
			monsters.erase(monsters.begin() + i);
		}
	}
	//Sets Position of the dropped Item///////
	if (!isObjectat(pos.m_r, pos.m_c))
	{
		if (m->getName() == "Bogeyman")
		{
			if (trueWithProbability(m->dropProbability()))
			{
				GameObjects* dropped = new MagicAxe();
				dropped->setPos(pos);
				obj.push_back(dropped);
				return;
			}
		}
		if (m->getName() == "Snakewoman")
		{
			if (trueWithProbability(m->dropProbability()))
			{
				GameObjects* dropped = new MagicFang();
				dropped->setPos(pos);
				obj.push_back(dropped);
				return;
			}
		}
		if (m->getName() == "Dragon")
		{
			if (trueWithProbability(m->dropProbability()))
			{
				GameObjects* dropped;
				switch (randInt(0, 4))
				{
				case 0:
					dropped = new StrengthS();
					break;
				case 1:
					dropped = new ArmorS();
					break;
				case 2:
					dropped = new DexterityS();
					break;
				case 3:
					dropped = new HealthS();
					break;
				case 4:
					dropped = new teleportS();
					break;
				default:
					dropped = new teleportS();
					break;
				}
				dropped->setPos(pos);
				obj.push_back(dropped);
				return;
			}
		}
		if (m->getName() == "Goblin")
		{
			if (trueWithProbability(m->dropProbability()))
			{
				GameObjects* dropped;
				int rand = randInt(0, 1);
				switch (rand)
				{
				case 0:
					dropped = new MagicAxe();
					break;
				case 1:
					dropped = new MagicFang();
					break;
				default:
					dropped = new MagicAxe();
					break;
				}
				dropped->setPos(pos);
				obj.push_back(dropped);
				return;
			}
		}
	}

}

void Dungeon::clearAll() //Does not remove players
{
	while (!obj.empty())
	{
		GameObjects* toDelete = obj.back();
		obj.pop_back();
		delete toDelete;
	}
	while (!monsters.empty())
	{
		Monsters* toDelete = monsters.back();
		monsters.pop_back();
		delete toDelete;
	}

}

void Dungeon::removeMarker()
{
	for (int i = 0; i < MAXROWS; i++)
		for (int j = 0; j < MAXCOLS; j++)
		{
			if (m_grid[i][j] == '!')
				m_grid[i][j] = ' ';
		}
}

//// END OF CODE ///////////////////////////////////////////////////////////////////////////////////////////////////////////
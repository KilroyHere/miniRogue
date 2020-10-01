#ifndef DUNGEON_H
#define DUNGEON_H
#include"globals.h"
#include"geometry.h"
#include <string>
#include <vector>
class Actors;
class Player;
class Monsters;
class GameObjects;
class Weapon;

//The class Room makes the smaller room in the large dungeom

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// ROOMS //////////////////////////////////////////////////////////////////////////////////////////////////////////

class Room
{
public:

    ///Constructors
    Room() : m_topLeft(Coord(1, 1)), m_width(0), m_height(0) { Centre(); }
    Room(Coord tl, int width, int height) : m_topLeft(tl), m_width(width), m_height(height) { Centre(); }
    
    ///Accesssors
    Coord getTopLeft() { return m_topLeft; }
    Coord getCentre() { return m_centre; }
    int getWidth() { return m_width; }
    int getHeight() { return m_height; }

    ///Operator Overloader(s)
    bool operator<(const Room& other){ return (m_centre.m_c < other.m_centre.m_c); }
   
private:

//// Data Members
    void Centre();
    Coord m_topLeft;
    Coord m_centre;
    int m_width;
    int m_height;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// DUNGEON /////////////////////////////////////////////////////////////////////////////////////////////////////////

class Dungeon
{
public:

//// Constrcutor and Destructor
    Dungeon(int d, int level):m_goblinSmellDist(d) , m_level(level) {} 
    ~Dungeon() { clearAll(); }    

//// Core Dungeon Functions
    void makeDungeon();             //Makes the Dungeons skeleton 
    void insertPlayer(Player* p);   //Adds a player and sets its position
    void insertMonsters();          //Creates monsters and adds them to their vector
    void displayDungeon();          //Displays the dungeon
    void displayMsg();              //Display msg after the dungeon
    void insertObjects();           //Adds objects lying on the dungeon floor

//// Accessors and Mutators
    Player* getPlayer() { return player; }
    Monsters* whichMonster(int r, int c);
    GameObjects* whichObject(int r, int c);
    void setGrid(int r, int c, char ch) { m_grid[r][c] = ch; }
    char getGrid(int r, int c) { return m_grid[r][c]; }

//// Position Checkers
    bool isPlayerat(int r, int c);
    bool isMonsterat(int r, int c);
    bool isWallat(int r, int c) { if (m_grid[r][c] == '#') return true; else  return false;  }
    bool isObjectat(int r, int c);
    bool isOnObject(Coord pos, GameObjects* &p);

//// Utility Functions
    void updateActionMsg(std::string msg);     //Pushes messages to the actionString vector
    void removeObj(Coord pos);                 //Removes an Object from the given place
    void setActorPosition(Actors* a);          //Sets the actor's position to a random Place in the dungeon
    void removeDeadandDropItem(Monsters* m);   //Removes the dead monster from the dungeon and drops an object there
    void monsterMove();                        //Goes through the vector of Monsters to see if they can act
    void reduceSleepTimer();                   //Reduces sleep timer of all sleeping Actors
    void removeMarker();                       //Removes the marker used in the Goblins' Recursive function

//// Helper to the destructor
    void clearAll();

private:
    
//// Data Members
     
    Player* player;
    char m_grid[MAXROWS][MAXCOLS];
    int m_level;
    int m_goblinSmellDist;
    std::vector<Room> vRooms;
    std::vector<std::string> actionString;
    std::vector<GameObjects*> obj;
    std::vector<Monsters*> monsters;
    std::vector<std::string> actionMessages;

  
//// Internal helper functions
    //void makeDungeonSkeleton();
    void makeRooms();                   //Makes the rooms for the dungeon
    void addRooms();                    //Adds the room to the vRooms vector
    void sortRooms();                   //Sorts the vector vRooms using the Centre coordinate of the rooms
    void makePath(Coord a, Coord b);    //Makes the path between the sorted rooms
    void connectRooms();                //Connects the rooms using the paths
    void generateWeapons(int max);      //Makes random weapons for the dungeon
    void generateScrolls(int max);      //Makes random scrolls for the dungeon
    void setObjectPosition(GameObjects* o); //Given random Positions to the Objects in the dungeon
  
};

#endif
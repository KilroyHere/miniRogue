
#ifndef ACTORS_H
#define ACTORS_H
#include "geometry.h"
#include "utilities.h"
#include "GameObjects.h"
#include <string>
#include<queue>
#include<vector>
class Dungeon;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// ACTORS //////////////////////////////////////////////////////////////////////////////////////////////////////////

class Actors
{
public:
    
//// Constructors and Destructors
    Actors(std::string name) : m_name(name) , m_sleepTime(0), m_dungeon(nullptr), m_position(Coord(1,1)) {}
    virtual ~Actors() { if (this->m_name != "Player") delete this->m_weapon; }

//// Trivial Accessor and Mutator Functions
    std::string getName() const { return m_name; }
    void setName(std::string n) { m_name = n; }

    Coord getPosition() const { return m_position; }
    void setPosition(Coord p) { m_position = p; }
    int getRow() const { return m_position.m_r; }
    int getCol() const { return m_position.m_c; }

    Weapon* weapon() const {return m_weapon; }
    void setWeapon(Weapon* w) { m_weapon = w; }

    int armorPoints() const { return m_armorP; }
    void setArmorpoints(int a) { m_armorP = a; }

    int strengthPoints() const { return m_strengthP; }
    void setStrengthpoints(int s) { m_strengthP = s; }

    int dexterityPoints() const { return m_dexterityP; }
    void setDexteritypoints(int d) { m_dexterityP = d; }

    int sleepTime() const { return m_sleepTime; }
    void setSleeptime(int s) { m_sleepTime = s; }

    int HitPoint() const { return m_hitP; }
    void setHitPoints(int hp) { m_hitP = hp; }

    Dungeon* dungeon() const { return m_dungeon; }
    void setDungeon(Dungeon* d) { m_dungeon = d; }

    char symbol() const { return m_symbol; }
    void setSymbol(char s) { m_symbol = s; }

//// Utility Functions
//// Functions to check possible movement
    bool IfcanMove(int r , int c);      //Used by the Player and all Monsters barring Goblin
    bool canMove(int r, int c);         //Used by Goblin in its recursive function

//// Common Attacking Function used by all Actors
    void Attack(Actors* other);

private:

//// Data Members
    std::string m_name; 
    Coord m_position; 
    int m_hitP; 
    Weapon* m_weapon; 
    int m_armorP;
    int m_strengthP;
    int m_dexterityP;
    int m_sleepTime;
    Dungeon* m_dungeon;
    char m_symbol;

 //// Private functions used by the Attack() function
    int damage();       //Calculates dame done by a weapon
    void Die();         //Does the cleanup wehen a monster dies
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////// MONSTERS //////////////////////////////////////////////////////////////////////////////////////////////////////

class Monsters :public Actors //Derived from Actors
{
public:

//// Constructor and Destructor
    Monsters(std::string name) :Actors(name) {}
    virtual ~Monsters() {}

//// Trivial Accessor and Mutatir Functions
    double dropProbability() const { return m_dropProbability; }
    void setDropProbability(double d) { m_dropProbability = d; }
    
    int smellDist() const { return smellDistance; }
    void setSmellDist(int sd) { smellDistance = sd; }
    
//// Utility Functions
//// Function to move the Monsters
    virtual void moveMonster(Player *p , int steps);
private:

//// Data Members
    int smellDistance;
    double m_dropProbability;
   
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////// BOGEYMEN //////////////////////////////////////////////////////////////////////////////////////////////////////

class Bogeymen :public Monsters
{
public:

////Constructor and Destructor 
    Bogeymen(std::string name = "Bogeyman") :Monsters(name)
    {
        setHitPoints(randInt(6) + 5);
        setWeapon(new ShortSword());
        setStrengthpoints(randInt(2) + 2);
        setDexteritypoints(randInt(2) + 2);
        setArmorpoints(2);
        setSymbol('B');
        setDropProbability(0.1);
        setSmellDist(3);
    }
    virtual ~Bogeymen() {}
    
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////// SNAKEWOMEN ////////////////////////////////////////////////////////////////////////////////////////////////////

class Snakewomen :public Monsters
{
public:

 //// Constructor and Destructor 
    Snakewomen(std::string name = "Snakewoman") :Monsters(name)
    {
        setHitPoints(randInt(4) + 3);
        setWeapon(new MagicFang());
        setStrengthpoints(2);
        setDexteritypoints(3);
        setArmorpoints(3);
        setSymbol('S');
        setDropProbability(1.0 / 3);
        setSmellDist(3);
    }
    virtual ~Snakewomen(){}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////// DRAGONS ///////////////////////////////////////////////////////////////////////////////////////////////////////

class Dragons :public Monsters
{
public:

 //// Constructor and Destructor 
    Dragons( std::string name = "Dragon") :Monsters(name)
    {
        setHitPoints(randInt(6) + 20);
        setWeapon(new LongSword());
        setStrengthpoints(4);
        setDexteritypoints(4);
        setArmorpoints(4);
        setSymbol('D');
        setDropProbability(1);
        setSmellDist(1);
    }
    virtual ~Dragons(){}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////// GOBLINS ///////////////////////////////////////////////////////////////////////////////////////////////////////

class Goblins :public Monsters
{
public:

 //// Constructor and Destructor 
    Goblins(std::string name = "Goblin") :Monsters(name)
    {
        setHitPoints(randInt(6) + 15);
        setWeapon(new ShortSword());
        setStrengthpoints(3);
        setDexteritypoints(1);
        setArmorpoints(1);
        setSymbol('G');
        setDropProbability(1.0 / 3);
        setSmellDist(15);
    }
    virtual ~Goblins(){}
//// Utility Functions
    virtual void moveMonster(Player* p, int steps);     //Overloaded moveMOnster(), makes the Goblin move
    int shortestPathExists(Coord playerPos, int maxdist);       //Finds the shortest path recursively (moveMonster() call this)
    void clearqueue();                                  //Clears the queue used in the pathExists() function to keep track of visited Coordinates

private:

//// Private Data Members
    std::queue <placeInfo> cQueue;      //Helps keep track of the visited Coordinates  
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////// PLAYER ////////////////////////////////////////////////////////////////////////////////////////////////////////

class Player :public Actors
{
public:

//// Constructor and Destructor
    Player(std::string name = "Player") : Actors(name)
    {
        setHitPoints(20);
        Weapon* s = new ShortSword();
        inventory.push_back(s);
        setWeapon(s);
        setArmorpoints(2);
        setStrengthpoints(2);
        setDexteritypoints(2);
        setSymbol('@');
        setWin(false);
        setSleeptime(0);
        setMaxHitPoints(50);
    }
    virtual ~Player();

//// Trivial Accessors and Mutators
    bool won() const { return win; }
    void setWin(bool w) { win = w; }

    bool lost() const { return lose; }
    void setLose(bool l) { lose = l; }

    int getMaxHitPoints() { return maxHitPoints; }
    void setMaxHitPoints(int m) { maxHitPoints = m; }

//// Utility Functions
    void increaseHitPoint()     //Increases hit points of the player every turn
    {
        if (HitPoint() < maxHitPoints)
        {
            int prob = trueWithProbability(0.1) ? 1 : 0;
            setHitPoints(HitPoint() + prob);
        }
    }


//// Core Player Functions
    void PlayerMove(int Direction);     //Makes the Player move in a given direction
    void displayInventory();        //Displays the Players Inventory
    void pick();        //Makes the player pick up an object
    void wield();       //Makes the player wield a weapon
    void cheat();       //Makes the player cheat
    void read();        //Makes the player read a scroll
    
private:

//// Data Members
    bool win =  false;
    bool lose  = false;
    std::vector<GameObjects*> inventory; //Player's inventory
    int maxHitPoints;   //Players Max Hit Points ay change if A Health scroll is read.
};



#endif //ACTORS_H
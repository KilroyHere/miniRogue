#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H
#include "utilities.h"
#include <string>
#include "geometry.h"

class Dungeon;
class Actor;
class Player;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// GAME OBJECTS ////////////////////////////////////////////////////////////////////////////////////////////////////

class GameObjects
{
public:

//// Constructors and Destructors
    GameObjects(std::string name, std::string action, char sym) : m_name(name), m_action(action), symbol(sym) {}
    GameObjects(std::string name, char sym) : m_name(name) , symbol(sym) {}
    virtual ~GameObjects() {};

//// Accessors and Mutators
    std::string Name() const { return m_name; }
    std::string Action() const { return m_action;  }
    Coord getPos() const { return m_position; }
    char getSymbol() const{ return symbol; }
    void setPos(Coord pos) { m_position = pos; }
    virtual void foo() = 0; //MAKES CLASS ABSTRACT 
  
private:
//// Data Members
    char symbol;
    std::string m_name;
    std::string m_action;
    Coord m_position;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// WEAPONS //////////////////////////////////////////////////////////////////////////////////////////////////////////

class Weapon :public GameObjects
{
public:

//// Constructors and Destructors
    Weapon(std::string name, std::string action, int dex, int dmg) : GameObjects(name, action,')'), dexBonus(dex), dmgAmt(dmg){};
    virtual ~Weapon() {};
    virtual void foo() { return; }

//// Accessors and Mutators
    int DexBonus() const { return dexBonus; }
    int DmgAmt() const { return dmgAmt; }
 
private:
    int dexBonus;
    int dmgAmt;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// DIFFERENT WEAPONS ///////////////////////////////////////////////////////////////////////////////////////////////

class ShortSword : public Weapon
{
public:
    ShortSword(): Weapon("short sword", "slashes", 0, 2) {}
    virtual ~ShortSword() {};
};

class Mace : public Weapon
{
public:

    Mace(): Weapon("mace", "swings", 0, 2) {}
    virtual ~Mace() {};
};

class LongSword :public Weapon
{
public:

    LongSword(): Weapon("long sword", "swings", 2, 4) {}
    virtual ~LongSword() {};

};

class MagicAxe :public Weapon
{
public:
  
    MagicAxe(): Weapon("magic axe", "chops", 5, 5) {};
    virtual ~MagicAxe() {};
};

class MagicFang :public Weapon
{
public:
    MagicFang():Weapon("magic fangs of sleep", "strikes", 3, 2) {};
    virtual ~MagicFang() {};
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// SCROLLS /////////////////////////////////////////////////////////////////////////////////////////////////////////

class Scroll :public GameObjects
{
public:
    Scroll(std::string name, std::string effect) :GameObjects(name, effect, '?') {};
    virtual ~Scroll() {};
    virtual void foo() { return; }
    virtual void effectOnPlayer(Player *p) = 0;     //Pure Virtual because every scroll has a dfferent effect
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// DIFFERENT SCROLLS ///////////////////////////////////////////////////////////////////////////////////////////////

class StrengthS : public Scroll
{
public:
    StrengthS(): Scroll("strength scroll", "Your muscles bulge.") {};
    virtual ~StrengthS() {};
    virtual void effectOnPlayer(Player* p);    
};

class ArmorS : public Scroll
{
public:
    ArmorS():Scroll("scroll of enhance armor", "Your armor glows blue.") {};
    virtual ~ArmorS() {};
    virtual void effectOnPlayer(Player* p);   
};

class HealthS :public Scroll
{
public:

    HealthS() :Scroll("scroll of enhance health", "You feel your heart beating stronger.") {};
    virtual ~HealthS() {};
    virtual void effectOnPlayer(Player* p);    
};

class DexterityS :public Scroll
{
public:
    DexterityS():Scroll("scroll of enhance dexterity", "You feel like less of a klutz.") {};
    virtual ~DexterityS() {};
    virtual void effectOnPlayer(Player* p);    
};

class teleportS : public Scroll
{
public:
    teleportS() :Scroll("scroll of teleportation", "You feel your body wrenched in space and time.") {};
    virtual ~teleportS() {};
    virtual void effectOnPlayer(Player* p);
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// STAIRS AND GOLD IDOL ////////////////////////////////////////////////////////////////////////////////////////////

class Stairs :public GameObjects
{
public:
    Stairs(std::string name = "stairs", char sym = '>') : GameObjects(name, sym) { }
    virtual void foo() { return; }
};

class Gold :public GameObjects 
{
public:
    Gold(std::string name = "the golden idol", char sym = '&') :GameObjects(name, sym) { }
    virtual void foo() { return; }
};


#endif //GAMEOBJECTS_H
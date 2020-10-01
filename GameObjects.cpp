#include<iostream>
#include<string>
#include "utilities.h"
#include "GameObjects.h"
#include "Actors.h"
#include "Dungeon.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// SCROLL IMPLEMENTATIONS //////////////////////////////////////////////////////////////////////////////////////////

void StrengthS::effectOnPlayer(Player* p)
{
    int strength = p->strengthPoints();
    int newStrength = strength + randInt(1, 3);
    if(newStrength <= 99)
    p->setStrengthpoints(newStrength);
}
void ArmorS::effectOnPlayer(Player* p)
{
    int armor = p->armorPoints();
    int newArmor = armor + randInt(1, 3);
    if(newArmor <= 99)
    p->setArmorpoints(newArmor);
}
 void HealthS::effectOnPlayer(Player* p)
{
    int maxHP = p->getMaxHitPoints();
    int newMaxHP = maxHP + randInt(3, 5);
    if (newMaxHP <=  99)
    p->setMaxHitPoints(newMaxHP);
}
void DexterityS::effectOnPlayer(Player* p)
{
    int dexterity = p->dexterityPoints();
    if(dexterity+1 <= 99)
    p->setDexteritypoints(dexterity + 1);
}
 void teleportS::effectOnPlayer(Player* p)
{
    p->dungeon()->setActorPosition(p);
}

 ///END OF CODE /////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <iostream>
#include <string>
#include <cmath>

//// The Coord structure defined here helps keep track of all the coordinates in the Dungeon's grid in an efficient way.  
//// The placeInfo class defined here keeps track of Coordinates for the recursive pathExists() function used in the Goblins class

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// COORD  //////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Coord
{
    int m_r; /// The Row Coordinate
    int m_c; /// The Column Coordinate

    ///Constructors
    Coord(int rr = 0, int cc = 0) : m_r(rr), m_c(cc) {}
    Coord(const Coord& other)
    {
        m_r = other.m_r;
        m_c = other.m_c;
    }

    /// Utility Functions
    int steps(Coord p)  ///Manhattan Distance
    { 
        return abs(m_r - p.m_r) + abs(m_c - p.m_c); 
    }

    /// Operators Overloaded
    bool operator==(const Coord& other) 
    { 
        if (m_r == other.m_r && m_c == other.m_c)
            return true;
        else 
            return false;
    }
     Coord& operator=(const Coord& other)
     {
         if (this != &other)
         {
             m_r = other.m_r;
             m_c = other.m_c;
         }
         return *this;
     }
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// PLACEINFO ///////////////////////////////////////////////////////////////////////////////////////////////////////

class placeInfo //For the Goblin's recursive function (Keeps he details of the Coordinate being explored)
{
public:
    placeInfo(Coord p, int d) { pos = p; dist = d; }
    Coord pos;
    int dist;
};

#endif // GEOMETRY_H
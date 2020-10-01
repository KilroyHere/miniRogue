#ifndef GAME_H
#define GAME_H

// You may add data members and other member functions to this class.

class Game
{
public:

    //Constructor
    Game() :m_level(0) {}
    
    //Utility Functions
    void play(int dist);    //Starts the game
  
    //Accessors
    int getLevel() { return m_level; }

    //Mutators
    void setLevel(int l) { m_level = l; }

private:
    int goblinSmellDist;
    int m_level;
    bool levelChange = false;
};

#endif // GAME_H


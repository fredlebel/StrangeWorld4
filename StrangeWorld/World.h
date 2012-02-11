#ifndef _STRANGEWORLD_H_INCLUDED_
#define _STRANGEWORLD_H_INCLUDED_

#include <vector>
#include "Operations/OpAsyncHitTest.h"
#include "IThread.h"

class Creature;
class Operation;
class StrangeView;

class World
{
public:
    typedef std::vector<Creature*> CreatureList;

    int growthRate_;

private:
    int width_;
    int height_;
    unsigned int tickCount_;
    CreatureList creatureList_;
    CreatureList _creaturesToAdd;
    std::vector<IThread*> _workerThreads;

public:
    World( int width, int height, std::vector<IThread*> workerThreads );
    virtual ~World();
    
    unsigned int getTickCount();
    void addCreature( Creature* creature, bool autopos = false );
    int getWidth();
    int getHeight();
    void tick();
    int creatureCount();
    inline void wrapXY( double& x, double &y )
    {
        while ( x < 0 )
            x += width_;
        while ( x >= width_ )
            x -= width_;

        while ( y < 0 )
            y += height_;
        while ( y >= height_ )
            y -= height_;
    }
    Creature* checkContact( Creature* creature, OpAsyncHitTest::WantToHit wth );
    void globalOperation( Operation* operation );

    /** Level from '0' to '9' */
    static void setMutationLevel( unsigned int level );
    static unsigned int getMutationLevel();

    // Creature selection
    void toggleCreatureSelection( Creature* creature );
    void clearAllSelection();
    void selectionAll();

    void render( StrangeView* view, bool drawSensors, bool drawData );

};

#endif // _STRANGEWORLD_H_INCLUDED_
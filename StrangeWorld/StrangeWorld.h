#ifndef _STRANGEWORLD_H_INCLUDED_
#define _STRANGEWORLD_H_INCLUDED_

#include <list>
#include "OperationCreatureHitTest.h"

class StrangeCreature;
class StrangeCreatureOperation;
class StrangeView;

class StrangeWorld
{
public:
    typedef std::list<StrangeCreature*>::iterator CreatureIterator;
    typedef std::list<StrangeCreature*> CreatureList;

    int growthRate_;

private:
    int width_;
    int height_;
    unsigned int tickCount_;
    CreatureList creatureList_;

public:
    StrangeWorld( int width, int height, int growthRate );
    virtual ~StrangeWorld();
    
    unsigned int getTickCount();
    void addCreature( StrangeCreature* creature, bool autopos = false );
    int getWidth();
    int getHeight();
    CreatureIterator getCreatureIterator();
    void tick();
    int creatureCount();
    inline void wrapX( int& x )
    {
        if ( x < 0 )
            x += width_;
        else if ( x >= width_ )
            x %= width_;
    }
    inline void wrapY( int& y )
    {
        if ( y < 0 )
            y += height_;
        else if ( y >= height_ )
            y %= height_;
    }
    StrangeCreature* checkContact( StrangeCreature* creature, OperationCreatureHitTest::WantToHit wth );
    void globalOperation( StrangeCreatureOperation* operation );

    /** Level from '0' to '9' */
    static void setMutationLevel( unsigned int level );
    static unsigned int getMutationLevel();

    // Creature selection
    void toggleCreatureSelection( StrangeCreature* creature );
    void clearAllSelection();
    void selectionAll();

    void render( StrangeView* view, bool drawSensors, bool drawData );

};

#endif // _STRANGEWORLD_H_INCLUDED_
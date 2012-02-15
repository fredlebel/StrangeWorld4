#ifndef _STRANGEWORLD_H_INCLUDED_
#define _STRANGEWORLD_H_INCLUDED_

#include <vector>
#include <algorithm>
#include <memory>
#include "IThread.h"

class Creature;
class Grass;
class Herbivore;
class Carnivore;
class LivingCreature;
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
    std::vector<std::unique_ptr<Grass>> _grasses;
    std::vector<std::unique_ptr<Herbivore>> _herbivores;
    std::vector<std::unique_ptr<Carnivore>> _carnivores;

    std::vector<Creature*> _creaturesToAdd;
    std::vector<IThread*> _workerThreads;

public:
    World( int width, int height, std::vector<IThread*> workerThreads );
    virtual ~World();
    
    unsigned int getTickCount();
    void addCreature( Creature* creature, bool autopos = false );
    int getWidth();
    int getHeight();
    void tick();
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

    void runOperation( Operation& operation );

    bool operateOnGrass( std::function<bool(Grass*)> fn );
    bool operateOnHerbivore( std::function<bool(Herbivore*)> fn );
    bool operateOnCarnivore( std::function<bool(Carnivore*)> fn );
    bool operateOnAll( std::function<bool(Creature*)> fn );
    bool operateOnLivingCreature( std::function<bool(LivingCreature*)> fn );

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
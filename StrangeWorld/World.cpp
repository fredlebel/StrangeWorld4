#include "World.h"
#include "Creatures/Creature.h"
#include "Operations/Operation.h"
#include "Operations/OpAsyncPreTick.h"
#include "Operations/OpTick.h"
#include "Operations/OpRender.h"
#include "Creatures/Grass.h"
#include "Creatures/Herbivore.h"
#include "Creatures/Carnivore.h"
#include "NeuralNetwork/NNGene.h"
#include "StrangeView.h"
#include "FastRand.h"

// Function name   : World::World
// Description     : 
// Return type     : 
// Argument        : int width
// Argument        : int height
World::World( int width, int height, int growthRate )
    : width_( width )
    , height_( height )
    , tickCount_( 0 )
    , growthRate_( growthRate )
{
}


// Function name   : World::~World
// Description     : 
// Return type     : 
World::~World()
{
    // Delete all creatures
    for ( CreatureList::iterator i = creatureList_.begin(); i != creatureList_.end(); ++i )
    {
        Creature* pCreature = *i;
        delete pCreature;
    }
    creatureList_.clear();
}


// Function name   : World::addCreature
// Description     : 
// Return type     : void 
// Argument        : Creature* creature
// Argument        : bool autopos
void World::addCreature( Creature* creature, bool autopos )
{
    creatureList_.push_back( creature );
    creature->setWorld( this );
    
    if ( autopos )
    {
        int r = randomMT() % width_;
        creature->setX( r );
        r = randomMT() % height_;
        creature->setY( r );
    }
}


// Function name   : World::getTickCount
// Description     : 
// Return type     : unsigned int 
unsigned int World::getTickCount()
{
    return tickCount_;
}


// Function name   : World::getWidth
// Description     : 
// Return type     : int 
int World::getWidth()
{
    return width_;
}


// Function name   : World::getHeight
// Description     : 
// Return type     : int 
int World::getHeight()
{
    return height_;
}


// Function name   : World::tick
// Description     : 
// Return type     : void 
void World::tick()
{
    ++tickCount_;
    // Tick all creatures in the list
    {
        // This operation can be run across multiple threads.
        OpAsyncPreTick op( this );
        globalOperation( &op );
    }
    {
        OpTick op( this );
        globalOperation( &op );
    }

    // Remove all the dead creatures from the list
#if 0
    CreatureList::iterator i = creatureList_.begin();
    while ( i != creatureList_.end() )
    {
        Creature* creature = *i;
        if ( creature->getHealth() == 0 )
        {
            CreatureList::iterator deadCreature = i++;
            creatureList_.erase( deadCreature );
            delete creature;
        }
        else
        {
            ++i;
        }
    }
#else
    CreatureList::iterator i = creatureList_.begin();
    while ( i != creatureList_.end() )
    {
        Creature* creature = *i;
        if ( creature->getHealth() == 0 )
        {
            creatureList_.erase( i );
            delete creature;
            i = creatureList_.begin();
        }
        else
        {
            ++i;
        }
    }
#endif

    // Check for extinctions
    while ( Carnivore::CREATURE_COUNT < 15 && ( Carnivore::ourEliteGene.get() != nullptr ) )
    {
        // Create based on the elite gene.
        NNGene* pGene = new NNGene( Carnivore::ourEliteGene.get() );
        pGene->mutate();
        addCreature( new Carnivore( pGene ), true );
    }
    while ( Herbivore::CREATURE_COUNT < 15 && ( Herbivore::ourEliteGene.get() != nullptr ) )
    {
        NNGene* pGene = new NNGene( Herbivore::ourEliteGene.get() );
        pGene->mutate();
        addCreature( new Herbivore( pGene ), true );
    }
#if 0
    while ( Grass::CREATURE_COUNT < growthRate_ )
        addCreature( new Grass(), true );
#else
    if ( ( tickCount_ % 20 == 0 ) && ( Grass::CREATURE_COUNT < growthRate_ ) )
        addCreature( new Grass(), true );
#endif
}


// Function name   : World::creatureCount
// Description     : 
// Return type     : int 
int World::creatureCount()
{
    return (int)creatureList_.size();
}

// Function name   : World::checkContact
// Description     : 
// Return type     : Creature* 
// Argument        : Creature* ignore
Creature* World::checkContact( Creature* creature, OpAsyncHitTest::WantToHit wth )
{
    OpAsyncHitTest op( creature->getX(), creature->getY(), creature, wth );
    globalOperation( &op );

    return op.creatureHit;

}


// Function name   : World::globalOperation
// Description     : 
// Return type     : void 
// Argument        : Operation* operation
void World::globalOperation( Operation* operation )
{
    CreatureList::iterator i;
    CreatureList::iterator endOfList = creatureList_.end();
    for ( i = creatureList_.begin(); i != endOfList; ++i )
    {
        (*i)->accept( operation );
        if ( operation->mustStop )
            break;
    }
}

void World::setMutationLevel( unsigned int level )
{
    NNGene::setMutationLevel( level );
}

unsigned int World::getMutationLevel()
{
    return NNGene::getMutationLevel();
}

void World::toggleCreatureSelection( Creature* creature )
{
    if ( creature != nullptr )
        creature->selected_ = !creature->selected_;
}

void World::clearAllSelection()
{
    CreatureList::iterator i;
    CreatureList::iterator endOfList = creatureList_.end();
    for ( i = creatureList_.begin(); i != endOfList; ++i )
    {
        (*i)->selected_ = false;
    }
}

void World::selectionAll()
{
    CreatureList::iterator i;
    CreatureList::iterator endOfList = creatureList_.end();
    for ( i = creatureList_.begin(); i != endOfList; ++i )
    {
        (*i)->selected_ = true;
    }
}

void World::render( StrangeView* view, bool drawSensors, bool gDrawData )
{
    OperationCreatureRenderer renderer( view, drawSensors, gDrawData );
    globalOperation( &renderer );
}

#include "StrangeWorld.h"
#include "StrangeCreature.h"
#include "StrangeCreatureOperation.h"
#include "OperationCreatureTick.h"
#include "OperationCreatureRenderer.h"
#include "StrangeGrass.h"
#include "StrangeHerbivore.h"
#include "StrangeCarnivore.h"
#include "StrangeNNGene.h"
#include "StrangeView.h"
#include "FastRand.h"


// Function name   : StrangeWorld::StrangeWorld
// Description     : 
// Return type     : 
// Argument        : int width
// Argument        : int height
StrangeWorld::StrangeWorld( int width, int height, int growthRate )
    : width_( width )
    , height_( height )
    , tickCount_( 0 )
    , growthRate_( growthRate )
{
}


// Function name   : StrangeWorld::~StrangeWorld
// Description     : 
// Return type     : 
StrangeWorld::~StrangeWorld()
{
    // Delete all creatures
    for ( CreatureIterator i = creatureList_.begin(); i != creatureList_.end(); ++i )
    {
        StrangeCreature* pCreature = *i;
        delete pCreature;
    }
    creatureList_.clear();
}


// Function name   : StrangeWorld::addCreature
// Description     : 
// Return type     : void 
// Argument        : StrangeCreature* creature
// Argument        : bool autopos
void StrangeWorld::addCreature( StrangeCreature* creature, bool autopos )
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


// Function name   : StrangeWorld::getTickCount
// Description     : 
// Return type     : unsigned int 
unsigned int StrangeWorld::getTickCount()
{
    return tickCount_;
}


// Function name   : StrangeWorld::getWidth
// Description     : 
// Return type     : int 
int StrangeWorld::getWidth()
{
    return width_;
}


// Function name   : StrangeWorld::getHeight
// Description     : 
// Return type     : int 
int StrangeWorld::getHeight()
{
    return height_;
}


// Function name   : StrangeWorld::getCreatureIterator
// Description     : 
// Return type     : StrangeWorld::CreatureIterator 
StrangeWorld::CreatureIterator StrangeWorld::getCreatureIterator()
{
    return creatureList_.begin();
}


// Function name   : StrangeWorld::tick
// Description     : 
// Return type     : void 
void StrangeWorld::tick()
{
    ++tickCount_;
    // Tick all creatures in the list
    OperationCreatureTick op( this );
    globalOperation( &op );

    // Remove all the dead creatures from the list
    CreatureIterator i = creatureList_.begin();
    while ( i != creatureList_.end() )
    {
        StrangeCreature* creature = *i;
        if ( creature->getHealth() == 0 )
        {
            CreatureIterator deadCreature = i++;
            creatureList_.erase( deadCreature );
            delete creature;
        }
        else
        {
            ++i;
        }
    }
    // Check for extinctions
    /*
    int j;
    if ( 0 == StrangeCarnivore::CREATURE_COUNT && ( NULL != StrangeCarnivore::ourEliteGene ) )
        for ( j = 0; j < 10; ++j )
        {
            StrangeNNGene* pGene = StrangeCarnivore::ourEliteGene->cloneGene();
            pGene->mutate();
            addCreature( new StrangeCarnivore( pGene ), true );
        }
    if ( 0 == StrangeHerbivore::CREATURE_COUNT && ( NULL != StrangeHerbivore::ourEliteGene ) )
        for ( j = 0; j < 10; ++j )
        {
            StrangeNNGene* pGene = StrangeHerbivore::ourEliteGene->cloneGene();
            pGene->mutate();
            addCreature( new StrangeHerbivore( pGene ), true );
        }
    */
    while ( 15 >= StrangeCarnivore::CREATURE_COUNT && ( NULL != StrangeCarnivore::ourEliteGene ) )
    {
        StrangeNNGene* pGene = new StrangeNNGene( StrangeCarnivore::ourEliteGene );
        pGene->mutate();
        addCreature( new StrangeCarnivore( pGene ), true );
    }
    while ( 15 >= StrangeHerbivore::CREATURE_COUNT && ( NULL != StrangeHerbivore::ourEliteGene ) )
    {
        StrangeNNGene* pGene = new StrangeNNGene( StrangeHerbivore::ourEliteGene );
        pGene->mutate();
        addCreature( new StrangeHerbivore( pGene ), true );
    }
#if 0
    while ( StrangeGrass::CREATURE_COUNT < growthRate_ )
        addCreature( new StrangeGrass(), true );
#else
    if ( ( tickCount_ % 20 == 0 ) && ( StrangeGrass::CREATURE_COUNT < growthRate_ ) )
        addCreature( new StrangeGrass(), true );
#endif
}


// Function name   : StrangeWorld::creatureCount
// Description     : 
// Return type     : int 
int StrangeWorld::creatureCount()
{
    return (int)creatureList_.size();
}

// Function name   : StrangeWorld::checkContact
// Description     : 
// Return type     : StrangeCreature* 
// Argument        : StrangeCreature* ignore
StrangeCreature* StrangeWorld::checkContact( StrangeCreature* creature, OperationCreatureHitTest::WantToHit wth )
{
    OperationCreatureHitTest op( creature->getX(), creature->getY(), creature, wth );
    globalOperation( &op );

    return op.creatureHit;

}


// Function name   : StrangeWorld::globalOperation
// Description     : 
// Return type     : void 
// Argument        : StrangeCreatureOperation* operation
void StrangeWorld::globalOperation( StrangeCreatureOperation* operation )
{
    CreatureIterator i;
    CreatureIterator endOfList = creatureList_.end();
    for ( i = creatureList_.begin(); i != endOfList; ++i )
    {
        (*i)->accept( operation );
        if ( operation->mustStop )
            break;
    }
}

void StrangeWorld::setMutationLevel( unsigned int level )
{
    StrangeNNGene::setMutationLevel( level );
}

unsigned int StrangeWorld::getMutationLevel()
{
    return StrangeNNGene::getMutationLevel();
}

void StrangeWorld::toggleCreatureSelection( StrangeCreature* creature )
{
    if ( creature != NULL )
        creature->selected_ = !creature->selected_;
}

void StrangeWorld::clearAllSelection()
{
    CreatureIterator i;
    CreatureIterator endOfList = creatureList_.end();
    for ( i = creatureList_.begin(); i != endOfList; ++i )
    {
        (*i)->selected_ = false;
    }
}

void StrangeWorld::selectionAll()
{
    CreatureIterator i;
    CreatureIterator endOfList = creatureList_.end();
    for ( i = creatureList_.begin(); i != endOfList; ++i )
    {
        (*i)->selected_ = true;
    }
}

void StrangeWorld::render( StrangeView* view, bool drawSensors, bool gDrawData )
{
    OperationCreatureRenderer renderer( view, drawSensors, gDrawData );
    globalOperation( &renderer );
}

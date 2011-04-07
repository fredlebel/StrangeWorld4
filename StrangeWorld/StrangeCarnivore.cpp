#include "StrangeCarnivore.h"
#include "StrangeCreatureOperation.h"
#include "StrangeNNGene.h"
#include "StrangeWorldSettings.h"

int StrangeCarnivore::CREATURE_COUNT       = 0;
int StrangeCarnivore::ourAverageAge        = 0;
int StrangeCarnivore::ourAverageSpawnCount = 0;
int StrangeCarnivore::ourAverageFeedCount  = 0;
int StrangeCarnivore::ourDeathCount        = 0;

// Elite gene, usually the latest one
StrangeNNGene* StrangeCarnivore::ourEliteGene = NULL;


// Function name   : StrangeCarnivore::StrangeCarnivore
// Description     : 
// Return type     : 
// Argument        : StrangeNNGene* aGene
StrangeCarnivore::StrangeCarnivore( StrangeNNGene* aGene ) : StrangeLivingCreature( aGene )
{
    ++StrangeCarnivore::CREATURE_COUNT;
}


// Function name   : StrangeCarnivore::~StrangeCarnivore
// Description     : 
// Return type     : 
StrangeCarnivore::~StrangeCarnivore()
{
    --StrangeCarnivore::CREATURE_COUNT;
}


// Function name   : StrangeCarnivore::accept
// Description     : 
// Return type     : bool 
// Argument        : StrangeCreatureOperation* operation
bool StrangeCarnivore::accept( StrangeCreatureOperation* operation )
{
    operation->visit_Carnivore( this );
    return true;
}


// Function name   : StrangeCarnivore::getWidth
// Description     : 
// Return type     : int 
int StrangeCarnivore::getRadius()
{
    return bodyRadius_;
}


// Function name   : StrangeCarnivore::die
// Description     : 
// Return type     : void 
void StrangeCarnivore::die()
{
    assert( gene_ != NULL );

    bool isElite = false;

    // Save this gene as the elite one
    if ( age_ > ourAverageAge ) // if we are older than the average creature
    {
        ourAverageAge = age_;
#if defined( ELITE_IS_OLDEST )
        isElite = true;
#endif
    }

    if ( spawnCount_ >= ourAverageSpawnCount ) // if we have spawned more times than the average creature
    {
        ourAverageSpawnCount = spawnCount_;
#if defined( ELITE_IS_PROLIFIC )
        isElite = true;
#endif
    }

    if ( feedCount_ >= ourAverageFeedCount ) // if we have spawned more times than the average creature
    {
        ourAverageFeedCount = feedCount_;
#if defined( ELITE_EATS_MOST )
        isElite = true;
#endif
    }

    if ( isElite )
    {
        delete ourEliteGene;
        ourEliteGene = gene_;
    }
    else
    {
        delete gene_;
    }

    ourAverageAge        = ( ( ourAverageAge        * 16 ) + age_        ) / 17;
    ourAverageSpawnCount = ( ( ourAverageSpawnCount * 16 ) + spawnCount_ ) / 17;
    ourAverageFeedCount  = ( ( ourAverageFeedCount  * 16 ) + feedCount_  ) / 17;
    /*
    ourAverageAge && --ourAverageAge;
    ourAverageSpawnCount && --ourAverageSpawnCount;
    ourAverageFeedCount && --ourAverageFeedCount;
    */

    ++ourDeathCount;
    gene_ = NULL;
}



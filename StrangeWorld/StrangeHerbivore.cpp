#include "StrangeCreatureOperation.h"
#include "StrangeHerbivore.h"
#include "StrangeNNGene.h"
#include "StrangeWorldSettings.h"

int StrangeHerbivore::CREATURE_COUNT       = 0;
int StrangeHerbivore::ourAverageAge        = 0;
int StrangeHerbivore::ourAverageSpawnCount = 0;
int StrangeHerbivore::ourAverageFeedCount  = 0;
int StrangeHerbivore::ourDeathCount        = 0;

// Elite gene, depends wether the dead creature lived longer than the average age
StrangeNNGene* StrangeHerbivore::ourEliteGene = NULL;


// Function name   : StrangeHerbivore::StrangeHerbivore
// Description     : 
// Return type     : 
// Argument        : StrangeNNGene* aGene
StrangeHerbivore::StrangeHerbivore( StrangeNNGene* aGene ) : StrangeLivingCreature( aGene )
{
    ++StrangeHerbivore::CREATURE_COUNT;
}


// Function name   : StrangeHerbivore::~StrangeHerbivore
// Description     : 
// Return type     : 
StrangeHerbivore::~StrangeHerbivore()
{
    --StrangeHerbivore::CREATURE_COUNT;
}


// Function name   : StrangeHerbivore::accept
// Description     : 
// Return type     : bool 
// Argument        : StrangeCreatureOperation* operation
bool StrangeHerbivore::accept( StrangeCreatureOperation* operation )
{
    operation->visit_Herbivore( this );
    return true;
}


// Function name   : StrangeHerbivore::getWidth
// Description     : 
// Return type     : int 
int StrangeHerbivore::getRadius()
{
    return bodyRadius_;
//    return 10;
}


// Function name   : StrangeHerbivore::die
// Description     : 
// Return type     : void 
void StrangeHerbivore::die()
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



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
std::auto_ptr<StrangeNNGene> StrangeCarnivore::ourEliteGene;


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
    bool isElite = false;

#if defined( ELITE_IS_OLDEST )
    // Save this gene as the elite one
    if ( age_ > ourAverageAge ) // if we are older than the average creature
    {
        isElite = true;
    }
#endif

#if defined( ELITE_IS_PROLIFIC )
    if ( spawnCount_ >= ourAverageSpawnCount ) // if we have spawned more times than the average creature
    {
        isElite = true;
    }
#endif

#if defined( ELITE_EATS_MOST )
    if ( feedCount_ >= ourAverageFeedCount ) // if we have spawned more times than the average creature
    {
        isElite = true;
    }
#endif

    if ( isElite )
    {
        ourEliteGene = gene_;
    }

    ourAverageAge        = ( ( ourAverageAge        * ELITE_SAMPLING ) + age_        ) / (ELITE_SAMPLING+1);
    ourAverageSpawnCount = ( ( ourAverageSpawnCount * ELITE_SAMPLING ) + spawnCount_ ) / (ELITE_SAMPLING+1);
    ourAverageFeedCount  = ( ( ourAverageFeedCount  * ELITE_SAMPLING ) + feedCount_  ) / (ELITE_SAMPLING+1);

    ++ourDeathCount;
}



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
std::auto_ptr<StrangeNNGene> StrangeHerbivore::ourEliteGene;


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
}


// Function name   : StrangeHerbivore::die
// Description     : 
// Return type     : void 
void StrangeHerbivore::die()
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



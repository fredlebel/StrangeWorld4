#include "Carnivore.h"
#include "Operations/Operation.h"
#include "NeuralNetwork/NNGene.h"
#include "WorldSettings.h"

int Carnivore::CREATURE_COUNT       = 0;
int Carnivore::ourAverageAge        = 0;
int Carnivore::ourAverageSpawnCount = 0;
int Carnivore::ourAverageFeedCount  = 0;
int Carnivore::ourDeathCount        = 0;

// Elite gene, usually the latest one
std::auto_ptr<NNGene> Carnivore::ourEliteGene;


// Function name   : Carnivore::Carnivore
// Description     : 
// Return type     : 
// Argument        : NNGene* aGene
Carnivore::Carnivore( NNGene* aGene ) : LivingCreature( aGene )
{
    ++Carnivore::CREATURE_COUNT;
}


// Function name   : Carnivore::~Carnivore
// Description     : 
// Return type     : 
Carnivore::~Carnivore()
{
    --Carnivore::CREATURE_COUNT;
}


// Function name   : Carnivore::accept
// Description     : 
// Return type     : bool 
// Argument        : Operation* operation
bool Carnivore::accept( Operation* operation )
{
    operation->visit_Carnivore( this );
    return true;
}


// Function name   : Carnivore::getWidth
// Description     : 
// Return type     : int 
double Carnivore::getRadius()
{
    return bodyRadius_;
}


// Function name   : Carnivore::die
// Description     : 
// Return type     : void 
void Carnivore::die()
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



#include "Operations/Operation.h"
#include "Herbivore.h"
#include "NeuralNetwork/NNGene.h"
#include "WorldSettings.h"

int Herbivore::CREATURE_COUNT       = 0;
int Herbivore::ourAverageAge        = 0;
int Herbivore::ourAverageSpawnCount = 0;
int Herbivore::ourAverageFeedCount  = 0;
int Herbivore::ourDeathCount        = 0;

// Elite gene, depends wether the dead creature lived longer than the average age
std::auto_ptr<NNGene> Herbivore::ourEliteGene;


// Function name   : Herbivore::Herbivore
// Description     : 
// Return type     : 
// Argument        : NNGene* aGene
Herbivore::Herbivore( NNGene* aGene ) : LivingCreature( aGene )
{
    ++Herbivore::CREATURE_COUNT;
}


// Function name   : Herbivore::~Herbivore
// Description     : 
// Return type     : 
Herbivore::~Herbivore()
{
    --Herbivore::CREATURE_COUNT;
}


// Function name   : Herbivore::accept
// Description     : 
// Return type     : bool 
// Argument        : Operation* operation
bool Herbivore::accept( Operation* operation )
{
    operation->visit_Herbivore( this );
    return true;
}


// Function name   : Herbivore::getWidth
// Description     : 
// Return type     : int 
double Herbivore::getRadius()
{
    return bodyRadius_;
}


// Function name   : Herbivore::die
// Description     : 
// Return type     : void 
void Herbivore::die()
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



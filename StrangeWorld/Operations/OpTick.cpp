#include "Operations/OpTick.h"
#include "Creatures/Carnivore.h"
#include "Creatures/Herbivore.h"
#include "Creatures/Grass.h"
#include "World.h"
#include "WorldSettings.h"
#include "NeuralNetwork/NNGene.h"
#include "FastRand.h"
#include <assert.h>

// Function name   : OpTick::OpTick
// Description     : 
// Return type     : 
OpTick::OpTick( World* world )
    : world_( world )
{
    hurtCarnivore_ = (world_->getTickCount() % CARNIVORE_TICK_HURT) == 0;
    hurtHerbivore_ = (world_->getTickCount() % HERBIVORE_TICK_HURT) == 0;
}


// Function name   : OpTick::~OpTick
// Description     : 
// Return type     : 
OpTick::~OpTick()
{
}

// Function name   : OpTick::visit_Carnivore
// Description     : 
// Return type     : void 
// Argument        : Carnivore* creature
void OpTick::visit_Carnivore( Carnivore* creature )
{
    // Operation to eat
    auto eatFn = [&](Herbivore* herbivore)
    {
        if ( herbivore->isLiving() )
        {
            herbivore->kill();
            return;
        }

        // Feed on dead creature's corpse
        creature->heal( herbivore->hurt( FEED_AMOUNT ) / 2 );
        ++creature->feedCount_;

        // If the creature is ready to spawn
        if ( creature->getHealth() >= CARNIVORE_SPAWN_THRES &&
                creature->getAge() > CARNIVORE_SPAWN_AGE_THRES &&
                Carnivore::CREATURE_COUNT < 25 )
        {
            // Spawning takes energy
            creature->hurt( SPAWN_HURT * 2 );
            creature->spawnCount_++;
            double x = creature->getX();
            double y = creature->getY();
            for ( int i = 0; i < CARNIVORE_OFFSPRING_COUNT; ++i )
            {
                NNGene* geneCol = new NNGene( creature->getGene() );
                // Elitism can speed up evolution
                geneCol->mutate();
                Carnivore* nc = new Carnivore( geneCol );
                // Propagate the selection to the offspring
                nc->selected_ = creature->selected_;
                world_->addCreature( nc, false );
                // Place the new creature somewhere around the parent.
                nc->setX( x + (randomMT() % 40) - 20 );
                nc->setY( y + (randomMT() % 40) - 20 );
            }
        }
    };

    if ( creature->isLiving() )
    {
        creature->age();

        // Neural processing
        double energyUsed = creature->popBrainOutputs();
        creature->hurt( energyUsed );

        if ( creature->isLiving() )
        {
            // Eating phase
            if ( creature->herbivoreToEat != nullptr )
            {
                eatFn( creature->herbivoreToEat );
            }
        }

        if ( hurtCarnivore_ )
            creature->hurt( 1 );
    }
    else
    {
        creature->hurt( DECAY_RATE );
    }
}


// Function name   : OpTick::visit_Grass
// Description     : 
// Return type     : void 
// Argument        : Grass* creature
void OpTick::visit_Grass( Grass* creature )
{
//    // Grass heals constantly
//    if ( creature->isLiving() )
//        creature->heal( GRASS_HEAL );
}


// Function name   : OpTick::visit_Herbivore
// Description     : 
// Return type     : void 
// Argument        : Herbivore* creature
void OpTick::visit_Herbivore( Herbivore* creature )
{
    // Operation to eat
    auto eatFn = [&](Grass* grass)
    {
        // Feed on grass
        creature->heal( grass->hurt( FEED_AMOUNT ) );
        ++creature->feedCount_;

        // If the creature is ready to spawn
        if ( creature->getHealth() >= HERBIVORE_SPAWN_THRES &&
                creature->getAge() > HERBIVORE_SPAWN_AGE_THRES )
        {
            // Spawning takes energy
            creature->hurt( SPAWN_HURT );
            creature->spawnCount_++;
            double x = creature->getX();
            double y = creature->getY();
            for ( int i = 0; i < HERBIVORE_OFFSPRING_COUNT; ++i )
            {
                NNGene* geneCol = new NNGene( creature->getGene() );
                geneCol->mutate();
                Herbivore* nc = new Herbivore( geneCol );
                // Propagate the selection to the offspring
                nc->selected_ = creature->selected_;
                world_->addCreature( nc, false );
                // Place the new creature somewhere around the parent.
                nc->setX( x + (randomMT() % 40) - 20 );
                nc->setY( y + (randomMT() % 40) - 20 );
            }
        }
    };

    if ( creature->isLiving() )
    {
        creature->age();

        // Neural processing
        double energyUsed = creature->popBrainOutputs();
        creature->hurt( energyUsed );

        if ( creature->isLiving() )
        {
            // Eating phase
            if ( creature->grassToEat != nullptr )
            {
                eatFn( creature->grassToEat );
            }
        }

        if ( hurtHerbivore_ )
            creature->hurt( 1 );
    }
    else
    {
        creature->hurt( DECAY_RATE );
    }
}



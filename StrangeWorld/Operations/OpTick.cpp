#include "Operations/OpTick.h"
#include "Operations/OpAsyncHitTest.h"
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


// Function name   : OpTick::basicPhase
// Description     : 
// Return type     : void 
// Argument        : Operation* op
void OpTick::basicPhase( Operation* op,
                                       LivingCreature* creature,
                                       OpAsyncHitTest::WantToHit wth )
{
    creature->age();

    // Neural processing
    double energyUsed = creature->popBrainOutputs();
    creature->hurt( energyUsed );

    if ( creature->isLiving() )
    {
        // Eating phase
        Creature* c = world_->checkContact( creature, wth );
        if ( nullptr != c )
        {
            c->accept( op );
        }
    }
}



// Function name   : OpTick::visit_Carnivore
// Description     : 
// Return type     : void 
// Argument        : Carnivore* creature
void OpTick::visit_Carnivore( Carnivore* creature )
{
    // Operation to eat
    class EatOperation : public Operation
    {
    public:
        EatOperation( Carnivore* c, World* world )
            : creature_( c )
            , world_( world )
        {
        }

        virtual void visit_Herbivore( Herbivore* target )
        {
            if ( target->isLiving() )
            {
                target->kill();
                return;
            }

            // Feed on dead creature's corpse
            creature_->heal( target->hurt( FEED_AMOUNT ) / 2 );
            ++creature_->feedCount_;

            // If the creature is ready to spawn
            if ( creature_->getHealth() >= CARNIVORE_SPAWN_THRES &&
                 creature_->getAge() > CARNIVORE_SPAWN_AGE_THRES &&
                 Carnivore::CREATURE_COUNT < 25 )
            {
                // Spawning takes energy
                creature_->hurt( SPAWN_HURT * 2 );
                creature_->spawnCount_++;
                double x = creature_->getX();
                double y = creature_->getY();
                for ( int i = 0; i < CARNIVORE_OFFSPRING_COUNT; ++i )
                {
                    NNGene* geneCol = new NNGene( creature_->getGene() );
                    // Elitism can speed up evolution
#ifdef USE_ELITISM
                    if ( nullptr != Carnivore::ourEliteGene )
                        geneCol->merge( Carnivore::ourEliteGene );
#endif
                    geneCol->mutate();
                    Carnivore* nc = new Carnivore( geneCol );
                    // Propagate the selection to the offspring
                    nc->selected_ = creature_->selected_;
                    world_->addCreature( nc, false );
                    // Place the new creature somewhere around the parent.
                    nc->setX( x + (randomMT() % 40) - 20 );
                    nc->setY( y + (randomMT() % 40) - 20 );
                }
            }
        }

        Carnivore* creature_;
        World* world_;
    };

    if ( creature->isLiving() )
    {
        static EatOperation op( creature, world_ );
        op.creature_ = creature;
        basicPhase( &op, creature, OpAsyncHitTest::HitHerbivore );

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
    class EatOperation : public Operation
    {
    public:
        EatOperation( Herbivore* c, World* world )
            : creature_( c )
            , world_( world )
        {
        }

        virtual void visit_Grass( Grass* target )
        {
            // Feed on grass
            creature_->heal( target->hurt( FEED_AMOUNT ) );
            ++creature_->feedCount_;

            // If the creature is ready to spawn
            if ( creature_->getHealth() >= HERBIVORE_SPAWN_THRES &&
                 creature_->getAge() > HERBIVORE_SPAWN_AGE_THRES )
            {
                // Spawning takes energy
                creature_->hurt( SPAWN_HURT );
                creature_->spawnCount_++;
                double x = creature_->getX();
                double y = creature_->getY();
                for ( int i = 0; i < HERBIVORE_OFFSPRING_COUNT; ++i )
                {
                    NNGene* geneCol = new NNGene( creature_->getGene() );
                    // Elitism can speed up evolution
#ifdef USE_ELITISM
                    if ( nullptr != Herbivore::ourEliteGene )
                        geneCol->merge( Herbivore::ourEliteGene );
#endif
                    geneCol->mutate();
                    Herbivore* nc = new Herbivore( geneCol );
                    // Propagate the selection to the offspring
                    nc->selected_ = creature_->selected_;
                    world_->addCreature( nc, false );
                    // Place the new creature somewhere around the parent.
                    nc->setX( x + (randomMT() % 40) - 20 );
                    nc->setY( y + (randomMT() % 40) - 20 );
                }
            }
        }

        Herbivore* creature_;
        World* world_;
    };

    if ( creature->isLiving() )
    {
        static EatOperation op( creature, world_ );
        op.creature_ = creature;
        basicPhase( &op, creature, OpAsyncHitTest::HitGrass );

        if ( hurtHerbivore_ )
            creature->hurt( 1 );
    }
    else
    {
        creature->hurt( DECAY_RATE );
    }
}



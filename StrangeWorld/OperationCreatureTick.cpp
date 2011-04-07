#include "OperationCreatureTick.h"
#include "OperationCreatureHitTest.h"
#include "StrangeWorld.h"
#include "StrangeWorldSettings.h"
#include "StrangeNNGene.h"
#include "FastRand.h"
#include <assert.h>

// Function name   : OperationCreatureTick::OperationCreatureTick
// Description     : 
// Return type     : 
OperationCreatureTick::OperationCreatureTick( StrangeWorld* world )
    : world_( world )
{
    hurtCarnivore_ = (world_->getTickCount() % CARNIVORE_TICK_HURT) == 0;
    hurtHerbivore_ = (world_->getTickCount() % HERBIVORE_TICK_HURT) == 0;
}


// Function name   : OperationCreatureTick::~OperationCreatureTick
// Description     : 
// Return type     : 
OperationCreatureTick::~OperationCreatureTick()
{
}


// Function name   : OperationCreatureTick::basicPhase
// Description     : 
// Return type     : void 
// Argument        : StrangeCreatureOperation* op
void OperationCreatureTick::basicPhase( StrangeCreatureOperation* op,
                                       StrangeLivingCreature* creature,
                                       OperationCreatureHitTest::WantToHit wth )
{
    creature->age();

    // Neural processing
    creature->pushBrainInputs();
    creature->tickBrain();
    int energyUsed = creature->popBrainOutputs();
    creature->hurt( energyUsed );

    if ( creature->isLiving() )
    {
        // Eating phase
        StrangeCreature* c;
        c = world_->checkContact( creature, wth );
        if ( NULL != c )
        {
            c->accept( op );
        }
    }
}



// Function name   : OperationCreatureTick::visit_Carnivore
// Description     : 
// Return type     : void 
// Argument        : StrangeCarnivore* creature
void OperationCreatureTick::visit_Carnivore( StrangeCarnivore* creature )
{
    // Operation to eat
    class EatOperation : public StrangeCreatureOperation
    {
    public:
        EatOperation( StrangeCarnivore* c, StrangeWorld* world )
            : creature_( c )
            , world_( world )
        {
        }

        virtual void visit_Herbivore( StrangeHerbivore* target )
        {
            if ( target->isLiving() )
            {
                target->kill();
                return;
            }

            // Feed on dead creature's corpse
            creature_->heal( target->hurt( FEED_AMOUNT ) >> 1 );
            ++creature_->feedCount_;

            // If the creature is ready to spawn
            if ( creature_->getHealth() >= CARNIVORE_SPAWN_THRES &&
                 creature_->getAge() > CARNIVORE_SPAWN_AGE_THRES &&
                 StrangeCarnivore::CREATURE_COUNT < 25 )
            {
                // Spawning takes energy
                creature_->hurt( SPAWN_HURT * 2 );
                creature_->spawnCount_++;
                int x = creature_->getX();
                int y = creature_->getY();
                for ( int i = 0; i < CARNIVORE_OFFSPRING_COUNT; ++i )
                {
                    StrangeNNGene* geneCol = new StrangeNNGene( creature_->getGene() );
                    // Elitism can speed up evolution
#ifdef USE_ELITISM
                    if ( NULL != StrangeCarnivore::ourEliteGene )
                        geneCol->merge( StrangeCarnivore::ourEliteGene );
#endif
                    geneCol->mutate();
                    StrangeCarnivore* nc = new StrangeCarnivore( geneCol );
                    // Propagate the selection to the offspring
                    nc->selected_ = creature_->selected_;
                    world_->addCreature( nc, false );
                    // Place the new creature somewhere around the parent.
                    nc->setX( x + (randomMT() % 40) - 20 );
                    nc->setY( y + (randomMT() % 40) - 20 );
                }
            }
        }

        StrangeCarnivore* creature_;
        StrangeWorld* world_;
    };

    if ( creature->isLiving() )
    {
        static EatOperation op( creature, world_ );
        op.creature_ = creature;
        basicPhase( &op, creature, OperationCreatureHitTest::HitHerbivore );

        if ( hurtCarnivore_ )
            creature->hurt( 1 );
    }
    else
    {
        creature->hurt( DECAY_RATE );
    }
}


// Function name   : OperationCreatureTick::visit_Grass
// Description     : 
// Return type     : void 
// Argument        : StrangeGrass* creature
void OperationCreatureTick::visit_Grass( StrangeGrass* creature )
{
//    // Grass heals constantly
//    if ( creature->isLiving() )
//        creature->heal( GRASS_HEAL );
}


// Function name   : OperationCreatureTick::visit_Herbivore
// Description     : 
// Return type     : void 
// Argument        : StrangeHerbivore* creature
void OperationCreatureTick::visit_Herbivore( StrangeHerbivore* creature )
{
    // Operation to eat
    class EatOperation : public StrangeCreatureOperation
    {
    public:
        EatOperation( StrangeHerbivore* c, StrangeWorld* world )
            : creature_( c )
            , world_( world )
        {
        }

        virtual void visit_Grass( StrangeGrass* target )
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
                int x = creature_->getX();
                int y = creature_->getY();
                for ( int i = 0; i < HERBIVORE_OFFSPRING_COUNT; ++i )
                {
                    StrangeNNGene* geneCol = new StrangeNNGene( creature_->getGene() );
                    // Elitism can speed up evolution
#ifdef USE_ELITISM
                    if ( NULL != StrangeHerbivore::ourEliteGene )
                        geneCol->merge( StrangeHerbivore::ourEliteGene );
#endif
                    geneCol->mutate();
                    StrangeHerbivore* nc = new StrangeHerbivore( geneCol );
                    // Propagate the selection to the offspring
                    nc->selected_ = creature_->selected_;
                    world_->addCreature( nc, false );
                    // Place the new creature somewhere around the parent.
                    nc->setX( x + (randomMT() % 40) - 20 );
                    nc->setY( y + (randomMT() % 40) - 20 );
                }
            }
        }

        StrangeHerbivore* creature_;
        StrangeWorld* world_;
    };

    if ( creature->isLiving() )
    {
        static EatOperation op( creature, world_ );
        op.creature_ = creature;
        basicPhase( &op, creature, OperationCreatureHitTest::HitGrass );

        if ( hurtHerbivore_ )
            creature->hurt( 1 );
    }
    else
    {
        creature->hurt( DECAY_RATE );
    }
}



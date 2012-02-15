#include "World.h"
#include "Creatures/Creature.h"
#include "Operations/Operation.h"
#include "Operations/OpTick.h"
#include "Operations/OpRender.h"
#include "Creatures/Grass.h"
#include "Creatures/Herbivore.h"
#include "Creatures/Carnivore.h"
#include "NeuralNetwork/NNGene.h"
#include "StrangeView.h"
#include "FastRand.h"

World::World( int width, int height, std::vector<IThread*> workerThreads )
    : width_( width )
    , height_( height )
    , tickCount_( 0 )
    , growthRate_( 50 )
    , _workerThreads( workerThreads )
{
}


// Function name   : World::~World
// Description     : 
// Return type     : 
World::~World()
{
}


// Function name   : World::addCreature
// Description     : 
// Return type     : void 
// Argument        : Creature* creature
// Argument        : bool autopos
void World::addCreature( Creature* creature, bool autopos )
{
    _creaturesToAdd.push_back( creature );
    creature->setWorld( this );

    if ( autopos )
    {
        int r = randomMT() % width_;
        creature->setX( r );
        r = randomMT() % height_;
        creature->setY( r );
    }
}


// Function name   : World::getTickCount
// Description     : 
// Return type     : unsigned int 
unsigned int World::getTickCount()
{
    return tickCount_;
}


// Function name   : World::getWidth
// Description     : 
// Return type     : int 
int World::getWidth()
{
    return width_;
}


// Function name   : World::getHeight
// Description     : 
// Return type     : int 
int World::getHeight()
{
    return height_;
}


// Function name   : World::tick
// Description     : 
// Return type     : void 
void World::tick()
{
    ++tickCount_;

    // Start by adding all the new creatures
    {
        class AddOp : public Operation
        {
        public:
            AddOp( std::vector<std::unique_ptr<Grass>>* grasses,
                   std::vector<std::unique_ptr<Herbivore>>* herbivores,
                   std::vector<std::unique_ptr<Carnivore>>* carnivores )
                : _grasses( grasses )
                , _herbivores( herbivores )
                , _carnivores( carnivores )
            {
            }

            std::vector<std::unique_ptr<Grass>>* _grasses;
            std::vector<std::unique_ptr<Herbivore>>* _herbivores;
            std::vector<std::unique_ptr<Carnivore>>* _carnivores;

            virtual void visit_Grass( Grass* c ) { _grasses->push_back( std::move( std::unique_ptr<Grass>(c) ) ); }
            virtual void visit_Herbivore( Herbivore* c ) { _herbivores->push_back( std::move( std::unique_ptr<Herbivore>(c) ) ); }
            virtual void visit_Carnivore( Carnivore* c ) { _carnivores->push_back( std::move( std::unique_ptr<Carnivore>(c) ) ); }
        };

        AddOp op( &_grasses, &_herbivores, &_carnivores );
        for ( auto it = _creaturesToAdd.begin(); it != _creaturesToAdd.end(); ++it )
        {
            (*it)->accept( op );
        }
        _creaturesToAdd.clear();
    }

    // Tick all creatures in the list
#if 0
    {
        // This operation can be run across multiple threads.
        OpAsyncPreTick op( this );
        globalOperation( &op );
    }
#else
    {
        auto asyncFn = [&](LivingCreature* c)
        {
            c->pushBrainInputs();
            c->tickBrain();
            c->checkContactWithEdible();
        };

        // Spread the work over the worker threads.
        for ( unsigned int threadIndex = 0; threadIndex < _workerThreads.size(); ++threadIndex )
        {
            _workerThreads[threadIndex]->run( [&asyncFn, threadIndex, this]()
            {
                int threadCount = _workerThreads.size();
                // Herbivores
                for ( unsigned int i = 0; i < _herbivores.size(); ++i )
                {
                    // Thread aliasing to distribute the workload.
                    if ( i % threadCount != threadIndex )
                        continue;
                    asyncFn( _herbivores[i].get() );
                }

                // Carnivores
                for ( unsigned int i = 0; i < _carnivores.size(); ++i )
                {
                    // Thread aliasing to distribute the workload.
                    if ( i % threadCount != threadIndex )
                        continue;
                    asyncFn( _carnivores[i].get() );
                }
            } );
        }

        for ( auto it = _workerThreads.begin(); it != _workerThreads.end(); ++it )
        {
            (*it)->waitUntilDone();
        }
    }
#endif
    {
        OpTick op( this );
        runOperation( op );
    }

    // Remove all the dead creatures from the list
#if 0
    CreatureList::iterator i = creatureList_.begin();
    while ( i != creatureList_.end() )
    {
        Creature* creature = *i;
        if ( creature->getHealth() == 0 )
        {
            CreatureList::iterator deadCreature = i++;
            creatureList_.erase( deadCreature );
            delete creature;
        }
        else
        {
            ++i;
        }
    }
#elif 1
    _grasses.erase(
        std::remove_if( _grasses.begin(), _grasses.end(), [] (std::unique_ptr<Grass>& c) { return c->getHealth() == 0; } ),
        _grasses.end());

    _herbivores.erase(
        std::remove_if( _herbivores.begin(), _herbivores.end(), [] (std::unique_ptr<Herbivore>& c) { return c->getHealth() == 0; } ),
        _herbivores.end());

    _carnivores.erase(
        std::remove_if( _carnivores.begin(), _carnivores.end(), [] (std::unique_ptr<Carnivore>& c) { return c->getHealth() == 0; } ),
        _carnivores.end());

#else

    CreatureList::iterator i = creatureList_.begin();
    while ( i != creatureList_.end() )
    {
        Creature* creature = *i;
        if ( creature->getHealth() == 0 )
        {
            creatureList_.erase( i );
            delete creature;
            i = creatureList_.begin();
        }
        else
        {
            ++i;
        }
    }
#endif

    // Check for extinctions
    while ( Carnivore::CREATURE_COUNT < 15 && ( Carnivore::ourEliteGene.get() != nullptr ) )
    {
        // Create based on the elite gene.
        NNGene* pGene = new NNGene( Carnivore::ourEliteGene.get() );
        pGene->mutate();
        addCreature( new Carnivore( pGene ), true );
    }
    while ( Herbivore::CREATURE_COUNT < 15 && ( Herbivore::ourEliteGene.get() != nullptr ) )
    {
        NNGene* pGene = new NNGene( Herbivore::ourEliteGene.get() );
        pGene->mutate();
        addCreature( new Herbivore( pGene ), true );
    }
#if 0
    while ( Grass::CREATURE_COUNT < growthRate_ )
        addCreature( new Grass(), true );
#else
    if ( ( tickCount_ % 80 == 0 ) && ( Grass::CREATURE_COUNT < growthRate_ ) )
        addCreature( new Grass(), true );
#endif
}


void World::runOperation( Operation& operation )
{
    operateOnGrass( [&](Grass* c) -> bool
    {
        c->accept( operation );
        return !operation.mustStop;
    });

    if ( operation.mustStop )
        return;

    operateOnHerbivore( [&](Herbivore* c) -> bool
    {
        c->accept( operation );
        return !operation.mustStop;
    });

    if ( operation.mustStop )
        return;

    operateOnCarnivore( [&](Carnivore* c) -> bool
    {
        if ( !operation.mustStop )
            c->accept( operation );
        return !operation.mustStop;
    });
}

bool World::operateOnGrass( std::function<bool(Grass*)> fn )
{
    for ( auto it = _grasses.begin(); it != _grasses.end(); ++it )
    {
        if ( ! fn( it->get() ) )
            return false;
    }
    return true;
}

bool World::operateOnHerbivore( std::function<bool(Herbivore*)> fn )
{
    for ( auto it = _herbivores.begin(); it != _herbivores.end(); ++it )
    {
        if ( ! fn( it->get() ) )
            return false;
    }
    return true;
}

bool World::operateOnCarnivore( std::function<bool(Carnivore*)> fn )
{
    for ( auto it = _carnivores.begin(); it != _carnivores.end(); ++it )
    {
        if ( ! fn( it->get() ) )
            return false;
    }
    return true;
}

bool World::operateOnAll( std::function<bool(Creature*)> fn )
{
    bool cont = operateOnGrass( fn );
    if ( !cont ) return false;
    cont = operateOnHerbivore( fn );
    if ( !cont ) return false;
    cont = operateOnCarnivore( fn );
    return cont;
}

bool World::operateOnLivingCreature( std::function<bool(LivingCreature*)> fn )
{
    bool cont = operateOnHerbivore( fn );
    if ( !cont ) return false;
    cont = operateOnCarnivore( fn );
    return cont;
}

void World::setMutationLevel( unsigned int level )
{
    NNGene::setMutationLevel( level );
}

unsigned int World::getMutationLevel()
{
    return NNGene::getMutationLevel();
}

void World::toggleCreatureSelection( Creature* creature )
{
    if ( creature != nullptr )
        creature->selected_ = !creature->selected_;
}

void World::clearAllSelection()
{
    operateOnAll( [](Creature* c) -> bool
    {
        c->selected_ = false;
        return true;
    });
}

void World::selectionAll()
{
    operateOnAll( [](Creature* c) -> bool
    {
        c->selected_ = true;
        return true;
    });
}

void World::render( StrangeView* view, bool drawSensors, bool gDrawData )
{
    OperationCreatureRenderer renderer( view, drawSensors, gDrawData );
    runOperation( renderer );
}

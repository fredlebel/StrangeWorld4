#include "Operations/OpAsyncPreTick.h"
#include "Operations/OpAsyncConcentration.h"
#include "Creatures/Carnivore.h"
#include "Creatures/Herbivore.h"
#include "World.h"
#include "WorldSettings.h"
#include "NeuralNetwork/NNGene.h"
#include "FastRand.h"
#include "MathAccel.h"
#include <assert.h>

OpAsyncPreTick::OpAsyncPreTick( World* world )
    : world_( world )
{
}

void OpAsyncPreTick::pushAndTickBrain( LivingCreature* creature )
{
	creature->pushBrainInputs();
	creature->tickBrain();
}

void OpAsyncPreTick::visit_Carnivore( Carnivore* creature )
{
	pushAndTickBrain( creature );
}

void OpAsyncPreTick::visit_Herbivore( Herbivore* creature )
{
	pushAndTickBrain( creature );
}



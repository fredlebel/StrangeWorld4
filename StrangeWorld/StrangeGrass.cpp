#include <math.h>
#include "StrangeGrass.h"
#include "StrangeWorld.h"
#include "StrangeCreatureOperation.h"
#include "StrangeNNGene.h"
#include "FastRand.h"

// Race counter
int StrangeGrass::CREATURE_COUNT = 0;


// Function name   : StrangeGrass::StrangeGrass
// Description     : 
// Return type     : 
StrangeGrass::StrangeGrass()
{
    ++StrangeGrass::CREATURE_COUNT;
    health_ = 1000;
}


// Function name   : StrangeGrass::~StrangeGrass
// Description     : 
// Return type     : 
StrangeGrass::~StrangeGrass()
{
    --StrangeGrass::CREATURE_COUNT;
}


// Function name   : StrangeGrass::initHealth
// Description     : 
// Return type     : void 
void StrangeGrass::initHealth()
{
    health_ = 1000;
}


// Function name   : StrangeGrass::initPosition
// Description     : 
// Return type     : void 
void StrangeGrass::initPosition()
{
    x_ = randomMT() % world_->getWidth();
    y_ = randomMT() % world_->getHeight();
}


// Function name   : StrangeGrass::getWidth
// Description     : 
// Return type     : int 
int StrangeGrass::getRadius()
{
    return std::min( health_, 1000 ) / 160 + 2;
//    return 8;
}


// Function name   : StrangeGrass::die
// Description     : 
// Return type     : void 
void StrangeGrass::die()
{
    isAlive_ = false;
}


// Function name   : StrangeGrass::accept
// Description     : 
// Return type     : bool 
// Argument        : StrangeCreatureOperation* operation
bool StrangeGrass::accept( StrangeCreatureOperation* operation )
{
    operation->visit_Grass( this );
    return true;
}

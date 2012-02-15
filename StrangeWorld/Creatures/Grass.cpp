#include <math.h>
#include "Grass.h"
#include "World.h"
#include "Operations/Operation.h"
#include "NeuralNetwork/NNGene.h"
#include "FastRand.h"

// Race counter
int Grass::CREATURE_COUNT = 0;


// Function name   : Grass::Grass
// Description     : 
// Return type     : 
Grass::Grass()
{
    ++Grass::CREATURE_COUNT;
    health_ = 1000;
}


// Function name   : Grass::~Grass
// Description     : 
// Return type     : 
Grass::~Grass()
{
    --Grass::CREATURE_COUNT;
}


// Function name   : Grass::initHealth
// Description     : 
// Return type     : void 
void Grass::initHealth()
{
    health_ = 1000;
}


// Function name   : Grass::initPosition
// Description     : 
// Return type     : void 
void Grass::initPosition()
{
    x_ = randomMT() % world_->getWidth();
    y_ = randomMT() % world_->getHeight();
}


// Function name   : Grass::getWidth
// Description     : 
// Return type     : int 
double Grass::getRadius()
{
    return std::min( health_, 1000.0 ) / 160 + 2;
//    return 8;
}


// Function name   : Grass::die
// Description     : 
// Return type     : void 
void Grass::die()
{
    isAlive_ = false;
}


// Function name   : Grass::accept
// Description     : 
// Return type     : bool 
// Argument        : Operation* operation
bool Grass::accept( Operation& operation )
{
    operation.visit_Grass( this );
    return true;
}

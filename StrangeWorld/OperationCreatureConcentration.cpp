#include "OperationCreatureConcentration.h"
#include "MathAccel.h"


// Function name   : OperationCreatureConcentration::OperationCreatureConcentration
// Description     : 
// Return type     : 
// Argument        : StrangeWorld* world
// Argument        : StrangeCreature* ignore
// Argument        : int x
// Argument        : int y
// Argument        : int range
OperationCreatureConcentration::OperationCreatureConcentration( StrangeWorld* world, StrangeCreature* ignore, int x, int y, int range )
    : world_( world )
    , ignore_( ignore )
    , detectorX_( x )
    , detectorY_( y )
    , range_( range )
    , carnivoreConcentration( 0 )
    , herbivoreConcentration( 0 )
    , grassConcentration( 0 )
{
    creatureX_ = ignore_->getX();
    creatureY_ = ignore_->getY();
}


// Function name   : OperationCreatureConcentration::~OperationCreatureConcentration
// Description     : 
// Return type     : 
OperationCreatureConcentration::~OperationCreatureConcentration()
{
}


// Function name   : OperationCreatureConcentration::getConcentration
// Description     : 
// Return type     : int 
// Argument        : StrangeCreature* creature
int OperationCreatureConcentration::getConcentration( StrangeCreature* creature )
{
    if ( creature == ignore_ )
        return 0;
    int cx = creature->getX();
    int cy = creature->getY();
    // Get the distance between the detector and the creature
    int distance = MathAccel::dist( 
        cx, cy,detectorX_, detectorY_ );
    // If the creature falls in the range of the detector
    if ( distance < range_ )
    {
        // Get the distance of the creature to the target
        distance = MathAccel::dist( 
            cx, cy, creatureX_, creatureY_ );

        /*
        // Calculate how faint is the creature
        int fullRange = range_ << 1;
        int strength = fullRange - distance;
        */
        // The strength is between 0 and 100
        int fullRange = range_ << 1;
        int strength = (int)((( fullRange ) - distance) / (double)fullRange * 100);
        return strength;
    }
    return 0;
}


// Function name   : OperationCreatureConcentration::visit_Carnivore
// Description     : 
// Return type     : void 
// Argument        : StrangeCarnivore* creature
void OperationCreatureConcentration::visit_Carnivore( StrangeCarnivore* creature )
{
    int c = getConcentration( creature );
    if ( c > 0 )
        carnivoreConcentration += c;
}


// Function name   : OperationCreatureConcentration::visit_Grass
// Description     : 
// Return type     : void 
// Argument        : StrangeGrass* creature
void OperationCreatureConcentration::visit_Grass( StrangeGrass* creature )
{
    int c = getConcentration( creature );
    if ( c > 0 )
    {
        grassConcentration += c;
    }
}


// Function name   : OperationCreatureConcentration::visit_Herbivore
// Description     : 
// Return type     : void 
// Argument        : StrangeHerbivore* creature
void OperationCreatureConcentration::visit_Herbivore( StrangeHerbivore* creature )
{
    int c = getConcentration( creature );
    if ( c > 0 )
        herbivoreConcentration += c;
}



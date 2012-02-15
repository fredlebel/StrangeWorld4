#include "Operations/OpAsyncConcentration.h"
#include "Creatures/Carnivore.h"
#include "Creatures/Herbivore.h"
#include "Creatures/Grass.h"
#include "World.h"
#include "MathAccel.h"


// Function name   : OpAsyncConcentration::OpAsyncConcentration
// Description     : 
// Return type     : 
// Argument        : World* world
// Argument        : Creature* ignore
// Argument        : int x
// Argument        : int y
// Argument        : int range
OpAsyncConcentration::OpAsyncConcentration( World* world, Creature* ignore, double x, double y, double range )
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


// Function name   : OpAsyncConcentration::~OpAsyncConcentration
// Description     : 
// Return type     : 
OpAsyncConcentration::~OpAsyncConcentration()
{
}


// Function name   : OpAsyncConcentration::getConcentration
// Description     : 
// Return type     : int 
// Argument        : Creature* creature
double OpAsyncConcentration::getConcentration( Creature* creature )
{
    if ( creature == ignore_ )
        return 0;
    double cx = creature->getX();
    double cy = creature->getY();
    // Get the distance between the detector and the creature
    double distance = MathAccel::dist( 
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
        double fullRange = range_ * 2;
        double strength = (fullRange - distance) / fullRange * 100;
        return strength;
    }
    return 0;
}


// Function name   : OpAsyncConcentration::visit_Carnivore
// Description     : 
// Return type     : void 
// Argument        : Carnivore* creature
void OpAsyncConcentration::visit_Carnivore( Carnivore* creature )
{
    double c = getConcentration( creature );
    if ( c > 0 )
        carnivoreConcentration += c;
}


// Function name   : OpAsyncConcentration::visit_Grass
// Description     : 
// Return type     : void 
// Argument        : Grass* creature
void OpAsyncConcentration::visit_Grass( Grass* creature )
{
    double c = getConcentration( creature );
    if ( c > 0 )
    {
        grassConcentration += c;
    }
}


// Function name   : OpAsyncConcentration::visit_Herbivore
// Description     : 
// Return type     : void 
// Argument        : Herbivore* creature
void OpAsyncConcentration::visit_Herbivore( Herbivore* creature )
{
    double c = getConcentration( creature );
    if ( c > 0 )
        herbivoreConcentration += c;
}



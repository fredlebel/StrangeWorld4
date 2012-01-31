#include "Operations/OpConcentration.h"
#include "Creatures/Carnivore.h"
#include "Creatures/Herbivore.h"
#include "Creatures/Grass.h"
#include "MathAccel.h"


// Function name   : OpConcentration::OpConcentration
// Description     : 
// Return type     : 
// Argument        : World* world
// Argument        : Creature* ignore
// Argument        : int x
// Argument        : int y
// Argument        : int range
OpConcentration::OpConcentration( World* world, Creature* ignore, double x, double y, double range )
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


// Function name   : OpConcentration::~OpConcentration
// Description     : 
// Return type     : 
OpConcentration::~OpConcentration()
{
}


// Function name   : OpConcentration::getConcentration
// Description     : 
// Return type     : int 
// Argument        : Creature* creature
double OpConcentration::getConcentration( Creature* creature )
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


// Function name   : OpConcentration::visit_Carnivore
// Description     : 
// Return type     : void 
// Argument        : Carnivore* creature
void OpConcentration::visit_Carnivore( Carnivore* creature )
{
    double c = getConcentration( creature );
    if ( c > 0 )
        carnivoreConcentration += c;
}


// Function name   : OpConcentration::visit_Grass
// Description     : 
// Return type     : void 
// Argument        : Grass* creature
void OpConcentration::visit_Grass( Grass* creature )
{
    double c = getConcentration( creature );
    if ( c > 0 )
    {
        grassConcentration += c;
    }
}


// Function name   : OpConcentration::visit_Herbivore
// Description     : 
// Return type     : void 
// Argument        : Herbivore* creature
void OpConcentration::visit_Herbivore( Herbivore* creature )
{
    double c = getConcentration( creature );
    if ( c > 0 )
        herbivoreConcentration += c;
}



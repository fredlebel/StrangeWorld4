#include "Creature.h"

#include "Operations/Operation.h"
#include "World.h"



// Function name   : Creature::Creature
// Description     : 
// Return type     : 
Creature::Creature()
    : isAlive_( true )
    , world_( nullptr )
    , age_( 0 )
    , health_( 500 )
    , x_( -1 ), y_( -1 )
    , selected_( false )
{
}


// Function name   : Creature::~Creature
// Description     : 
// Return type     : 
Creature::~Creature()
{
}

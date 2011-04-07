#include "StrangeCreature.h"

#include "StrangeCreatureOperation.h"
#include "StrangeWorld.h"



// Function name   : StrangeCreature::StrangeCreature
// Description     : 
// Return type     : 
StrangeCreature::StrangeCreature()
    : isAlive_( true )
    , world_( NULL )
    , age_( 0 )
    , health_( 500 )
    , x_( -1 ), y_( -1 )
    , selected_( false )
{
}


// Function name   : StrangeCreature::~StrangeCreature
// Description     : 
// Return type     : 
StrangeCreature::~StrangeCreature()
{
}

#include "Operation.h"
#include "Creatures/Carnivore.h"
#include "Creatures/Herbivore.h"
#include "Creatures/Grass.h"

// Function name   : Operation::Operation
// Description     : 
// Return type     : 
Operation::Operation() : mustStop( false )
{
}


// Function name   : Operation::~Operation
// Description     : 
// Return type     : 
Operation::~Operation()
{
}


// Function name   : Operation::visit_creature
// Description     : 
// Return type     : void 
// Argument        : Creature* creature
void Operation::visit_creature( Creature* )
{
}


// Function name   : Operation::visit_Carnivore
// Description     : 
// Return type     : void 
// Argument        : Carnivore* creature
void Operation::visit_Carnivore( Carnivore* creature )
{
    visit_creature( creature );
}


// Function name   : Operation::visit_Grass
// Description     : 
// Return type     : void 
// Argument        : Grass* creature
void Operation::visit_Grass( Grass* creature )
{
    visit_creature( creature );
}


// Function name   : Operation::visit_Herbivore
// Description     : 
// Return type     : void 
// Argument        : Herbivore* creature
void Operation::visit_Herbivore( Herbivore* creature )
{
    visit_creature( creature );
}



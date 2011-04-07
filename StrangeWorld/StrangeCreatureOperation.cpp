#include "StrangeCreatureOperation.h"


// Function name   : StrangeCreatureOperation::StrangeCreatureOperation
// Description     : 
// Return type     : 
StrangeCreatureOperation::StrangeCreatureOperation() : mustStop( false )
{
}


// Function name   : StrangeCreatureOperation::~StrangeCreatureOperation
// Description     : 
// Return type     : 
StrangeCreatureOperation::~StrangeCreatureOperation()
{
}


// Function name   : StrangeCreatureOperation::visit_creature
// Description     : 
// Return type     : void 
// Argument        : StrangeCreature* creature
void StrangeCreatureOperation::visit_creature( StrangeCreature* )
{
}


// Function name   : StrangeCreatureOperation::visit_Carnivore
// Description     : 
// Return type     : void 
// Argument        : StrangeCarnivore* creature
void StrangeCreatureOperation::visit_Carnivore( StrangeCarnivore* creature )
{
    visit_creature( creature );
}


// Function name   : StrangeCreatureOperation::visit_Grass
// Description     : 
// Return type     : void 
// Argument        : StrangeGrass* creature
void StrangeCreatureOperation::visit_Grass( StrangeGrass* creature )
{
    visit_creature( creature );
}


// Function name   : StrangeCreatureOperation::visit_Herbivore
// Description     : 
// Return type     : void 
// Argument        : StrangeHerbivore* creature
void StrangeCreatureOperation::visit_Herbivore( StrangeHerbivore* creature )
{
    visit_creature( creature );
}



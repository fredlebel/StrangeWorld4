#include "OperationCreatureHitTest.h"
#include "MathAccel.h"


// Function name   : OperationCreatureHitTest::OperationCreatureHitTest
// Description     : 
// Return type     : 
// Argument        : int x
// Argument        : int y
// Argument        : int width
// Argument        : StrangeWorld* world
// Argument        : StrangeCreature* ignore
OperationCreatureHitTest::OperationCreatureHitTest( int x, int y, StrangeCreature* creature, WantToHit wth )
    : creature_( creature )
    , creatureHit( NULL )
    , x_( x )
    , y_( y )
    , wth_( wth )
{
}


// Function name   : OperationCreatureHitTest::~OperationCreatureHitTest
// Description     : 
// Return type     : 
OperationCreatureHitTest::~OperationCreatureHitTest()
{
}


// Function name   : OperationCreatureHitTest::visit_creature
// Description     : 
// Return type     : void 
// Argument        : StrangeCreature* creature
void OperationCreatureHitTest::checkHit( StrangeCreature* target, WantToHit hit )
{
    if ( creature_ == NULL )
    {
        int cx      = target->getX();
        int cy      = target->getY();
        int radius  = target->getRadius();

        /*if (( x_ >= ( cx - radius ) ) &&
            ( x_ <= ( cx + radius ) ) &&
            ( y_ >= ( cy - radius ) ) &&
            ( y_ <= ( cy + radius ) ) )*/
        {
            int distance = MathAccel::dist( cx, cy, x_, y_ );
            if ( distance <= radius &&
                ( wth_ == HitAll || wth_ == hit ) )
            {
                creatureHit = target;
                mustStop = true;
            }
        }
    }
    else if ( target != creature_ )
    {
        int cx      = target->getX();
        int cy      = target->getY();
        int tradius = target->getRadius();
        int radius  = creature_->getRadius();

        /*if (( ( x_ + radius ) >= ( cx - tradius ) ) &&
            ( ( x_ - radius ) <= ( cx + tradius ) ) &&
            ( ( y_ + radius ) >= ( cy - tradius ) ) &&
            ( ( y_ - radius ) <= ( cy + tradius ) ) )*/
        {
            int distance = MathAccel::dist( cx, cy, x_, y_ );
            if ( distance <= (radius + tradius) &&
                ( wth_ == HitAll || wth_ == hit ) )
            {
                creatureHit = target;
                mustStop = true;
            }
        }
    }
}

void OperationCreatureHitTest::visit_Carnivore( StrangeCarnivore* target )
{
    checkHit( target, HitCarnivore );
}

void OperationCreatureHitTest::visit_Herbivore( StrangeHerbivore* target )
{
    checkHit( target, HitHerbivore );
}

void OperationCreatureHitTest::visit_Grass( StrangeGrass* target )
{
    checkHit( target, HitGrass );
}

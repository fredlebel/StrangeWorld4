#include "OpAsyncHitTest.h"
#include "Creatures/Carnivore.h"
#include "Creatures/Herbivore.h"
#include "Creatures/Grass.h"
#include "MathAccel.h"


// Function name   : OpAsyncHitTest::OpAsyncHitTest
// Description     : 
// Return type     : 
// Argument        : int x
// Argument        : int y
// Argument        : int width
// Argument        : World* world
// Argument        : Creature* ignore
OpAsyncHitTest::OpAsyncHitTest( double x, double y, Creature* creature, WantToHit wth )
    : creature_( creature )
    , creatureHit( 0 )
    , x_( x )
    , y_( y )
    , wth_( wth )
{
}


// Function name   : OpAsyncHitTest::~OpAsyncHitTest
// Description     : 
// Return type     : 
OpAsyncHitTest::~OpAsyncHitTest()
{
}


// Function name   : OpAsyncHitTest::visit_creature
// Description     : 
// Return type     : void 
// Argument        : Creature* creature
void OpAsyncHitTest::checkHit( Creature* target, WantToHit hit )
{
    if ( creature_ == 0 )
    {
        double cx      = target->getX();
        double cy      = target->getY();
        double radius  = target->getRadius();

        /*if (( x_ >= ( cx - radius ) ) &&
            ( x_ <= ( cx + radius ) ) &&
            ( y_ >= ( cy - radius ) ) &&
            ( y_ <= ( cy + radius ) ) )*/
        {
            double distance = MathAccel::dist( cx, cy, x_, y_ );
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
        double cx      = target->getX();
        double cy      = target->getY();
        double tradius = target->getRadius();
        double radius  = creature_->getRadius();

        /*if (( ( x_ + radius ) >= ( cx - tradius ) ) &&
            ( ( x_ - radius ) <= ( cx + tradius ) ) &&
            ( ( y_ + radius ) >= ( cy - tradius ) ) &&
            ( ( y_ - radius ) <= ( cy + tradius ) ) )*/
        {
            double distance = MathAccel::dist( cx, cy, x_, y_ );
            if ( distance <= (radius + tradius) &&
                ( wth_ == HitAll || wth_ == hit ) )
            {
                creatureHit = target;
                mustStop = true;
            }
        }
    }
}

void OpAsyncHitTest::visit_Carnivore( Carnivore* target )
{
    checkHit( target, HitCarnivore );
}

void OpAsyncHitTest::visit_Herbivore( Herbivore* target )
{
    checkHit( target, HitHerbivore );
}

void OpAsyncHitTest::visit_Grass( Grass* target )
{
    checkHit( target, HitGrass );
}

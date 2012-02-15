#include "CollisionDetection.h"
#include "Creatures/Creature.h"
#include "MathAccel.h"


bool collisionDetect( Creature* c1, Creature* c2 )
{
    double distance = MathAccel::dist( c1->getX(), c1->getY(), c2->getX(), c2->getY() );
    return distance <= c1->getRadius() + c2->getRadius();
}

bool hitTest( Creature* c, int x, int y )
{
    double distance = MathAccel::dist( c->getX(), c->getY(), x, y );
    return distance <= c->getRadius();
}


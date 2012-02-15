#ifndef _CollisionDetection_H_INCLUDED_
#define _CollisionDetection_H_INCLUDED_

class Creature;

bool collisionDetect( Creature* c1, Creature* c2 );
bool hitTest( Creature* c, int x, int y );

#endif //_CollisionDetection_H_INCLUDED_

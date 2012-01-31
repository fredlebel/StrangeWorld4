#ifndef _STRANGEGRASS_H_INCLUDED_
#define _STRANGEGRASS_H_INCLUDED_

#include "Creature.h"

class World;
class StrangeGeneCollection;

class Grass : public Creature
{
public:
    // Race counter
    static int CREATURE_COUNT;
    
    Grass();
    virtual ~Grass();
    
    // Overloaded
    virtual void initHealth();
    virtual void initPosition();
    virtual double getRadius();
    virtual void die();

    virtual bool accept(Operation* operation);
};

#endif // _STRANGEGRASS_H_INCLUDED_
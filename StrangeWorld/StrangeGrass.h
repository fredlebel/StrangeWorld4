#ifndef _STRANGEGRASS_H_INCLUDED_
#define _STRANGEGRASS_H_INCLUDED_

#include "StrangeCreature.h"

class StrangeWorld;
class StrangeGeneCollection;

class StrangeGrass : public StrangeCreature
{
public:
    // Race counter
    static int CREATURE_COUNT;
    
    StrangeGrass();
    virtual ~StrangeGrass();
    
    // Overloaded
    virtual void initHealth();
    virtual void initPosition();
    virtual int getRadius();
    virtual void die();

    virtual bool accept(StrangeCreatureOperation* operation);
};

#endif // _STRANGEGRASS_H_INCLUDED_
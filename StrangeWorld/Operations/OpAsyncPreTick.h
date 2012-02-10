#ifndef _OpAsyncPreTick_H_INCLUDED_
#define _OpAsyncPreTick_H_INCLUDED_

#include "Operations/Operation.h"

class World;
class LivingCreature;

class OpAsyncPreTick : public Operation
{
public:
    OpAsyncPreTick(World* world);

private:
    World* world_;
	void pushAndTickBrain( LivingCreature* creature );

public:
    virtual void visit_Carnivore(Carnivore* creature);
    virtual void visit_Herbivore(Herbivore* creature);
};

#endif //_OpAsyncPreTick_H_INCLUDED_
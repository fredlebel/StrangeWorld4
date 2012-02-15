#ifndef _OpTick_H_INCLUDED_
#define _OpTick_H_INCLUDED_

#include "Operations/Operation.h"

class World;
class LivingCreature;

class OpTick : public Operation
{
public:
    OpTick(World* world);
    virtual ~OpTick();

private:
    World* world_;
    // Whether this time the creature loses one health
    bool hurtCarnivore_;
    bool hurtHerbivore_;

public:
    virtual void visit_Carnivore(Carnivore* creature);
    virtual void visit_Grass(Grass* creature);
    virtual void visit_Herbivore(Herbivore* creature);
};

#endif //_OpTick_H_INCLUDED_

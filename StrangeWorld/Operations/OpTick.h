#ifndef _OPERATIONCREATURETICK_H_INCLUDED_
#define _OPERATIONCREATURETICK_H_INCLUDED_

#include "Operations/Operation.h"
#include "Operations/OpHitTest.h"

class World;
class LivingCreature;

class OperationCreatureTick : public Operation
{
private:    // CTOR/DTOR
protected:  // CTOR/DTOR
public:     // CTOR/DTOR
    OperationCreatureTick(World* world);
    virtual ~OperationCreatureTick();

private:    // Attributes
protected:  // Attributes
    World* world_;
    // Whether this time the creature loses one health
    bool hurtCarnivore_;
    bool hurtHerbivore_;
public:     // Attributes

private:    // Functions
    void basicPhase(Operation* op, LivingCreature* creature, OpHitTest::WantToHit wth);
protected:  // Functions
public:     // Functions
    virtual void visit_Carnivore(Carnivore* creature);
    virtual void visit_Grass(Grass* creature);
    virtual void visit_Herbivore(Herbivore* creature);
};

#endif //_OPERATIONCREATURETICK_H_INCLUDED_
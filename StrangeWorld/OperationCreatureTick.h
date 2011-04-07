#ifndef _OPERATIONCREATURETICK_H_INCLUDED_
#define _OPERATIONCREATURETICK_H_INCLUDED_

#include "StrangeCreatureOperation.h"
#include "OperationCreatureHitTest.h"

class StrangeWorld;

class OperationCreatureTick : public StrangeCreatureOperation
{
private:    // CTOR/DTOR
protected:  // CTOR/DTOR
public:     // CTOR/DTOR
    OperationCreatureTick(StrangeWorld* world);
    virtual ~OperationCreatureTick();

private:    // Attributes
protected:  // Attributes
    StrangeWorld* world_;
    // Whether this time the creature loses one health
    bool hurtCarnivore_;
    bool hurtHerbivore_;
public:     // Attributes

private:    // Functions
    void basicPhase(StrangeCreatureOperation* op, StrangeLivingCreature* creature, OperationCreatureHitTest::WantToHit wth);
protected:  // Functions
public:     // Functions
    virtual void visit_Carnivore(StrangeCarnivore* creature);
    virtual void visit_Grass(StrangeGrass* creature);
    virtual void visit_Herbivore(StrangeHerbivore* creature);
};

#endif //_OPERATIONCREATURETICK_H_INCLUDED_
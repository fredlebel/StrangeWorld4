#ifndef _OPERATIONCREATUREHITTEST_H_INCLUDED_
#define _OPERATIONCREATUREHITTEST_H_INCLUDED_

#include "StrangeCreatureOperation.h"

class OperationCreatureHitTest 
    : public StrangeCreatureOperation
{
private:    // CTOR/DTOR
protected:  // CTOR/DTOR
public:     // CTOR/DTOR
    enum WantToHit
    {
        HitAll,
        HitHerbivore,
        HitCarnivore,
        HitGrass
    };
    OperationCreatureHitTest( int x, int y, StrangeCreature* creature = NULL, WantToHit wth = HitAll );
    virtual ~OperationCreatureHitTest();

private:    // Attributes
protected:  // Attributes
    StrangeCreature* creature_;
    int x_;
    int y_;
    WantToHit wth_;
public:     // Attributes
    StrangeCreature* creatureHit;

private:    // Functions
    void checkHit( StrangeCreature* target, WantToHit hit );
protected:  // Functions
public:     // Functions
    virtual void visit_Carnivore(StrangeCarnivore* creature);
    virtual void visit_Grass(StrangeGrass* creature);
    virtual void visit_Herbivore(StrangeHerbivore* creature);
};

#endif //_OPERATIONCREATUREHITTEST_H_INCLUDED_
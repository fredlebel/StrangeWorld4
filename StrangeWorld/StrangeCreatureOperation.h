#ifndef _StrangeCreatureOperation_h_included_
#define _StrangeCreatureOperation_h_included_

#include "StrangeCarnivore.h"
#include "StrangeHerbivore.h"
#include "StrangeGrass.h"

class StrangeCreatureOperation
{
private:    // CTOR/DTOR
protected:  // CTOR/DTOR
public:     // CTOR/DTOR
    StrangeCreatureOperation();
    virtual ~StrangeCreatureOperation();

private:    // Attributes
protected:  // Attributes
public:     // Attributes
    bool mustStop;

private:    // Functions
protected:  // Functions
public:     // Functions
    virtual void visit_creature(StrangeCreature* creature);

    virtual void visit_Carnivore(StrangeCarnivore* creature);
    virtual void visit_Grass(StrangeGrass* creature);
    virtual void visit_Herbivore(StrangeHerbivore* creature);
};

#endif //_StrangeCreatureOperation_h_included_
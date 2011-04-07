#ifndef _OPERATIONCREATURECONCENTRATION_H_INCLUDED_
#define _OPERATIONCREATURECONCENTRATION_H_INCLUDED_

#include "StrangeCreatureOperation.h"

class OperationCreatureConcentration
    : public StrangeCreatureOperation
{
private:    // CTOR/DTOR
protected:  // CTOR/DTOR
public:     // CTOR/DTOR
    OperationCreatureConcentration( StrangeWorld* world, StrangeCreature* ignore, int x, int y, int range );
    virtual ~OperationCreatureConcentration();

private:    // Attributes
protected:  // Attributes
    StrangeCreature* ignore_;
    StrangeWorld* world_;
    int detectorX_;
    int detectorY_;
    int creatureX_;
    int creatureY_;
    int range_;
public:     // Attributes
    int carnivoreConcentration;
    int herbivoreConcentration;
    int grassConcentration;

private:    // Functions
    int getConcentration( StrangeCreature* creature );
protected:  // Functions
public:     // Functions
    virtual void visit_Carnivore( StrangeCarnivore* creature );
    virtual void visit_Grass( StrangeGrass* creature );
    virtual void visit_Herbivore( StrangeHerbivore* creature );
};

#endif //_OPERATIONCREATURECONCENTRATION_H_INCLUDED_

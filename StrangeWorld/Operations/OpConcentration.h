#ifndef _OPERATIONCREATURECONCENTRATION_H_INCLUDED_
#define _OPERATIONCREATURECONCENTRATION_H_INCLUDED_

#include "Operation.h"
class World;
class Creature;

class OpConcentration
    : public Operation
{
private:    // CTOR/DTOR
protected:  // CTOR/DTOR
public:     // CTOR/DTOR
    OpConcentration( World* world, Creature* ignore, double x, double y, double range );
    virtual ~OpConcentration();

private:    // Attributes
protected:  // Attributes
    Creature* ignore_;
    World* world_;
    double detectorX_;
    double detectorY_;
    double creatureX_;
    double creatureY_;
    double range_;
public:     // Attributes
    double carnivoreConcentration;
    double herbivoreConcentration;
    double grassConcentration;

private:    // Functions
    double getConcentration( Creature* creature );
protected:  // Functions
public:     // Functions
    virtual void visit_Carnivore( Carnivore* creature );
    virtual void visit_Grass( Grass* creature );
    virtual void visit_Herbivore( Herbivore* creature );
};

#endif //_OPERATIONCREATURECONCENTRATION_H_INCLUDED_

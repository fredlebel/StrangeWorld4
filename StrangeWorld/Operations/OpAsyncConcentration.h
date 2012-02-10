#ifndef _OpAsyncConcentration_H_INCLUDED_
#define _OpAsyncConcentration_H_INCLUDED_

#include "Operation.h"
class World;
class Creature;

class OpAsyncConcentration
    : public Operation
{
private:    // CTOR/DTOR
protected:  // CTOR/DTOR
public:     // CTOR/DTOR
    OpAsyncConcentration( World* world, Creature* ignore, double x, double y, double range );
    virtual ~OpAsyncConcentration();

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

#endif //_OpAsyncConcentration_H_INCLUDED_

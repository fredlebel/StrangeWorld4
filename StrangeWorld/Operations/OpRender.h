#ifndef _OperationCreatureRenderer_h_included_
#define _OperationCreatureRenderer_h_included_

#include "Operation.h"

class StrangeView;
class LivingCreature;

class OperationCreatureRenderer 
    : public Operation
{
private:    // CTOR/DTOR
protected:  // CTOR/DTOR
public:     // CTOR/DTOR
    OperationCreatureRenderer( StrangeView* view, bool drawSensors, bool drawData );
    virtual ~OperationCreatureRenderer();

private:    // Attributes
protected:  // Attributes
    StrangeView* view_;
    bool drawSensors_;
    bool drawData_;
public:     // Attributes

private:    // Functions
    void renderLivingCreature( LivingCreature* creature );
protected:  // Functions
public:     // Functions
    virtual void visit_Carnivore( Carnivore* creature );
    virtual void visit_Grass( Grass* creature );
    virtual void visit_Herbivore( Herbivore* creature );
};

#endif //_OperationCreatureRenderer_h_included_
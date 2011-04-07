#ifndef _OperationCreatureRenderer_h_included_
#define _OperationCreatureRenderer_h_included_

#include "StrangeCreatureOperation.h"

class StrangeView;

class OperationCreatureRenderer 
    : public StrangeCreatureOperation
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
    void renderLivingCreature( StrangeLivingCreature* creature );
protected:  // Functions
public:     // Functions
    virtual void visit_Carnivore( StrangeCarnivore* creature );
    virtual void visit_Grass( StrangeGrass* creature );
    virtual void visit_Herbivore( StrangeHerbivore* creature );
};

#endif //_OperationCreatureRenderer_h_included_
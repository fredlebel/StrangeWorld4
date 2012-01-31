#ifndef _OPERATIONCREATUREHITTEST_H_INCLUDED_
#define _OPERATIONCREATUREHITTEST_H_INCLUDED_

#include "Operation.h"

class OpHitTest 
    : public Operation
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
    OpHitTest( double x, double y, Creature* creature = 0, WantToHit wth = HitAll );
    virtual ~OpHitTest();

private:    // Attributes
protected:  // Attributes
    Creature* creature_;
    double x_;
    double y_;
    WantToHit wth_;
public:     // Attributes
    Creature* creatureHit;

private:    // Functions
    void checkHit( Creature* target, WantToHit hit );
protected:  // Functions
public:     // Functions
    virtual void visit_Carnivore(Carnivore* creature);
    virtual void visit_Grass(Grass* creature);
    virtual void visit_Herbivore(Herbivore* creature);
};

#endif //_OPERATIONCREATUREHITTEST_H_INCLUDED_
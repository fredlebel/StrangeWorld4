#ifndef _StrangeCreatureOperation_h_included_
#define _StrangeCreatureOperation_h_included_

class Creature;
class Carnivore;
class Herbivore;
class Grass;

class Operation
{
private:    // CTOR/DTOR
protected:  // CTOR/DTOR
public:     // CTOR/DTOR
    Operation();
    virtual ~Operation();

private:    // Attributes
protected:  // Attributes
public:     // Attributes
    bool mustStop;

private:    // Functions
protected:  // Functions
public:     // Functions
    virtual void visit_creature(Creature* creature);

    virtual void visit_Carnivore(Carnivore* creature);
    virtual void visit_Grass(Grass* creature);
    virtual void visit_Herbivore(Herbivore* creature);
};

#endif //_StrangeCreatureOperation_h_included_
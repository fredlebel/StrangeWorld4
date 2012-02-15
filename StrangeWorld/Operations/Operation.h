#ifndef _StrangeCreatureOperation_h_included_
#define _StrangeCreatureOperation_h_included_

class Creature;
class Carnivore;
class Herbivore;
class Grass;

class Operation
{
public:
    Operation();
    virtual ~Operation();

public:
    bool mustStop;

public:
    virtual void visit_creature(Creature* creature);

    virtual void visit_Carnivore(Carnivore* creature);
    virtual void visit_Grass(Grass* creature);
    virtual void visit_Herbivore(Herbivore* creature);
};

#endif //_StrangeCreatureOperation_h_included_
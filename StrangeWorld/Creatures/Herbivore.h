#ifndef _STRANGEHERBIVORE_H_INCLUDED_
#define _STRANGEHERBIVORE_H_INCLUDED_

#include "LivingCreature.h"

class World;
class Operation;
class StrangeGene;
class NNGene;

class Herbivore : public LivingCreature
{
public:
    // Race counter
    static int CREATURE_COUNT;
    static int ourAverageAge;
    static int ourAverageSpawnCount;
    static int ourAverageFeedCount;
    static int ourDeathCount;
    // Elite gene, usually the latest one
    static std::auto_ptr<NNGene> ourEliteGene;
    
    Herbivore(NNGene* aGene);
    virtual ~Herbivore();
    
    // Overloaded
    virtual double getRadius();
    virtual void die();

    virtual bool accept(Operation* operation);
};

#endif // _STRANGEHERBIVORE_H_INCLUDED_
#ifndef _STRANGECARNIVORE_H_INCLUDED_
#define _STRANGECARNIVORE_H_INCLUDED_

#include "LivingCreature.h"

class World;
class Operation;
class StrangeGene;
class NNGene;

class Carnivore : public LivingCreature
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
    
    Carnivore(NNGene* aGene);
    virtual ~Carnivore();
    
    // Overloaded
    virtual double getRadius();
    virtual void die();

    virtual bool accept(Operation* operation);
};

#endif // _STRANGECARNIVORE_H_INCLUDED_
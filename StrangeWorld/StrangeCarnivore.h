#ifndef _STRANGECARNIVORE_H_INCLUDED_
#define _STRANGECARNIVORE_H_INCLUDED_

#include "StrangeLivingCreature.h"

class StrangeWorld;
class StrangeCreatureOperation;
class StrangeGene;
class StrangeNNGene;

class StrangeCarnivore : public StrangeLivingCreature
{
public:
    // Race counter
    static int CREATURE_COUNT;
    static int ourAverageAge;
    static int ourAverageSpawnCount;
    static int ourAverageFeedCount;
    static int ourDeathCount;
    // Elite gene, usually the latest one
    static StrangeNNGene* ourEliteGene;
    
    StrangeCarnivore(StrangeNNGene* aGene);
    virtual ~StrangeCarnivore();
    
    // Overloaded
    virtual int getRadius();
    virtual void die();

    virtual bool accept(StrangeCreatureOperation* operation);
};

#endif // _STRANGECARNIVORE_H_INCLUDED_
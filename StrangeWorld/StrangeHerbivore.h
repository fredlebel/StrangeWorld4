#ifndef _STRANGEHERBIVORE_H_INCLUDED_
#define _STRANGEHERBIVORE_H_INCLUDED_

#include "StrangeLivingCreature.h"

class StrangeWorld;
class StrangeCreatureOperation;
class StrangeGene;
class StrangeNNGene;

class StrangeHerbivore : public StrangeLivingCreature
{
public:
    // Race counter
    static int CREATURE_COUNT;
    static int ourAverageAge;
    static int ourAverageSpawnCount;
    static int ourAverageFeedCount;
    static int ourDeathCount;
    // Elite gene, usually the latest one
    static std::auto_ptr<StrangeNNGene> ourEliteGene;
    
    StrangeHerbivore(StrangeNNGene* aGene);
    virtual ~StrangeHerbivore();
    
    // Overloaded
    virtual int getRadius();
    virtual void die();

    virtual bool accept(StrangeCreatureOperation* operation);
};

#endif // _STRANGEHERBIVORE_H_INCLUDED_
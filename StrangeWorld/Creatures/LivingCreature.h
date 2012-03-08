#ifndef _StrangeLivingCreature_h_included_
#define _StrangeLivingCreature_h_included_

#include "Creature.h"
#include <memory>

class NeuralNetwork;
class NNGene;

class LivingCreature : public Creature
{
public:
    // Neural network input addresses
    enum NeuralInputs
    { 
        // Enviromnent information
        NNI_L_CARN = 0,     // Left - Carnivore
        NNI_L_HERB,         // Left - Herbivore
        NNI_L_GRAS,         // Left - Grass
        //----------------------------------------------------
        NNI_R_CARN,         // Right - Carnivore
        NNI_R_HERB,         // Right - Herbivore
        NNI_R_GRAS,         // Right - Grass

        // Personal information
        NNI_HEALTH,         // Health
        NNI_EYE_RADIUS,     // Radius of the eyes

        NNI_BODY_RADIUS,    // Radius of the body

        // Neural feedback
        NNI_FB_DISP,        // Neural feedback - Displacement
        NNI_FB_ROTA,        // Neural feedback - Rotation
        NNI_FB_EYE_RADIUS,  // Neural feedback - Radius of vision
        NNI_FB_BODY_RADIUS, // Neural feedback - Action
        NNI_COUNT,
    };

    // Neural network output addresses
    enum NeuralOutputs
    { 
        NNO_DISP = 0,       // Displacement
        NNO_ROTA,           // Rotation
        NNO_EYE_RADIUS,     // Radius of vision
        NNO_BODY_RADIUS,    // Radius of body
    };

    // Counts the number of times the creature has spawned.
    int spawnCount_;
    // Counts the number of times the creature has fed.
    int feedCount_;

    double angle_;          // Angle the creature is pointing
    double eyeRadius_;      // Radius of vision for the eyes
    double bodyRadius_;     // Radius of vision for the eyes

protected:
    std::auto_ptr<NeuralNetwork> brain_;
    std::auto_ptr<NNGene> gene_;
    Creature* _contact;

public:
    LivingCreature( NNGene* aGene );
    virtual ~LivingCreature();


    NNGene* getGene();

    void tickBrain();
    void pushBrainInputs();
    virtual void checkContactWithEdible() = 0;
    Creature* getContact();

    // Returns energy used
    double popBrainOutputs();
};


#endif //_StrangeLivingCreature_h_included_
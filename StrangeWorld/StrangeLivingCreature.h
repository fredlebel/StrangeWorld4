#ifndef _StrangeLivingCreature_h_included_
#define _StrangeLivingCreature_h_included_

#include "StrangeCreature.h"

class StrangeNeuralNetwork;
class StrangeNNGene;

class StrangeLivingCreature : public StrangeCreature
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
        NNI_EYE_ANGLE,      // Angle between the eyes
        NNI_EYE_RADIUS,     // Radius of the eyes

        NNI_BODY_RADIUS,    // Radius of the body

        // Neural feedback
        NNI_FB_DISP,        // Neural feedback - Displacement
        NNI_FB_ROTA,        // Neural feedback - Rotation
        NNI_FB_EYE_RADIUS,  // Neural feedback - Radius of vision
        NNI_FB_FOCUS,       // Neural feedback - Angle between the eyes
        NNI_FB_BODY_RADIUS, // Neural feedback - Action

        // Number of input neurons.
        NNI_Count,
    };

    // Neural network output addresses
    enum NeuralOutputs
    { 
        NNO_DISP = 0,       // Displacement
        NNO_ROTA,           // Rotation
        NNO_EYE_RADIUS,     // Radius of vision ( 150-0 )
        NNO_FOCUS,          // Angle between the eyes ( 180-0 )
        NNO_BODY_RADIUS,    // Radius of vision ( 150-0 )
        // Number of output neurons.
        NNO_Count,
    };

    // Displacement and angle resolution
    enum { DISPLACEMENT_STEP_LEVEL  = 100 };
    enum { ROTATION_STEP_LEVEL      = 100 };
    enum { EYE_RADIUS_STEP_LEVEL    =  50 };
    enum { EYEANGLE_STEP_LEVEL      =  50 };
    enum { BODY_RADIUS_STEP_LEVEL   = 500 };

    // Counts the number of times the creature has spawned.
    int spawnCount_;
    // Counts the number of times the creature has fed.
    int feedCount_;

    int angle_;             // Angle the creature is pointing
    int eyeRadius_;         // Radius of vision for the eyes
    int eyeAngle_;          // Angle between both eyes
    int bodyRadius_;         // Radius of vision for the eyes

protected:
    int xLevel_;
    int yLevel_;
    int angleLevel_;
    int eyeRadiusLevel_;
    int eyeAngleLevel_;
    int bodyRadiusLevel_;

    int angleChange_;
    StrangeNeuralNetwork* brain_;
    StrangeNNGene* gene_;

public:
    StrangeLivingCreature( StrangeNNGene* aGene );
    virtual ~StrangeLivingCreature();

    StrangeNNGene* getGene();
    void tickBrain();
    void pushBrainInputs();
    // Returns energy used
    int popBrainOutputs();
};


#endif //_StrangeLivingCreature_h_included_

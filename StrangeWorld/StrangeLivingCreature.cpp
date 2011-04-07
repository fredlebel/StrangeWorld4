#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include "StrangeLivingCreature.h"
#include "StrangeWorld.h"
#include "StrangeNeuralNetwork.h"
#include "OperationCreatureConcentration.h"
#include "StrangeNNGene.h"
#include "MathAccel.h"
#include "FastRand.h"
#include "math.h"


// Function name   : StrangeLivingCreature::StrangeLivingCreature
// Description     : 
// Return type     : 
// Argument        : StrangeNNGene* aGene
StrangeLivingCreature::StrangeLivingCreature( StrangeNNGene* aGene )
    : gene_( aGene )
    , spawnCount_( 0 )
    , feedCount_( 0 )
    , angle_( randomMT() % 360 )
    , eyeRadius_( 50 )
    , eyeAngle_( 45 )
    , xLevel_( 0 )
    , yLevel_( 0 )
    , angleLevel_( 0 )
    , eyeRadiusLevel_( 0 )
    , eyeAngleLevel_( 0 )
    , bodyRadius_( MIN_BODY_RADIUS )
    , bodyRadiusLevel_( 0 )
    , angleChange_( 0 )
{
    // Construct the brain
    if ( gene_ != NULL )
        brain_ = new StrangeNeuralNetwork( gene_ );
    else
        brain_ = NULL;

}


// Function name   : StrangeLivingCreature::~StrangeLivingCreature
// Description     : 
// Return type     : 
StrangeLivingCreature::~StrangeLivingCreature()
{
    delete gene_;
    delete brain_;
}


// Function name   : StrangeLivingCreature::getGene
// Description     : 
// Return type     : StrangeNNGene* 
StrangeNNGene* StrangeLivingCreature::getGene()
{
    return gene_;
}


// Function name   : StrangeLivingCreature::tickBrain
// Description     : 
// Return type     : void 
void StrangeLivingCreature::tickBrain()
{
    brain_->tick();
}


// Function name   : StrangeLivingCreature::pushBrainInputs
// Description     : 
// Return type     : void 
void StrangeLivingCreature::pushBrainInputs()
{
    int lx = (int)( x_ + MathAccel::cos( angle_ + eyeAngle_ ) * eyeRadius_ );
    int ly = (int)( y_ - MathAccel::sin( angle_ + eyeAngle_ ) * eyeRadius_ );
    int rx = (int)( x_ + MathAccel::cos( angle_ - eyeAngle_ ) * eyeRadius_ );
    int ry = (int)( y_ - MathAccel::sin( angle_ - eyeAngle_ ) * eyeRadius_ );
    
    OperationCreatureConcentration lc( world_, this, lx, ly, eyeRadius_ );
    OperationCreatureConcentration rc( world_, this, rx, ry, eyeRadius_ );

    world_->globalOperation( &lc );
    world_->globalOperation( &rc );

    brain_->push( NNI_L_CARN,       __min( lc.carnivoreConcentration,       1000 ) );
    brain_->push( NNI_L_HERB,       __min( lc.herbivoreConcentration,       1000 ) );
    brain_->push( NNI_L_GRAS,       __min( lc.grassConcentration,           1000 ) );

    brain_->push( NNI_R_CARN,       __min( rc.carnivoreConcentration,       1000 ) );
    brain_->push( NNI_R_HERB,       __min( rc.herbivoreConcentration,       1000 ) );
    brain_->push( NNI_R_GRAS,       __min( rc.grassConcentration,           1000 ) );

    brain_->push( NNI_HEALTH,       __min( health_, 1000 ) );
    brain_->push( NNI_EYE_ANGLE,    eyeAngle_  * EYE_ANGLE_MULTIPLYER );
    brain_->push( NNI_EYE_RADIUS,   eyeRadius_ * EYE_RADIUS_MULTIPLYER );

    // Feedback of neural outputs
    brain_->push( NNI_FB_DISP,         brain_->pop( NNO_DISP ) );
    brain_->push( NNI_FB_ROTA,         brain_->pop( NNO_ROTA ) );
    brain_->push( NNI_FB_EYE_RADIUS,   brain_->pop( NNO_EYE_RADIUS ) );
    brain_->push( NNI_FB_FOCUS,        brain_->pop( NNO_FOCUS ) );
    brain_->push( NNI_FB_BODY_RADIUS,  brain_->pop( NNO_BODY_RADIUS ) );
}

//
//
void processStepValue( int& realValue, int& stepValue, int stepSize, int difference )
{
    stepValue += difference;
    while ( stepValue > stepSize )
    {
        stepValue -= stepSize;
        ++realValue;
    }
    while ( stepValue < -stepSize )
    {
        stepValue += stepSize;
        --realValue;
    }
}


// Function name   : StrangeLivingCreature::popBrainOutputs
// Description     : 
// Return type     : void 
int StrangeLivingCreature::popBrainOutputs()
{
    int energyused = 0;

    // Process displacement
    {
        int value = brain_->pop( NNO_DISP );
        assert( ( value <= 1000 ) && ( value >= -1000 ) );
        // Positive version of the value
        // energy used to move
        // int eneryUsed = (int)( pvalue / 80 );
        // Process the x translation
        xLevel_ += (int)( value * MathAccel::cos( angle_ ) );
        while ( xLevel_ > DISPLACEMENT_STEP_LEVEL )
        {
            xLevel_ -= DISPLACEMENT_STEP_LEVEL;
            ++x_;
            ++energyused;
        }
        while ( xLevel_ < -DISPLACEMENT_STEP_LEVEL )
        {
            xLevel_ += DISPLACEMENT_STEP_LEVEL;
            --x_;
            ++energyused;
        }
        
        // Process the y translation
        yLevel_ -= (int)( value * MathAccel::sin( angle_ ) );
        while ( yLevel_ > DISPLACEMENT_STEP_LEVEL )
        {
            yLevel_ -= DISPLACEMENT_STEP_LEVEL;
            ++y_;
            ++energyused;
        }
        while ( yLevel_ < -DISPLACEMENT_STEP_LEVEL )
        {
            yLevel_ += DISPLACEMENT_STEP_LEVEL;
            --y_;
            ++energyused;
        }

        // Wrap coordinates
        world_->wrapX( x_ );
        world_->wrapY( y_ );
    }

    // Process rotation
    {
        int value = brain_->pop( NNO_ROTA );

        int oldAngle = angle_;

        processStepValue( angle_, angleLevel_, ROTATION_STEP_LEVEL, value );

        // Every 10 degress the creature loses one health.
        angleChange_ += abs( oldAngle - angle_ );
        while ( angleChange_ > 10 )
        {
            angleChange_ -= 10;
            ++energyused;
        }

        // Wrap angle
        if ( angle_ > 360 )
            angle_ -= 360;
        else if ( angle_ < 0 )
            angle_ += 360;
    }
    
    // Process vision radius
    {
        int value = brain_->pop( NNO_EYE_RADIUS );

        processStepValue( eyeRadius_, eyeRadiusLevel_, EYE_RADIUS_STEP_LEVEL, value );

        // Limit eye radius
        eyeRadius_ = __min( eyeRadius_, MAX_EYE_RADIUS );
        eyeRadius_ = __max( eyeRadius_, MIN_EYE_RADIUS );
    }

    // Process eye angle
    {
        int value = brain_->pop( NNO_FOCUS );

        processStepValue( eyeAngle_, eyeAngleLevel_, EYEANGLE_STEP_LEVEL, value );

        // Limit eye angle
        eyeAngle_ = __min( eyeAngle_, MAX_EYE_ANGLE );
        eyeAngle_ = __max( eyeAngle_, MIN_EYE_ANGLE );
    }

    // Process body radius
    {
        int value = brain_->pop( NNO_BODY_RADIUS );

        processStepValue( bodyRadius_, bodyRadiusLevel_, BODY_RADIUS_STEP_LEVEL, value );

        // Limit eye radius
        bodyRadius_ = __min( bodyRadius_, MAX_BODY_RADIUS );
        bodyRadius_ = __max( bodyRadius_, MIN_BODY_RADIUS );
    }

    if ( energyused >= 2 )
        energyused /= 2;

    return energyused;
}


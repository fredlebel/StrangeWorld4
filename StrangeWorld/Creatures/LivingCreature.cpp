#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include "LivingCreature.h"
#include "World.h"
#include "NeuralNetwork/StrangeNeuralNetwork.h"
#include "Operations/OpConcentration.h"
#include "NeuralNetwork/NNGene.h"
#include "MathAccel.h"
#include "FastRand.h"
#include "math.h"


// Function name   : LivingCreature::LivingCreature
// Description     : 
// Return type     : 
// Argument        : NNGene* aGene
LivingCreature::LivingCreature( NNGene* aGene )
    : gene_( aGene )
    , spawnCount_( 0 )
    , feedCount_( 0 )
    , angle_( randomMT() % 360 )
    , eyeRadius_( 50 )
    , eyeAngle_( 45 )
    , bodyRadius_( MIN_BODY_RADIUS )
{
    // Construct the brain
    if ( gene_.get() != NULL )
        brain_.reset(new StrangeNeuralNetwork( gene_.get() ));

}


// Function name   : LivingCreature::~LivingCreature
// Description     : 
// Return type     : 
LivingCreature::~LivingCreature()
{
}


// Function name   : LivingCreature::getGene
// Description     : 
// Return type     : NNGene* 
NNGene* LivingCreature::getGene()
{
    return gene_.get();
}


// Function name   : LivingCreature::tickBrain
// Description     : 
// Return type     : void 
void LivingCreature::tickBrain()
{
    brain_->tick();
}


// Function name   : LivingCreature::pushBrainInputs
// Description     : 
// Return type     : void 
void LivingCreature::pushBrainInputs()
{
    double lx = x_ + MathAccel::cos( angle_ + eyeAngle_ ) * eyeRadius_;
    double ly = y_ - MathAccel::sin( angle_ + eyeAngle_ ) * eyeRadius_;
    double rx = x_ + MathAccel::cos( angle_ - eyeAngle_ ) * eyeRadius_;
    double ry = y_ - MathAccel::sin( angle_ - eyeAngle_ ) * eyeRadius_;
    
    OpConcentration lc( world_, this, lx, ly, eyeRadius_ );
    OpConcentration rc( world_, this, rx, ry, eyeRadius_ );

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

// Function name   : LivingCreature::popBrainOutputs
// Description     : 
// Return type     : void 
double LivingCreature::popBrainOutputs()
{
    double energyused = 0;

    // Process displacement
    {
        double value = brain_->pop( NNO_DISP );
        assert( ( value <= 1000 ) && ( value >= -1000 ) );
        // Positive version of the value
        // energy used to move
        // int eneryUsed = (int)( pvalue / 80 );
        // Process the x translation
		value *= MathAccel::cos( angle_ ) / 100;
        x_ += value;
		energyused += std::abs(value);
        
        // Process the y translation
        value = brain_->pop( NNO_DISP );
		value *= MathAccel::sin( angle_ ) / 100;
        y_ -= value;
		energyused += std::abs(value);

        // Wrap coordinates
        world_->wrapXY( x_, y_ );
    }

    // Process rotation
    {
        double value = brain_->pop( NNO_ROTA );

		angle_ += value / 100;
		energyused += std::abs(value) / 1000;

		if (angle_ > 360) angle_ -= 360;
		if (angle_ < 0) angle_ += 360;
    }
    
    // Process vision radius
    {
        double value = brain_->pop( NNO_EYE_RADIUS );

		eyeRadius_ += value / 50;

        // Limit eye radius
        eyeRadius_ = __min( eyeRadius_, MAX_EYE_RADIUS );
        eyeRadius_ = __max( eyeRadius_, MIN_EYE_RADIUS );
    }

    // Process eye angle
    {
        double value = brain_->pop( NNO_FOCUS );
		eyeAngle_ += value / 50;

        // Limit eye angle
        eyeAngle_ = __min( eyeAngle_, MAX_EYE_ANGLE );
        eyeAngle_ = __max( eyeAngle_, MIN_EYE_ANGLE );
    }

    // Process body radius
    {
        double value = brain_->pop( NNO_BODY_RADIUS );
		bodyRadius_ += value / 200;

        // Limit eye radius
        bodyRadius_ = __min( bodyRadius_, MAX_BODY_RADIUS );
        bodyRadius_ = __max( bodyRadius_, MIN_BODY_RADIUS );
    }

    energyused /= 2;

    return energyused;
}


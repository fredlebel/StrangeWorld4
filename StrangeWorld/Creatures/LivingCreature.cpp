#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include "LivingCreature.h"
#include "World.h"
#include "NeuralNetwork/NeuralNetwork.h"
#include "Operations/OpAsyncConcentration.h"
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
    , _contact( nullptr )
{
    // Construct the brain
    if ( gene_.get() != nullptr )
        brain_ = NeuralNetwork::buildFromGene( NNI_COUNT, gene_.get() );

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

Creature* LivingCreature::getContact()
{
    return _contact;
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
    
    OpAsyncConcentration lc( world_, this, lx, ly, eyeRadius_ );
    OpAsyncConcentration rc( world_, this, rx, ry, eyeRadius_ );

    world_->globalOperation( &lc );
    world_->globalOperation( &rc );

    brain_->push( NNI_L_CARN,       std::min( lc.carnivoreConcentration / 1000.0, NeuralNetwork::BiasMax ) );
    brain_->push( NNI_L_HERB,       std::min( lc.herbivoreConcentration / 1000.0, NeuralNetwork::BiasMax ) );
    brain_->push( NNI_L_GRAS,       std::min( lc.grassConcentration     / 1000.0, NeuralNetwork::BiasMax ) );

    brain_->push( NNI_R_CARN,       std::min( rc.carnivoreConcentration / 1000.0, NeuralNetwork::BiasMax ) );
    brain_->push( NNI_R_HERB,       std::min( rc.herbivoreConcentration / 1000.0, NeuralNetwork::BiasMax ) );
    brain_->push( NNI_R_GRAS,       std::min( rc.grassConcentration     / 1000.0, NeuralNetwork::BiasMax ) );

    brain_->push( NNI_HEALTH,       std::min( health_ / 1000.0, NeuralNetwork::BiasMax ) );
    brain_->push( NNI_EYE_ANGLE,    eyeAngle_  / MAX_EYE_ANGLE );
    brain_->push( NNI_EYE_RADIUS,   eyeRadius_ / MAX_EYE_RADIUS );

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
        double value;
        // Process the x translation
        value = brain_->pop( NNO_DISP );
        value *= MathAccel::cos( angle_ ) * 10.0;
        x_ += value;
        energyused += std::abs(value);
        
        // Process the y translation
        value = brain_->pop( NNO_DISP );
        value *= MathAccel::sin( angle_ ) * 10.0;
        y_ -= value;
        energyused += std::abs(value);

        // Wrap coordinates
        world_->wrapXY( x_, y_ );
    }

    // Process rotation
    {
        double value = brain_->pop( NNO_ROTA );

        angle_ += value * 10.0;
        energyused += std::abs(value);

        if (angle_ > 360) angle_ -= 360;
        if (angle_ < 0) angle_ += 360;
    }
    
    // Process vision radius
    {
        double value = brain_->pop( NNO_EYE_RADIUS );

        eyeRadius_ += value * 10;

        // Limit eye radius
        eyeRadius_ = std::min( eyeRadius_, MAX_EYE_RADIUS );
        eyeRadius_ = std::max( eyeRadius_, MIN_EYE_RADIUS );
    }

    // Process eye angle
    {
        double value = brain_->pop( NNO_FOCUS );
        eyeAngle_ += value * 10;

        // Limit eye angle
        eyeAngle_ = std::min( eyeAngle_, MAX_EYE_ANGLE );
        eyeAngle_ = std::max( eyeAngle_, MIN_EYE_ANGLE );
    }

    // Process body radius
    {
        double value = brain_->pop( NNO_BODY_RADIUS );
        bodyRadius_ += value * 2;

        // Limit eye radius
        bodyRadius_ = std::min( bodyRadius_, MAX_BODY_RADIUS );
        bodyRadius_ = std::max( bodyRadius_, MIN_BODY_RADIUS );
    }

    energyused /= 2;

    return energyused;
}


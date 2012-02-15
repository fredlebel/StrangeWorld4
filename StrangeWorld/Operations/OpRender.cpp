#include "StrangeView.h"
#include "Operations/OpAsyncConcentration.h"
#include "Operations/OpRender.h"
#include "Creatures/Carnivore.h"
#include "Creatures/Herbivore.h"
#include "Creatures/Grass.h"
#include "World.h"
#include "MathAccel.h"

#include <windows.h> // Only for wsprintf

// Function name   : OperationCreatureRenderer::OperationCreatureRenderer
// Description     : 
// Return type     : 
// Argument        : StrangeView* view
OperationCreatureRenderer::OperationCreatureRenderer( StrangeView* view,
        bool drawSensors, bool drawData )
    : view_( view )
    , drawSensors_( drawSensors )
    , drawData_( drawData )
{
}


// Function name   : OperationCreatureRenderer::~OperationCreatureRenderer
// Description     : 
// Return type     : 
OperationCreatureRenderer::~OperationCreatureRenderer()
{
}


// Function name   : OperationCreatureRenderer::visit_Carnivore
// Description     : 
// Return type     : void 
// Argument        : Carnivore* creature
void OperationCreatureRenderer::visit_Carnivore( Carnivore* creature )
{
    double lx = creature->getX() - MathAccel::cos( creature->angle_ ) * creature->getRadius() * 2;
    double ly = creature->getY() + MathAccel::sin( creature->angle_ ) * creature->getRadius() * 2;
    view_->drawCarnivore(
        creature->getX(), creature->getY(), lx, ly,
        creature->getRadius(),
        creature->getHealth(),
        creature->selected_,
        !creature->isLiving() );

    renderLivingCreature( creature );
}


// Function name   : OperationCreatureRenderer::visit_Grass
// Description     : 
// Return type     : void 
// Argument        : Grass* creature
void OperationCreatureRenderer::visit_Grass( Grass* creature )
{
    view_->drawGrass(
        creature->getX(),
        creature->getY(),
        creature->getRadius(),
        creature->getHealth(), 
        creature->selected_ );

    if ( drawSensors_ && drawData_ && creature->selected_ )
    {
        wchar_t buff[80];
        wsprintf( buff, L"(H:%i)", 
            creature->getHealth() );
        view_->write( creature->getX(), creature->getY(), buff );
    }
}


// Function name   : OperationCreatureRenderer::visit_Herbivore
// Description     : 
// Return type     : void 
// Argument        : Herbivore* creature
void OperationCreatureRenderer::visit_Herbivore( Herbivore* creature )
{
    double lx = creature->getX() - MathAccel::cos( creature->angle_ ) * creature->getRadius() * 2;
    double ly = creature->getY() + MathAccel::sin( creature->angle_ ) * creature->getRadius() * 2;
    view_->drawHerbivore(
        creature->getX(), creature->getY(), lx, ly,
        creature->getRadius(),
        creature->getHealth(),
        creature->selected_,
        !creature->isLiving() );

    renderLivingCreature( creature );
}

//
//
//
void OperationCreatureRenderer::renderLivingCreature( LivingCreature* creature )
{
    if ( drawSensors_ && creature->selected_ && creature->isLiving() )
    {
        int vrx = (int)( creature->getX() + MathAccel::cos( creature->angle_ - creature->eyeAngle_ ) * creature->eyeRadius_ );
        int vry = (int)( creature->getY() - MathAccel::sin( creature->angle_ - creature->eyeAngle_ ) * creature->eyeRadius_ );
        int vlx = (int)( creature->getX() + MathAccel::cos( creature->angle_ + creature->eyeAngle_ ) * creature->eyeRadius_ );
        int vly = (int)( creature->getY() - MathAccel::sin( creature->angle_ + creature->eyeAngle_ ) * creature->eyeRadius_ );
        view_->drawSensors( vrx, vry, vlx, vly, creature->eyeRadius_ );

        if ( drawData_ )
        {
            OpAsyncConcentration rc( creature->getWorld(), creature, vrx, vry, creature->eyeRadius_ );
            OpAsyncConcentration lc( creature->getWorld(), creature, vlx, vly, creature->eyeRadius_ );

            creature->getWorld()->runOperation( rc );
            creature->getWorld()->runOperation( lc );

            {
                wchar_t buff[80];
                wsprintf( buff, L"(R:%i, G:%i, B:%i)", 
                    (int)rc.carnivoreConcentration, 
                    (int)rc.grassConcentration, 
                    (int)rc.herbivoreConcentration );
                view_->write( vrx, vry, buff );
            }
            {
                wchar_t buff[80];
                wsprintf( buff, L"(R:%i, G:%i, B:%i)", 
                    (int)lc.carnivoreConcentration, 
                    (int)lc.grassConcentration, 
                    (int)lc.herbivoreConcentration );
                view_->write( vlx, vly, buff );
            }
            {
                wchar_t buff[80];
                wsprintf( buff, L"(H:%i)", 
                    (int)creature->getHealth() );
                view_->write( creature->getX(), creature->getY(), buff );
            }
        }
    }
}



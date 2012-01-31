#ifndef _StrangeWindowsView_h_included_
#define _StrangeWindowsView_h_included_

#include <Windows.h>
#include "StrangeView.h"
#include "World.h"

//#define USE_GDIPLUS
#if defined( USE_GDIPLUS )
#include "gdiplus.h"
#endif

class StrangeWindowsView
    : public StrangeView
{
public:
    StrangeWindowsView( HWND hWnd, World* world );
    virtual ~StrangeWindowsView();

    void beginPaint( HDC dc );
    void endPaint();

    virtual int getWidth();
    virtual int getHeight();

    // Rendering calls
    virtual void drawCarnivore( double x, double y, double tx, double ty, double r, double health, bool selected, bool dead );
    virtual void drawHerbivore( double x, double y, double tx, double ty, double r, double health, bool selected, bool dead );
    virtual void drawGrass( double x, double y, double r, double health, bool selected );
    virtual void drawSensors( double x1, double y1, double x2, double y2, double r );
    virtual void write( double x, double y, std::wstring const& str );

protected:
    HWND hWnd_;
    World* world_;
    HDC dc_;
#if defined( USE_GDIPLUS )
    Gdiplus::Graphics* graphics_;
#endif
};

#endif //_StrangeWindowsView_h_included_
#ifndef _StrangeWindowsView_h_included_
#define _StrangeWindowsView_h_included_

#include <Windows.h>
#include <StrangeView.h>
#include <StrangeWorld.h>

//#define USE_GDIPLUS
#if defined( USE_GDIPLUS )
#include "gdiplus.h"
#endif

class StrangeWindowsView
    : public StrangeView
{
public:
    StrangeWindowsView( HWND hWnd, StrangeWorld* world );
    virtual ~StrangeWindowsView();

    void beginPaint( HDC dc );
    void endPaint();

    virtual int getWidth();
    virtual int getHeight();

    // Rendering calls
    virtual void drawCarnivore( int x, int y, int tx, int ty, int r, int health, bool selected, bool dead );
    virtual void drawHerbivore( int x, int y, int tx, int ty, int r, int health, bool selected, bool dead );
    virtual void drawGrass( int x, int y, int r, int health, bool selected );
    virtual void drawSensors( int x1, int y1, int x2, int y2, int r );
    virtual void write( int x, int y, std::wstring const& str );

protected:
    HWND hWnd_;
    StrangeWorld* world_;
    HDC dc_;
#if defined( USE_GDIPLUS )
    Gdiplus::Graphics* graphics_;
#endif
};

#endif //_StrangeWindowsView_h_included_
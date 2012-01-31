#include <assert.h>
#include "StrangeWindowsView.h"
#include <vector>

static HFONT TextFont   = CreateFont( 10, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
                                     OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                     DEFAULT_QUALITY, DEFAULT_PITCH, L"Lucida Console" );

#if !defined( USE_GDIPLUS )

static std::vector<HPEN> gRedPensPastel;
static std::vector<HPEN> gGreenPensPastel;
static std::vector<HPEN> gBluePensPastel;

static std::vector<HPEN> gRedPensDark;
static std::vector<HPEN> gGreenPensDark;
static std::vector<HPEN> gBluePensDark;

// The pastel pens
static HPEN RedPen      = CreatePen(PS_SOLID, 1, RGB(255,   0,   0));
static HPEN GreenPen    = CreatePen(PS_SOLID, 1, RGB(128, 255, 128));
static HPEN WhitePen    = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
static HPEN YellowPen   = CreatePen(PS_SOLID, 1, RGB(255, 255,   0));

static LOGBRUSH emptyBrushLB = { BS_HOLLOW };
static HBRUSH emptyBrush = CreateBrushIndirect( &emptyBrushLB );


StrangeWindowsView::StrangeWindowsView( HWND hWnd, World* world )
    : hWnd_( hWnd )
    , world_( world )
{
    if ( gRedPensPastel.empty() )
    {
        for ( int i = 0; i < 128; ++i )
        {
            gRedPensPastel.push_back(   CreatePen( PS_SOLID, 2, RGB(128+i, 128-i,   128-i) ) );
            gGreenPensPastel.push_back( CreatePen( PS_SOLID, 2, RGB(128-i, 128+i,   128-i) ) );
            gBluePensPastel.push_back(  CreatePen( PS_SOLID, 2, RGB(128-i, 128-i,   128+i) ) );

            gRedPensDark.push_back(   CreatePen( PS_SOLID, 2, RGB(i*2,   0,   0) ) );
            gGreenPensDark.push_back( CreatePen( PS_SOLID, 2, RGB(  0, i*2,   0) ) );
            gBluePensDark.push_back(  CreatePen( PS_SOLID, 2, RGB(  0,   0, i*2) ) );
        }
    }
}

StrangeWindowsView::~StrangeWindowsView()
{
}

void StrangeWindowsView::beginPaint(HDC dc)
{
    dc_ = dc;
    HBRUSH br = CreateSolidBrush(RGB(0, 0, 0));
    RECT rect = {0, 0, world_->getWidth(), world_->getHeight() };
    FillRect(dc_, &rect, br);
    DeleteObject(br);
}

void StrangeWindowsView::endPaint()
{
    dc_ = NULL;
}

int StrangeWindowsView::getWidth()
{
    RECT rect;
    GetWindowRect( hWnd_, &rect );
    return (rect.right - rect.left);
}

int StrangeWindowsView::getHeight()
{
    RECT rect;
    GetWindowRect(hWnd_, &rect);
    return (rect.bottom - rect.top);
}

// Rendering calls
void StrangeWindowsView::drawCarnivore(
    double x, double y, double tx, double ty, double r,
    double health, bool selected, bool dead )
{
    HBRUSH br = emptyBrush;
    HPEN pen = gRedPensPastel[ max( health / 8, 0 ) ];

    HBRUSH prevBrush = (HBRUSH)SelectObject( dc_, br );
    HPEN prevPen = (HPEN)SelectObject( dc_, pen );

    Ellipse( dc_, x - r, y - r, x + r, y + r );
    if ( ! dead )
    {
        MoveToEx( dc_, x, y, NULL );
        LineTo( dc_, tx, ty );
    }

    if ( selected )
    {
        HPEN tmp = (HPEN)SelectObject( dc_, YellowPen );
        Ellipse( dc_, x - r-8, y - r-8, x + r+8, y + r+8 );
        SelectObject( dc_, tmp );
    }

    SelectObject(dc_, prevBrush);
    SelectObject(dc_, prevPen);
}

void StrangeWindowsView::drawHerbivore(
    double x, double y, double tx, double ty, double r,
    double health, bool selected, bool dead )
{
    HBRUSH br = emptyBrush;
    HPEN pen = gBluePensPastel[ max( health / 8, 0 ) ];

    HBRUSH prevBrush = (HBRUSH)SelectObject( dc_, br );
    HPEN prevPen = (HPEN)SelectObject( dc_, pen );

    Ellipse( dc_, x - r, y - r, x + r, y + r );
    if ( ! dead )
    {
        MoveToEx( dc_, x, y, NULL );
        LineTo( dc_, tx, ty );
    }

    if ( selected )
    {
        HPEN tmp = (HPEN)SelectObject( dc_, YellowPen );
        Ellipse( dc_, x - r-8, y - r-8, x + r+8, y + r+8 );
        SelectObject( dc_, tmp );
    }

    SelectObject(dc_, prevBrush);
    SelectObject(dc_, prevPen);
}

void StrangeWindowsView::drawGrass( double x, double y, double r, double health, bool selected )
{
    HBRUSH br = emptyBrush;
    HPEN pen = gGreenPensPastel[ max( health / 8, 0 ) ];

    HBRUSH prevBrush = (HBRUSH)SelectObject(dc_, br);
    HPEN prevPen = (HPEN)SelectObject(dc_, pen);

    Ellipse( dc_, x - r, y -r, x + r, y + r );

    if ( selected )
    {
        HPEN tmp = (HPEN)SelectObject( dc_, YellowPen );
        Ellipse( dc_, x - r*2, y - r*2, x + r*2, y + r*2 );
        SelectObject( dc_, tmp );
    }

    SelectObject(dc_, prevBrush);
    SelectObject(dc_, prevPen);
}

void StrangeWindowsView::drawSensors( double x1, double y1, double x2, double y2, double r )
{
    HBRUSH br = emptyBrush;
    HPEN pen = WhitePen;

    HBRUSH prevBrush = (HBRUSH)SelectObject(dc_, br);
    HPEN prevPen = (HPEN)SelectObject(dc_, pen);

    Ellipse( dc_, x1 - r, y1 -r, x1 + r, y1 + r );
    Ellipse( dc_, x2 - r, y2 -r, x2 + r, y2 + r );

    SelectObject(dc_, prevBrush);
    SelectObject(dc_, prevPen);
}

void StrangeWindowsView::write( double x, double y, std::wstring const& str )
{
    assert( dc_ );
    ::SetTextColor( dc_, RGB( 255, 255, 255 ) );
    SetBkMode( dc_, TRANSPARENT );
    HFONT oldFont = (HFONT)SelectObject( dc_, TextFont );
    TextOut( dc_, x, y, str.c_str(), (int)str.size() );
    SelectObject( dc_, oldFont );
}


#else

StrangeWindowsView::StrangeWindowsView( HWND hWnd, World* world )
    : hWnd_( hWnd )
    , world_( world )
{
}

StrangeWindowsView::~StrangeWindowsView()
{
}

void StrangeWindowsView::beginPaint(HDC dc)
{
    dc_ = dc;
    graphics_ = new Gdiplus::Graphics( dc );
    graphics_->SetSmoothingMode( Gdiplus::SmoothingModeAntiAlias );
    HBRUSH br = CreateSolidBrush(RGB(0, 0, 0));
    RECT rect = {0, 0, world_->getWidth(), world_->getHeight() };
    FillRect(dc_, &rect, br);
    DeleteObject(br);
}

void StrangeWindowsView::endPaint()
{
    dc_ = NULL;
    delete graphics_;
    graphics_ = NULL;
}

int StrangeWindowsView::getWidth()
{
    RECT rect;
    GetWindowRect( hWnd_, &rect );
    return (rect.right - rect.left);
}

int StrangeWindowsView::getHeight()
{
    RECT rect;
    GetWindowRect(hWnd_, &rect);
    return (rect.bottom - rect.top);
}

// Rendering calls
void StrangeWindowsView::drawCarnivore( int x, int y, int tx, int ty, int r, int health, bool selected, bool dead )
{
    {
        int i = max( health / 8, 0 );
        Gdiplus::Pen p( Gdiplus::Color(255, 128+i, 128-i, 128-i), 2);
        graphics_->DrawEllipse( &p, x - r, y - r, r*2, r*2 );
        graphics_->DrawLine( &p, x, y, tx, ty );
    }

    if ( selected )
    {
        Gdiplus::Pen p( Gdiplus::Color(255, 255, 255, 0), 1);
        graphics_->DrawEllipse( &p, x - r*2, y - r*2, r*4, r*4 );
    }
    if ( dead )
    {
        Gdiplus::Pen p( Gdiplus::Color(255, 255, 0, 0), 1);
        graphics_->DrawEllipse( &p, x - r-2, y - r-2, r+4, r+4 );
    }
}

void StrangeWindowsView::drawHerbivore( int x, int y, int tx, int ty, int r, int health, bool selected, bool dead )
{
    {
        int i = max( health / 8, 0 );
        Gdiplus::Pen p( Gdiplus::Color(255, 128-i, 128-i, 128+i), 2);
        graphics_->DrawEllipse( &p, x - r, y - r, r*2, r*2 );
        graphics_->DrawLine( &p, x, y, tx, ty );
    }

    if ( selected )
    {
        Gdiplus::Pen p( Gdiplus::Color(255, 255, 255, 0), 1);
        graphics_->DrawEllipse( &p, x - r*2, y - r*2, r*4, r*4 );
    }
    if ( dead )
    {
        Gdiplus::Pen p( Gdiplus::Color(255, 255, 0, 0), 1);
        graphics_->DrawEllipse( &p, x - r-2, y - r-2, r*2+4, r*2+4 );
    }
}

void StrangeWindowsView::drawGrass( int x, int y, int r, int health, bool selected )
{
    {
        int i = max( health / 8, 0 );
        Gdiplus::Pen p( Gdiplus::Color(255, 128-i, 128+i, 128-i), 2);
        graphics_->DrawEllipse( &p, x - r, y - r, r*2, r*2 );
    }

    if ( selected )
    {
        Gdiplus::Pen p( Gdiplus::Color(255, 255, 255, 0), 1);
        graphics_->DrawEllipse( &p, x - r*2, y - r*2, r*4, r*4 );
    }
}

void StrangeWindowsView::drawSensors( int x1, int y1, int x2, int y2, int r )
{
    Gdiplus::Pen p( Gdiplus::Color(255, 255, 255, 255), 1);
    graphics_->DrawEllipse( &p, x1 - r, y1 - r, r*2, r*2 );
    graphics_->DrawEllipse( &p, x2 - r, y2 - r, r*2, r*2 );
}

void StrangeWindowsView::write( int x, int y, std::wstring const& str )
{
    assert( dc_ );
    ::SetTextColor( dc_, RGB( 255, 255, 255 ) );
    SetBkMode( dc_, TRANSPARENT );
    HFONT oldFont = (HFONT)SelectObject( dc_, TextFont );
    TextOut( dc_, x, y, str.c_str(), (int)str.size() );
    SelectObject( dc_, oldFont );
}

#endif


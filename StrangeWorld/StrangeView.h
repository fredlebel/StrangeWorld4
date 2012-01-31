#ifndef _StrangeView_h_included_
#define _StrangeView_h_included_

#include <string>

class StrangeView
{
public:
    struct Color
    {
        Color(unsigned char r, unsigned char g, unsigned char b) 
            : red(r), green(g), blue(b) {}
        bool operator ==(Color& col)
        {
            return ((col.red == red) && 
                (col.green == green) &&
                (col.blue == blue));
        }
        unsigned char red;
        unsigned char green;
        unsigned char blue;
    };

    StrangeView() {}
    virtual ~StrangeView() {}

    virtual int getWidth() = 0;
    virtual int getHeight() = 0;

    // Rendering calls
    virtual void drawCarnivore( double x, double y, double tx, double ty, double r, double health, bool selected, bool dead ) = 0;
    virtual void drawHerbivore( double x, double y, double tx, double ty, double r, double health, bool selected, bool dead ) = 0;
    virtual void drawGrass( double x, double y, double r, double health, bool selected ) = 0;
    virtual void drawSensors( double x1, double y1, double x2, double y2, double r ) = 0;
    virtual void write( double x, double y, std::wstring const& str ) = 0;
};

#endif //_StrangeView_h_included_

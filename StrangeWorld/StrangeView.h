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
    virtual void drawCarnivore( int x, int y, int tx, int ty, int r, int health, bool selected, bool dead ) = 0;
    virtual void drawHerbivore( int x, int y, int tx, int ty, int r, int health, bool selected, bool dead ) = 0;
    virtual void drawGrass( int x, int y, int r, int health, bool selected ) = 0;
    virtual void drawSensors( int x1, int y1, int x2, int y2, int r ) = 0;
    virtual void write( int x, int y, std::wstring const& str ) = 0;
};

#endif //_StrangeView_h_included_

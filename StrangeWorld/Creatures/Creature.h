#ifndef _Creature_h_included_
#define _Creature_h_included_

#include <stdlib.h>
#include <assert.h>

class World;
class Operation;

class Creature
{
protected:
    double health_;
    int age_;
    bool isAlive_;
    World* world_;
    double x_;
    double y_;

public:
    bool selected_; // Selection is passed on to offsprings
    
public:
    Creature();
    virtual ~Creature();

    inline double getX() { return x_; }
    inline double getY() { return y_; }
    inline void setX( double x ) { x_ = x; }
    inline void setY( double y ) { y_ = y; }
    inline double getHealth() { return health_; }

    inline double hurt( double amount )
    {
        double damage = __min( amount, health_ );
        assert( damage >= 0 );
        health_ -= damage;
        if ( health_ <= 0 && isAlive_ )
        {
            isAlive_ = false;
            die();
        }
        return damage;
    }

    inline void heal( double amount )
    {
        if ( isAlive_ )
        {
            health_ += amount;
            health_ = __min( health_, 1000 );
        }
    }

    inline void kill()
    {
        if ( isAlive_ )
        {
            isAlive_ = false;
            die();
        }
    }

    inline bool isLiving() { return isAlive_; }
    inline void age() { ++age_; }
    inline World* getWorld() { return world_; }
    inline void setWorld( World* world ) { world_ = world; }
    inline int getAge() { return age_; }

    virtual bool accept(Operation& operation) = 0;
    virtual double getRadius() = 0;

protected:
    virtual void die() = 0;

};


#endif //_Creature_h_included_
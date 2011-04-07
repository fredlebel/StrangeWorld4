#ifndef _StrangeCreature_h_included_
#define _StrangeCreature_h_included_

#include <stdlib.h>
#include <assert.h>

class StrangeWorld;
class StrangeCreatureOperation;

class StrangeCreature
{
protected:
    int health_;
    int age_;
    bool isAlive_;
    StrangeWorld* world_;
    int x_;
    int y_;

public:
    bool selected_; // Selection is passed on to offsprings
    
public:
    StrangeCreature();
    virtual ~StrangeCreature();

    inline int getX() { return x_; }
    inline int getY() { return y_; }
    inline void setX( int x ) { x_ = x; }
    inline void setY( int y ) { y_ = y; }
    inline int getHealth() { return health_; }

    inline int hurt( int amount )
    {
        int damage = __min( amount, health_ );
        assert( damage >= 0 );
        health_ -= damage;
        if ( health_ <= 0 && isAlive_ )
        {
            isAlive_ = false;
            die();
        }
        return damage;
    }

    inline void heal( int amount )
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
    inline StrangeWorld* getWorld() { return world_; }
    inline void setWorld( StrangeWorld* world ) { world_ = world; }
    inline int getAge() { return age_; }

    virtual bool accept(StrangeCreatureOperation* operation) = 0;
    virtual int getRadius() = 0;

protected:
    virtual void die() = 0;

};


#endif //_StrangeCreature_h_included_
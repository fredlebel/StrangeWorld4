#ifndef _IThread_h_included_
#define _IThread_h_included_

#include <functional>

class IThread
{
public:
    virtual ~IThread() {}

    virtual void run( std::function<void(void)> fn) = 0;
    virtual void waitUntilDone() = 0;

};

#endif //_IThread_h_included_

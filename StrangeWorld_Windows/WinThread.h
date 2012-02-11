#ifndef _WinThread_h_included_
#define _WinThread_h_included_

#include <Windows.h>
#include "StrangeWorld/IThread.h"

class WinThread
    : public IThread
{
public:
    WinThread();
    virtual ~WinThread();

    virtual void run( std::function<void(void)> fn);
    virtual void waitUntilDone();

private:
    static DWORD WINAPI threadFn( LPVOID lpParam );
    HANDLE _hThread;
    DWORD _threadId;
    HANDLE _taskReadyEvent;
    HANDLE _taskCompletedEvent;

    // Function to execute on the thread.
    std::function<void(void)> _fn;
};

#endif //_WinThread_h_included_

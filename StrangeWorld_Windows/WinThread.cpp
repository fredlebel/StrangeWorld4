#include <assert.h>
#include "WinThread.h"

//#define EXE_SINGLE_THREADED

WinThread::WinThread()
{
    _taskReadyEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
    _taskCompletedEvent = CreateEvent( NULL, FALSE, FALSE, NULL );

    _hThread = CreateThread(
        NULL,           // default security attributes
        0,              // use default stack size
        threadFn,       // thread function name
        this,           // argument to thread function
        0,              // use default creation flags
        &_threadId);    // returns the thread identifier
}

WinThread::~WinThread()
{
    CloseHandle( _hThread );
    CloseHandle( _taskReadyEvent );
    CloseHandle( _taskCompletedEvent );
}

void WinThread::run( std::function<void(void)> fn)
{
#if defined (EXE_SINGLE_THREADED)
    fn();
#else
    _fn = fn;
    SetEvent( _taskReadyEvent );
#endif
}

void WinThread::waitUntilDone()
{
#if defined (EXE_SINGLE_THREADED)
#else
    WaitForSingleObject( _taskCompletedEvent, INFINITE );
#endif
}

DWORD WINAPI WinThread::threadFn( LPVOID lpParam )
{
    WinThread* that = (WinThread*)lpParam;

    while ( true )
    {
        WaitForSingleObject( that->_taskReadyEvent, INFINITE );
        that->_fn();
        SetEvent( that->_taskCompletedEvent );
    }
}

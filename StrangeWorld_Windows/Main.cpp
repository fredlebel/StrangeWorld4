#include <windows.h>

#include "StrangeWindowsView.h"
#include "StrangeWorld.h"
#include "StrangeCarnivore.h"
#include "StrangeHerbivore.h"
#include "StrangeGrass.h"
#include "OperationCreatureRenderer.h"
#include "OperationCreatureHitTest.h"
#include "StrangeNNGene.h"
#include "StrangeNeuralNetwork.h"
#include "MathAccel.h"
#include "FastRand.h"
#include "resource.h"
#include "time.h"

#define APPLICATION_NAME L"StrangeWorld 4.6"

HINSTANCE theMainInstance;
HWND theMainWindow;

LRESULT CALLBACK MainWndProc(HWND theWnd, UINT theMsg, WPARAM wParam, LPARAM lParam);

#define WORLD_WIDTH     1024
#define WORLD_HEIGHT    768
#define GROWTH_RATE     50 // Number of food on screen

#define TRAYNOTIFY          3000
#define TRAYID              3000
HICON trayIcon;

bool gDrawSensors = false;
bool gDrawData = false;

StrangeWorld* gWorld = NULL;
StrangeWindowsView* gView = NULL;

static HDC backBufferDC = NULL;
static HBITMAP backBufferBitmap = NULL;

enum RenderState { eFAST, eNORMAL, ePAUSE, eONESTEP };

RenderState gCurrentState = eNORMAL;
//RenderState gCurrentState = eFAST;

static bool setStatText = true;

void Tick()
{
    HDC dc = GetDC( theMainWindow );
    bool tickDone = false;

    switch ( gCurrentState )
    {
    case eFAST:
        {
            gWorld->tick();
            tickDone = true;
        } break;
    case eONESTEP:
            gCurrentState = ePAUSE;
    case eNORMAL:
            gWorld->tick();
            tickDone = true;
    case ePAUSE:
        {
            if ( NULL == backBufferDC )
            {
                backBufferDC = CreateCompatibleDC( dc );
                backBufferBitmap = CreateCompatibleBitmap( dc, WORLD_WIDTH, WORLD_HEIGHT );
                SelectObject( backBufferDC, backBufferBitmap );
            }

            gView->beginPaint( backBufferDC );

            // Render all the creatures to the view
            gWorld->render( gView, gDrawSensors, gDrawData );

            StretchBlt( dc, 0, 0, 
                gView->getWidth(), gView->getHeight(), 
                backBufferDC, 0, 0, 
                WORLD_WIDTH, WORLD_HEIGHT, SRCCOPY);

            gView->endPaint();

        } break;
    }

    // Every 250,000 iterations save the elite genes in case the program crashes
    if ( ( gWorld->getTickCount() % 250000 ) == 0 )
    {
        if ( StrangeCarnivore::ourEliteGene.get() != NULL )
            StrangeCarnivore::ourEliteGene->saveGene( L"carnivore.gen4" );
        if ( StrangeHerbivore::ourEliteGene.get() != NULL )
            StrangeHerbivore::ourEliteGene->saveGene( L"herbivore.gen4" );
    }

    static int nextCarnivoreGenerationDump = 0;
    static int nextHerbivoreGenerationDump = 0;

    if ( tickDone )
    {
        static unsigned int generationDumpInterval = 10000;

        if ( StrangeCarnivore::ourEliteGene.get() != NULL )
        {
            int currentGeneration = StrangeCarnivore::ourEliteGene->getGeneration();
            if ( nextCarnivoreGenerationDump == 0 )
                nextCarnivoreGenerationDump = (currentGeneration / generationDumpInterval) * 
                generationDumpInterval + generationDumpInterval;
            if ( currentGeneration > nextCarnivoreGenerationDump )
            {
                wchar_t genFilename[80];
                wsprintf(genFilename, L"carnivore_%i.gen4", nextCarnivoreGenerationDump );
                StrangeCarnivore::ourEliteGene->saveGene( genFilename );
                nextCarnivoreGenerationDump += generationDumpInterval;
            }
        }

        if ( StrangeHerbivore::ourEliteGene.get() != NULL )
        {
            int currentGeneration = StrangeHerbivore::ourEliteGene->getGeneration();
            if ( nextHerbivoreGenerationDump == 0 )
                nextHerbivoreGenerationDump = (currentGeneration / generationDumpInterval) * 
                generationDumpInterval + generationDumpInterval;
            if ( currentGeneration > nextHerbivoreGenerationDump )
            {
                wchar_t genFilename[80];
                wsprintf(genFilename, L"herbivore_%i.gen4", nextHerbivoreGenerationDump );
                StrangeHerbivore::ourEliteGene->saveGene( genFilename );
                nextHerbivoreGenerationDump += generationDumpInterval;
            }
        }
    }

    // Update windows title bar
    bool updateWindowTitle = true;
    switch ( gCurrentState )
    {
    case eFAST:
        updateWindowTitle = (gWorld->getTickCount() % 1000 == 0);
        break;
    case eONESTEP:
        updateWindowTitle = true;
        break;
    case eNORMAL:
        updateWindowTitle = (gWorld->getTickCount() % 100 == 0);
        break;
    case ePAUSE:
        break;
    };
    if ( setStatText && updateWindowTitle )
    {
        wchar_t buff[200];
        wsprintf(buff, APPLICATION_NAME L" - %i - Carnivore:(%i/%i-%i-%i-%i) Herbivore:(%i/%i-%i-%i-%i) Mutation(%i)", gWorld->getTickCount(),
            ( StrangeCarnivore::ourEliteGene.get() != NULL ? StrangeCarnivore::ourEliteGene->getGeneration() : 0 ),
            nextCarnivoreGenerationDump,
            StrangeCarnivore::ourAverageAge,
            StrangeCarnivore::ourAverageSpawnCount,
            StrangeCarnivore::ourAverageFeedCount,
            ( StrangeHerbivore::ourEliteGene.get() != NULL ? StrangeHerbivore::ourEliteGene->getGeneration() : 0 ),
            nextHerbivoreGenerationDump,
            StrangeHerbivore::ourAverageAge,
            StrangeHerbivore::ourAverageSpawnCount,
            StrangeHerbivore::ourAverageFeedCount,
            StrangeWorld::getMutationLevel() );
        SetWindowText(theMainWindow, buff);
    }

    
    ReleaseDC( theMainWindow, dc );
}

void GetDefaultWindowSize(RECT* rect)
{
    rect->left = 0;
    rect->right = WORLD_WIDTH;
    rect->top = 0;
    rect->bottom = WORLD_HEIGHT;
    AdjustWindowRect(rect, WS_OVERLAPPEDWINDOW, false);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE prev, LPSTR cmd, int show)
{
#if defined( USE_GDIPLUS )
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    // Initialize GDI+.
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
#endif

    MathAccel::init();
    srand( (unsigned int)time( 0 ) );
    seedMT( (unsigned int)time( 0 ) );

    SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);

    theMainInstance = hInst;

    WNDCLASS wc;
    wc.style            = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc      = (WNDPROC)MainWndProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = theMainInstance;
    wc.hIcon            = NULL;
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = NULL;
    wc.lpszMenuName     = NULL;
    wc.lpszClassName    = L"StrangeWorld4";

    RegisterClass(&wc);

    RECT wndRect;
    GetDefaultWindowSize(&wndRect);
    theMainWindow = CreateWindow( 
        L"StrangeWorld4", APPLICATION_NAME, 
        WS_VISIBLE | WS_OVERLAPPEDWINDOW,
        100, 100, wndRect.right, wndRect.bottom,
        NULL, NULL, theMainInstance, NULL);

    MSG msg = {0};
    do
    {
        if ( gCurrentState == ePAUSE )
        {
            if ( GetMessage( &msg, 0, 0, 0 ) )
            {
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            }
        }
        else
        {
            Tick();
            if ( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
            {
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            }
        }
    }
    while( msg.message != WM_QUIT );

#if defined( USE_GDIPLUS )
    Gdiplus::GdiplusShutdown(gdiplusToken);
#endif
    return 0;
}

template <class CreatureType>
void populateWorld( std::wstring const& geneFileName )
{
    StrangeNNGene g( geneFileName );
    for ( int i = 0; i < 13; ++i )
    {
        CreatureType* pCreature = new CreatureType( new StrangeNNGene( &g ) );
        gWorld->addCreature( pCreature, true );
    }
}


LRESULT CALLBACK MainWndProc( HWND theWnd, UINT theMsg, WPARAM wParam, LPARAM lParam )
{
    switch ( theMsg )
    {
    case WM_CREATE:
        {
            theMainWindow = theWnd;

            //set the icon in the tray
            trayIcon = LoadIcon( theMainInstance, MAKEINTRESOURCE( IDI_ICON2 ) );
            NOTIFYICONDATA iconData;
            iconData.cbSize = sizeof( NOTIFYICONDATA );
            iconData.hWnd = theWnd;
            iconData.hIcon = trayIcon;
            lstrcpy(iconData.szTip, APPLICATION_NAME);
            iconData.uCallbackMessage = TRAYNOTIFY;
            iconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
            iconData.uID = TRAYID;
            Shell_NotifyIcon( NIM_ADD, &iconData );

            gDrawSensors = false;
            gDrawData = false;

            gWorld = new StrangeWorld( WORLD_WIDTH, WORLD_HEIGHT, GROWTH_RATE );
            gView = new StrangeWindowsView( theWnd, gWorld );

            populateWorld<StrangeCarnivore>( L"carnivore.gen4" );
            populateWorld<StrangeHerbivore>( L"herbivore.gen4" );

        } break;
    case TRAYNOTIFY:
        {
            if ( lParam == WM_LBUTTONDOWN )
            {
                ShowWindow( theWnd, SW_SHOWNORMAL );
                SetForegroundWindow( theWnd );
                setStatText = true;
            }
        } break;
    case WM_SYSCOMMAND:
        {
            if ( wParam == SC_MINIMIZE )
            {
                ShowWindow( theWnd, SW_MINIMIZE );
                ShowWindow( theWnd, SW_HIDE );
                setStatText = false;
            }
            else
            {
                return DefWindowProc( theWnd, theMsg, wParam, lParam );
            }
        } break;
    case WM_LBUTTONDOWN:
        {
            int xPos = LOWORD( lParam );
            int yPos = HIWORD( lParam );
            OperationCreatureHitTest hittestOp( xPos, yPos );
            gWorld->globalOperation( &hittestOp );
            gWorld->toggleCreatureSelection( hittestOp.creatureHit );
            InvalidateRect( theWnd, NULL, FALSE );
        } break;
    case WM_RBUTTONDOWN:
        {
            gCurrentState = eONESTEP;
        } break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC pdc = BeginPaint( theWnd, &ps );
            EndPaint( theWnd, &ps );
            Tick();
            if ( gCurrentState == eFAST )
            {
                HDC dc = GetDC( theMainWindow );
                HBRUSH black = CreateSolidBrush( RGB( 0, 0, 0 ) );
                HBRUSH oldBrush = (HBRUSH)SelectObject( dc, black );
                RECT r;
                GetClientRect( theMainWindow, &r );
                Rectangle( dc, r.left, r.top, r.right, r.bottom );
                SelectObject( dc, oldBrush );
                ReleaseDC( theMainWindow, dc );
                DeleteObject( black );
            }
            
        } break;
    case WM_CHAR:
        {
            if ( wParam == ' ' )
            {
                if ( gCurrentState == ePAUSE )
                    gCurrentState = eNORMAL;
                else if ( gCurrentState == eFAST )
                    gCurrentState = eNORMAL;
                else
                    gCurrentState = eFAST;
                InvalidateRect( theWnd, NULL, FALSE );
            }
            if ( wParam == 's' )
            {
                if ( !gDrawSensors )
                {
                    gDrawSensors = true;
                    gDrawData = false;
                }
                else if ( gDrawSensors && !gDrawData )
                {
                    gDrawData = true;
                }
                else
                {
                    gDrawSensors = false;
                    gDrawData = false;
                }

                InvalidateRect( theWnd, NULL, FALSE );
            }
            else if ( wParam >= '0' && wParam <= '9' )
            {
                StrangeWorld::setMutationLevel( (unsigned int)wParam - '0' );
            }
            else if ( wParam == 'c' )
            {
                gWorld->clearAllSelection();
                InvalidateRect( theWnd, NULL, FALSE );
            }
            else if ( wParam == 'a' )
            {
                gWorld->selectionAll();
                InvalidateRect( theWnd, NULL, FALSE );
            }
            else if ( wParam == 'A' )
            {
                // TODO: Start as selected.
            }
            else if ( wParam == 'w' )
            {
                if (StrangeCarnivore::ourEliteGene.get() != NULL)
                    StrangeCarnivore::ourEliteGene->saveGene( L"carnivore.gen4" );
                if (StrangeHerbivore::ourEliteGene.get() != NULL)
                    StrangeHerbivore::ourEliteGene->saveGene( L"herbivore.gen4" );
            }
            else if ( wParam == 'R' )
            {
                // Resize back to normal size.
                RECT wndRect;
                GetDefaultWindowSize(&wndRect);
                SetWindowPos(theWnd, NULL, 0, 0, wndRect.right, wndRect.bottom, SWP_NOMOVE|SWP_NOZORDER);
            }

        } break;
    case WM_CLOSE:
        {
            if (StrangeCarnivore::ourEliteGene.get() != NULL)
                StrangeCarnivore::ourEliteGene->saveGene( L"carnivore.gen4" );
            if (StrangeHerbivore::ourEliteGene.get() != NULL)
                StrangeHerbivore::ourEliteGene->saveGene( L"herbivore.gen4" );

            DestroyIcon(trayIcon);
            NOTIFYICONDATA iconData;
            iconData.cbSize = sizeof(NOTIFYICONDATA);
            iconData.hWnd = theWnd;
            iconData.uFlags = NULL;
            iconData.uID = TRAYID;
            Shell_NotifyIcon(NIM_DELETE, &iconData);

            DestroyWindow(theWnd);
        } break;
    case WM_DESTROY:
        {
            DefWindowProc(theWnd, theMsg, wParam, lParam);
            PostQuitMessage(0);
        };
     default: return( DefWindowProc(theWnd, theMsg, wParam, lParam));

    }
    return 0;
}

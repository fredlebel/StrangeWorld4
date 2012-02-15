#include <windows.h>

#include "StrangeWindowsView.h"
#include "WinThread.h"
#include "StrangeWorld/World.h"
#include "StrangeWorld/Creatures/Carnivore.h"
#include "StrangeWorld/Creatures/Herbivore.h"
#include "StrangeWorld/Creatures/Grass.h"
#include "StrangeWorld/Operations/OpRender.h"
#include "StrangeWorld/NeuralNetwork/NNGene.h"
#include "StrangeWorld/NeuralNetwork/NeuralNetwork.h"
#include "StrangeWorld/MathAccel.h"
#include "StrangeWorld/FastRand.h"
#include "StrangeWorld/CollisionDetection.h"
#include "resource.h"
#include "time.h"

#define APPLICATION_NAME L"StrangeWorld 5.0"
#define CARNIVORE_GENEFILE L"carnivore.gen5"
#define HERBIVORE_GENEFILE L"herbivore.gen5"
#define CARNIVORE_GENEFILE_GENERATION L"carnivore_%i.gen5"
#define HERBIVORE_GENEFILE_GENERATION L"herbivore_%i.gen5"


HINSTANCE theMainInstance;
HWND theMainWindow;

LRESULT CALLBACK MainWndProc(HWND theWnd, UINT theMsg, WPARAM wParam, LPARAM lParam);

#define WORLD_WIDTH     1024
#define WORLD_HEIGHT    768

#define TRAYNOTIFY          3000
#define TRAYID              3000
HICON trayIcon;

bool gDrawSensors = false;
bool gDrawData = false;

World* gWorld = nullptr;
StrangeWindowsView* gView = nullptr;
std::vector<IThread*> gWorkerThreads;

static HDC backBufferDC = nullptr;
static HBITMAP backBufferBitmap = nullptr;

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
            if ( nullptr == backBufferDC )
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
        if ( Carnivore::ourEliteGene.get() != nullptr )
            Carnivore::ourEliteGene->saveGene( CARNIVORE_GENEFILE );
        if ( Herbivore::ourEliteGene.get() != nullptr )
            Herbivore::ourEliteGene->saveGene( HERBIVORE_GENEFILE );
    }

    static int nextCarnivoreGenerationDump = 0;
    static int nextHerbivoreGenerationDump = 0;

    if ( tickDone )
    {
        static unsigned int generationDumpInterval = 10000;

        if ( Carnivore::ourEliteGene.get() != nullptr )
        {
            int currentGeneration = Carnivore::ourEliteGene->getGeneration();
            if ( nextCarnivoreGenerationDump == 0 )
                nextCarnivoreGenerationDump = (currentGeneration / generationDumpInterval) * 
                generationDumpInterval + generationDumpInterval;
            if ( currentGeneration > nextCarnivoreGenerationDump )
            {
                wchar_t genFilename[80];
                wsprintf(genFilename, CARNIVORE_GENEFILE_GENERATION, nextCarnivoreGenerationDump );
                Carnivore::ourEliteGene->saveGene( genFilename );
                nextCarnivoreGenerationDump += generationDumpInterval;
            }
        }

        if ( Herbivore::ourEliteGene.get() != nullptr )
        {
            int currentGeneration = Herbivore::ourEliteGene->getGeneration();
            if ( nextHerbivoreGenerationDump == 0 )
                nextHerbivoreGenerationDump = (currentGeneration / generationDumpInterval) * 
                generationDumpInterval + generationDumpInterval;
            if ( currentGeneration > nextHerbivoreGenerationDump )
            {
                wchar_t genFilename[80];
                wsprintf(genFilename, HERBIVORE_GENEFILE_GENERATION, nextHerbivoreGenerationDump );
                Herbivore::ourEliteGene->saveGene( genFilename );
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
        wsprintf(buff, APPLICATION_NAME L" - %i - Carnivore:(g:%i/%i-a:%i-s:%i-f:%i) Herbivore:(g:%i/%i-a:%i-s:%i-f:%i) Mutation(%i)", gWorld->getTickCount(),
            ( Carnivore::ourEliteGene.get() != nullptr ? Carnivore::ourEliteGene->getGeneration() : 0 ),
            nextCarnivoreGenerationDump,
            Carnivore::ourAverageAge,
            Carnivore::ourAverageSpawnCount,
            Carnivore::ourAverageFeedCount,
            ( Herbivore::ourEliteGene.get() != nullptr ? Herbivore::ourEliteGene->getGeneration() : 0 ),
            nextHerbivoreGenerationDump,
            Herbivore::ourAverageAge,
            Herbivore::ourAverageSpawnCount,
            Herbivore::ourAverageFeedCount,
            World::getMutationLevel() );
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
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
#endif

    MathAccel::init();
    srand( (unsigned int)time( 0 ) );
    seedMT( (unsigned int)time( 0 ) );

    gWorkerThreads.push_back( new WinThread() );
    gWorkerThreads.push_back( new WinThread() );
    gWorkerThreads.push_back( new WinThread() );
    gWorkerThreads.push_back( new WinThread() );

    SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);

    theMainInstance = hInst;

    WNDCLASS wc;
    wc.style            = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc      = (WNDPROC)MainWndProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = theMainInstance;
    wc.hIcon            = nullptr;
    wc.hCursor          = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground    = nullptr;
    wc.lpszMenuName     = nullptr;
    wc.lpszClassName    = L"StrangeWorld5";

    RegisterClass(&wc);

    RECT wndRect;
    GetDefaultWindowSize(&wndRect);
    theMainWindow = CreateWindow( 
        L"StrangeWorld5", APPLICATION_NAME, 
        WS_VISIBLE | WS_OVERLAPPEDWINDOW,
        100, 100, wndRect.right - wndRect.left, wndRect.bottom - wndRect.top,
        nullptr, nullptr, theMainInstance, nullptr);

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
    NNGene g( geneFileName );
    for ( int i = 0; i < 13; ++i )
    {
        CreatureType* pCreature = new CreatureType( new NNGene( &g ) );
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

            gWorld = new World( WORLD_WIDTH, WORLD_HEIGHT, gWorkerThreads );
            gView = new StrangeWindowsView( theWnd, gWorld );

            populateWorld<Carnivore>( CARNIVORE_GENEFILE );
            populateWorld<Herbivore>( HERBIVORE_GENEFILE );

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

            Creature* clickedCreature = nullptr;
            auto hitTestFn = [&](Creature* c) -> bool
            {
                if ( hitTest( c, xPos, yPos ) )
                {
                    clickedCreature = c;
                    return false;
                }
                return true;
            };

            gWorld->operateOnAll( hitTestFn );
            if ( clickedCreature != nullptr )
                gWorld->toggleCreatureSelection( clickedCreature );
            InvalidateRect( theWnd, nullptr, FALSE );
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
                InvalidateRect( theWnd, nullptr, FALSE );
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

                InvalidateRect( theWnd, nullptr, FALSE );
            }
            else if ( wParam >= '0' && wParam <= '9' )
            {
                World::setMutationLevel( (unsigned int)wParam - '0' );
            }
            else if ( wParam == 'c' )
            {
                gWorld->clearAllSelection();
                InvalidateRect( theWnd, nullptr, FALSE );
            }
            else if ( wParam == 'a' )
            {
                gWorld->selectionAll();
                InvalidateRect( theWnd, nullptr, FALSE );
            }
            else if ( wParam == 'A' )
            {
                // TODO: Start as selected.
            }
            else if ( wParam == 'w' )
            {
                if (Carnivore::ourEliteGene.get() != nullptr)
                    Carnivore::ourEliteGene->saveGene( CARNIVORE_GENEFILE );
                if (Herbivore::ourEliteGene.get() != nullptr)
                    Herbivore::ourEliteGene->saveGene( HERBIVORE_GENEFILE );
            }
            else if ( wParam == 'R' )
            {
                // Resize back to normal size.
                RECT wndRect;
                GetDefaultWindowSize(&wndRect);
                SetWindowPos(theWnd, nullptr, 0, 0, wndRect.right - wndRect.left, wndRect.bottom - wndRect.top, SWP_NOMOVE|SWP_NOZORDER);
            }

        } break;
    case WM_CLOSE:
        {
            if (Carnivore::ourEliteGene.get() != nullptr)
                Carnivore::ourEliteGene->saveGene( CARNIVORE_GENEFILE );
            if (Herbivore::ourEliteGene.get() != nullptr)
                Herbivore::ourEliteGene->saveGene( HERBIVORE_GENEFILE );

            DestroyIcon(trayIcon);
            NOTIFYICONDATA iconData;
            iconData.cbSize = sizeof(NOTIFYICONDATA);
            iconData.hWnd = theWnd;
            iconData.uFlags = 0;
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

#include <windows.h>
#include <math.h>

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
char szClassName[ ] = "WindowsApp";
const double crad=M_PI/180;

typedef struct mypoint
{
        int x,y;
}sPoint;

typedef struct exp
{
               int x,y,d,r,status,cs;
}sExp,*pExp;


long i1;
sPoint mp;
    HDC dc;
    RECT r;
    PAINTSTRUCT ps;
    static char buffer[256];
    static char keybuffer[4];
    int count,ecount=0;
    double mf[3][2];

    HWND hwnd;               /* This is the handle for our window */

void runloop();

sExp expfx[20];

void initexp(pExp e,int x,int y)
{
     e->status = 1;
     e->x = x;
     e->y = y;
     e->d = 0;
     e->r = 4;
     e->cs = 2;
}

void initfx()
{
     for (int i=0;i<20;i++)
         expfx[i].status = 0;
     
     mf[0][0]=cos(22.5*crad);
     mf[0][1]=sin(22.5*crad);     
     mf[1][0]=cos(45*crad);
     mf[1][1]=sin(45*crad);     
     mf[2][0]=cos(67.5*crad);
     mf[2][1]=sin(67.5*crad);     
     
}

void render(pExp e)
{
     int rx,ry;
     
     if (e->status)
     {

        Ellipse(dc,e->x+e->d-(e->r>>1),e->y-(e->r>>1),e->x+e->d+(e->r>>1),e->y+(e->r>>1));
        Ellipse(dc,e->x-e->d-(e->r>>1),e->y-(e->r>>1),e->x-e->d+(e->r>>1),e->y+(e->r>>1));
        Ellipse(dc,e->x-(e->r>>1),e->y+e->d-(e->r>>1),e->x+(e->r>>1),e->y+e->d+(e->r>>1));
        Ellipse(dc,e->x-(e->r>>1),e->y-e->d-(e->r>>1),e->x+(e->r>>1),e->y-e->d+(e->r>>1));

        for (int i=0;i<3;i++)
        {
                rx = e->d*mf[i][0];
                ry = e->d*mf[i][1];
                Ellipse(dc,e->x+rx-(e->r>>1),e->y+ry-(e->r>>1),e->x+rx+(e->r>>1),e->y+ry+(e->r>>1));
                Ellipse(dc,e->x-rx-(e->r>>1),e->y+ry-(e->r>>1),e->x-rx+(e->r>>1),e->y+ry+(e->r>>1));
                Ellipse(dc,e->x+rx-(e->r>>1),e->y-ry-(e->r>>1),e->x+rx+(e->r>>1),e->y-ry+(e->r>>1));
                Ellipse(dc,e->x-rx-(e->r>>1),e->y-ry-(e->r>>1),e->x-rx+(e->r>>1),e->y-ry+(e->r>>1));
        }
        
        e->r+=e->cs;
        
        if ((e->r==20) || (e->r == 2))
           e->cs=-e->cs;

        e->d+=2;
        if (e->d>600)
        {
           e->status = 0;
           e->d = 0;
        }
     }
}


int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground =(HBRUSH)COLOR_WINDOW+1;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "Windows App",       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nFunsterStil);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    
    initfx();
    
    i1 = GetTickCount();    

    while (1)
    {

        /* Translate virtual-key messages into character messages */
            if (PeekMessage(&messages,0,0,0,PM_REMOVE))
            {
                 if (messages.message == WM_QUIT) break;
                 TranslateMessage(&messages);
                 /* Send message to WindowProcedure */
                 DispatchMessage(&messages);
            }
            runloop();
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

void runloop()
{
        HBRUSH b;

        if (mp.x > 0) mp.x-=keybuffer[0]*2;
        if (mp.x < r.right) mp.x+=keybuffer[2]*2;             
        if (mp.y > 0) mp.y-=keybuffer[1]*2;
        if (mp.y < r.bottom) mp.y+=keybuffer[3]*2;             

        dc = GetDC(hwnd);
        
 
        GetClientRect(hwnd,&r);
        
        FillRect(dc,&r,CreateSolidBrush(0));
        SelectObject(dc,CreateSolidBrush(RGB(255,255,255)));
        
        wsprintf(buffer,"Left: %d, Up: %d, Right: %d, Down: %d",keybuffer[0],keybuffer[1],keybuffer[2],keybuffer[3]);
        TextOut(dc,0,0,buffer,strlen(buffer));


        for (int i=0;i<20;i++)
            render(&expfx[i]);

          
        Ellipse(dc,mp.x-10,mp.y-10,mp.x+10,mp.y+10);
        ReleaseDC(hwnd,dc);

        i1 +=16;
        
        
        while ((long)(i1-GetTickCount())>0);
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
             mp.x = 0;
             mp.y = 0;
             break;
             
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
            
             
        case WM_KEYDOWN:
             //get scan code
             //count=(lParam & 0x00ff0000)>>16;
             
             count = lParam & (0x0000ffff); //get keycode
             
             //check last status of key at bit 30. if 0, key was up before this WM_KEYDOWN, else it was already down (repeat)
     //        count = (lParam & 0x40000000)>>30;
             switch (wParam)
             {
                    case 37:
                         keybuffer[0]=1;
                         break;
                         
                    case 38:
                         keybuffer[1]=1;
                         break;
                         
                    case 39:
                         keybuffer[2]=1;
                         break;
                    
                    case 40:
                         keybuffer[3]=1;
                         break;
             }
             break;
     
        case WM_LBUTTONDOWN:
             initexp(&expfx[ecount],LOWORD(lParam),HIWORD(lParam));
             ecount++;
             if (ecount > 19) ecount = 0;
             break;

        case WM_KEYUP:
             switch (wParam)
             {
                    case 37:
                         keybuffer[0]=0;
                         break;
                         
                    case 38:
                         keybuffer[1]=0;
                         break;
                         
                    case 39:
                         keybuffer[2]=0;
                         break;
                    
                    case 40:
                         keybuffer[3]=0;
                         break;
             }
             break;
                                   
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

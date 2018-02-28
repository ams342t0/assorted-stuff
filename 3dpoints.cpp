#include <windows.h>
#include <stdlib.h>
#include <math.h>


/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
    HWND hwnd;               /* This is the handle for our window */

ULONG sw,sh,midx,midy;
HDC dc,dc2;
HBITMAP bm;
RECT r;
HBRUSH bl,gr;
HPEN pn;
PAINTSTRUCT ps;
float px,py;
float zd,angle;
int cr;
float rad;

typedef struct p3d
{
float x,y,z;
}sp3d,*psp3d;

#define PCOUNT 10000
//sp3d p[PCOUNT];
psp3d p;
char os [256];



void merge_sort(psp3d a,int len_a,psp3d b,int len_b)
{
     if (len_a > 1)
     {
               int mid_a = len_a >> 1;
               merge_sort(a,mid_a,a+mid_a,len_a-mid_a);
     }
     
     if (len_b > 1)
     {
               int mid_b = len_b >> 1;
               merge_sort(b,mid_b,b+mid_b,len_b-mid_b);
     }
     
     int curr_a = 0, curr_b = 0, k = 0;
     sp3d temp_arr[len_a+len_b];
     
     while ((curr_a < len_a) && (curr_b < len_b))
     {
           if (a[curr_a].z >= b[curr_b].z)
           {
                           temp_arr[k] = a[curr_a];
                           curr_a++;
           }
           else
           {
                           temp_arr[k] = b[curr_b];
                           curr_b++;
           }
           k++;
     }
     
     while (curr_a < len_a)
     {
           temp_arr[k] = a[curr_a];
           curr_a++;
           k++;
     }
     
     while (curr_b < len_b)
     {
           temp_arr[k] = b[curr_b];
           curr_b++;
           k++;
     }
     
     for (int k=0;k<len_a+len_b;k++)
     {
         a[k] = temp_arr[k];
     }
}

void drawloop();

/*  Make the class name into a global variable  */
char szClassName[ ] = "WindowsApp";

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
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

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
    while (1)
    {
          if (PeekMessage(&messages,0,0,0,PM_REMOVE))
          {
                if (messages.message==WM_QUIT) break;
                TranslateMessage(&messages);
                DispatchMessage(&messages);
          }
          drawloop();
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}



HBRUSH hb;



void drawloop()
{
             FillRect(dc2,&r,bl);
             SelectObject(dc2,pn);
             merge_sort(p,1,p+1,PCOUNT-1);
             for(int i=0;i<PCOUNT;i++)
             {
				  px = p[i].x*(float)cos(rad)-p[i].y*(float)sin(rad);
				  py = p[i].x*(float)sin(rad)+p[i].y*(float)cos(rad);
				  p[i].x = px;
				  p[i].y = py;
				  px = p[i].x*zd/p[i].z;
				  py = p[i].y*zd/p[i].z;
				  cr = (((int)p[i].z)>>7)-15;
				  if (hb != NULL) DeleteObject(hb);
				  hb = CreateSolidBrush(RGB(0,255-(cr*16),0));
	              SelectObject(dc2,hb);
				  Ellipse(dc2,(int)px-cr+midx,midy-(int)py+cr,(int)px+cr+midx,midy-(int)py-cr);
				  p[i].z-=1.0f;
				  if (p[i].z<=1.0) p[i].z=2048.0;
             }

			 BitBlt(dc,0,0,r.right ,r.bottom,dc2,0,0,SRCCOPY);

}

/*  This function is called by the Windows function DispatchMessage()  */





LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
		case WM_CHAR:
			if (wParam == VK_ESCAPE) DestroyWindow(hwnd);
			break;

        case WM_CREATE:
			{
             p=(psp3d)malloc(sizeof(sp3d)*PCOUNT);
             srand(rand());
             for(int i=0;i<PCOUNT;i++)
             {
                p[i].x = (float)683.0-((float)1366.0*rand()/RAND_MAX);
                p[i].y = (float)384.0-((float)768.0*rand()/RAND_MAX);
                p[i].z = (float)2048.0*rand()/RAND_MAX;
             }
             zd=2048.0;
             rad= 3.1416f/360.0f;   
             angle=0.0;
			}
			dc = GetDC(hwnd);
			dc2 = CreateCompatibleDC(dc);
			bl = CreateSolidBrush(0);
			pn=CreatePen(PS_NULL,0,0);
            break;
                 
        case WM_SIZE:
             sh = HIWORD(lParam);
             sw = LOWORD(lParam);
             midx = sw >> 1;
             midy = sh >> 1;
             GetClientRect(hwnd,&r);
			 if (bm !=NULL) DeleteObject(bm);
			 bm = CreateCompatibleBitmap(dc,sw,sh);
			 SelectObject(dc2,bm);
             break;
             
        case WM_DESTROY:
			DeleteObject(bm);
			DeleteDC(dc2);
            ReleaseDC(hwnd,dc);
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;

        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

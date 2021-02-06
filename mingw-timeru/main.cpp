#define WIN32_LEAN_AND_MEAN
#define UNICODE
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <commctrl.h>
#include <commdlg.h>
#include <mmsystem.h>
#include "resource.h"

#define MYSTRING TEXT("WINAPP")

typedef struct psettings
{
	int sx,sy,sw,sh,titlesize,timesize,tcount;
	COLORREF ctitle,ctime,cbackground;
	wchar_t alarmfile[256];
}sSettings;

OPENFILENAME ofn;
CHOOSECOLOR cc;
COLORREF custcolors[16];
LV_COLUMN lvc;
LV_ITEM	  lvi;
HBRUSH blk;
HWND hMain;
HWND hCP;
HINSTANCE hInstMain;
HDC hdc,hdc1;
RECT r;
PAINTSTRUCT ps;
POINT pt;
HFONT hf,hf2;
LOGFONT lf;
BOOL winvisible,running;
SIZE szt;
HBITMAP bb;
int blink=1;
int px,py,tpx,tpy;
int ni=99771;
wchar_t strTime[256];
wchar_t strTitle[256];
//char strTmp[64];
wchar_t strTmp[64];
long lCountDown;
long lTickCurrent,lTick,lRefresh;
sSettings ts;

void regwindow();
void createwindow();
void runloop();
void getcounter();
void formattime();
void loadsettings();
void savesettings();
void loadlist();
void initvars();
void loaddefaults();
void breaktimestr(wchar_t *s);
void setsize();
int menucheck();
void GetLVItemText(HWND lv,wchar_t *s);

void setmenucheck(int);
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
BOOL CALLBACK DlgProc(HWND,UINT,WPARAM,LPARAM);


int WINAPI WinMain(HINSTANCE hi,HINSTANCE hp,LPSTR cmdln,int s)
{
	MSG m;
	HACCEL ha;
	INITCOMMONCONTROLSEX ice;

	hInstMain = hi;
	regwindow();
	loadsettings();
	createwindow();
	initvars();

	//InitCommonControls();

	ice.dwSize = sizeof(INITCOMMONCONTROLSEX);
	ice.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&ice);
	hCP = CreateDialog(hi,MAKEINTRESOURCE(IDD_FORMVIEW),hMain,DlgProc);
	if (hCP == INVALID_HANDLE_VALUE) return 0;
	ShowWindow(hCP,SW_SHOWNORMAL);
	ha=LoadAccelerators(hInstMain,MAKEINTRESOURCE(IDR_ACCELERATOR1));

	loadlist();
	running = false;
	
/*	while (true)
	{
		if (PeekMessage(&m,0,0,0,PM_REMOVE))
		{
			if (m.message == WM_QUIT) break;
			TranslateMessage(&m);
			DispatchMessage(&m);
		}
		runloop();
	}*/

	while (GetMessage(&m,0,0,0) != 0)
	{
		if (!IsDialogMessage(hCP,&m))
		{
			TranslateMessage(&m);
			DispatchMessage(&m);
			TranslateAccelerator(hMain,ha,&m);
		}
	}
	savesettings();
	return (m.wParam);
}


void regwindow()
{
	WNDCLASSEX w;
	w.cbSize = sizeof(WNDCLASSEX);
	w.hInstance = hInstMain;
	w.style = CS_HREDRAW | CS_VREDRAW;
	w.lpszClassName = MYSTRING;
	w.lpfnWndProc = WndProc;
	w.lpszMenuName = NULL;
	w.hbrBackground = CreateSolidBrush(RGB(0,0,0));
	w.hIcon = LoadIcon(hInstMain,MAKEINTRESOURCE(IDI_ICON3));
	w.hIconSm = w.hIcon;
	w.hCursor = LoadCursor(NULL,IDC_ARROW);
	w.cbWndExtra = 0;
	w.cbClsExtra = 0;
	if (!RegisterClassEx(&w)) exit(0);
}

void createwindow()
{
	hMain = CreateWindowEx(WS_EX_TOPMOST,MYSTRING,NULL,WS_DLGFRAME  |WS_SIZEBOX,
		ts.sx,ts.sy,ts.sw,ts.sh,NULL,NULL,hInstMain,NULL);
}

LRESULT CALLBACK WndProc(HWND h,UINT m,WPARAM wp,LPARAM lp)
{
	switch (m)
	{
	case WM_COMMAND:
		if(HIWORD(wp)==1)
		{
			switch(LOWORD(wp))
			{
			case ID_ALTS:
				SendDlgItemMessage(hCP,IDC_RUN,BM_CLICK,0,0);
				SetFocus(hCP);
				break;
			}
		}
		break;

	case WM_TIMER:
		runloop();
		break;

	case WM_CREATE:
		winvisible = FALSE;
		break;

	case WM_SIZE:
		ts.sw = LOWORD(lp);
		ts.sh = HIWORD(lp);
		GetClientRect(h,&r);
		setsize();
		break;

	case WM_MOVE:
		ts.sx = LOWORD(lp);
		ts.sy = HIWORD(lp);
		break;

	case WM_LBUTTONDOWN:
		SendMessage(h,WM_NCLBUTTONDOWN,HTCAPTION,0);
		break;

	case WM_DESTROY:
		ReleaseDC(hMain,hdc1);
		DeleteDC(hdc);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(h,m,wp,lp);
	}

	return 0;
}


BOOL CALLBACK DlgProc(HWND h,UINT m,WPARAM wp,LPARAM lp)
{
	switch (m)
	{
	case WM_NOTIFY:
		switch (((LPNMHDR)lp)->idFrom)
		{
		case IDC_LIST1:
			switch (((LPNMHDR)lp)->code )
			{

			case LVN_ITEMCHANGED:
				if (((NM_LISTVIEW*)(lp))->iItem != ni)
				{
					ni = ((NM_LISTVIEW*)(lp))->iItem;
					ListView_GetItemText(GetDlgItem(h,IDC_LIST1),ni,0,strTmp,64);
					SetDlgItemText(h,IDC_TITLE,strTmp);
					ListView_GetItemText(GetDlgItem(h,IDC_LIST1),ni,1,strTmp,64);
					breaktimestr(strTmp);
				}
				break;

			}
			break;
		}
		break;


	case WM_INITDIALOG:
		SetMenu(h,LoadMenu(hInstMain,MAKEINTRESOURCE(IDR_MENU2)));
		SetDlgItemText(h,IDC_HOUR,TEXT("0"));
		SetDlgItemText(h,IDC_MINUTE,TEXT("1"));
		SetDlgItemText(h,IDC_SECONDS,TEXT("0"));
		SetDlgItemText(h,IDC_RUN,TEXT("&START"));
		SetDlgItemText(h,IDC_TITLE,TEXT("TIME"));

		ListView_SetExtendedListViewStyle(GetDlgItem(h,IDC_LIST1), LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		wcscpy(strTmp,TEXT("Title"));
		lvc.pszText = strTmp;
		lvc.cx = 150;
		ListView_InsertColumn(GetDlgItem(h,IDC_LIST1),1,&lvc);
		wcscpy(strTmp,TEXT("Time"));
		lvc.pszText = strTmp;
		lvc.cx = 100;
		ListView_InsertColumn(GetDlgItem(h,IDC_LIST1),2,&lvc);
		
		break;

	case WM_COMMAND:
		if (!HIWORD(wp))
		{
			ZeroMemory(&cc,sizeof(CHOOSECOLOR));
			cc.lStructSize = sizeof(CHOOSECOLOR);
			cc.hwndOwner = h;
			cc.Flags = CC_FULLOPEN | CC_RGBINIT;
			cc.lpCustColors  = custcolors;
			
			switch (LOWORD(wp))
			{
			case IDSHUTDOWN:
				//setmenucheck(!menucheck());
				setmenucheck(1);
				break;

			case ID_TITLECOLOR:
				cc.rgbResult = ts.ctitle;
				if (ChooseColor(&cc)) ts.ctitle = cc.rgbResult;
				break;

			case ID_TIMECOLOR:
				cc.rgbResult =ts.ctime;
				if (ChooseColor(&cc)) ts.ctime = cc.rgbResult;
				break;

			case ID_OPTIONS_DEFAULTS:
				loaddefaults();
				MoveWindow(hMain,ts.sx,ts.sy,ts.sw,ts.sh,true);
				break;

			case ID_BACKGROUND:
				cc.rgbResult =ts.cbackground;
				if (ChooseColor(&cc)) 
				{
					ts.cbackground = cc.rgbResult;
					if (blk != NULL) DeleteObject(blk);
					blk = CreateSolidBrush(ts.cbackground);
				}
				break;

			case ID_ALARM:
				ZeroMemory(&ofn,sizeof(OPENFILENAME));
				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.hwndOwner = h;
				ofn.nMaxFile = 255;
				ofn.Flags = OFN_EXPLORER;
				ofn.lpstrFilter = TEXT("WAVE FILE\0*.wav\0");
				ofn.lpstrFile = ts.alarmfile;
				if (!GetOpenFileName(&ofn))
				{
					wcscpy(ts.alarmfile,TEXT("alarm.wav"));
				}
				break;
			}
		}
		switch (LOWORD(wp))
		{
		case IDC_RUN:
			if (!running && winvisible)
			{
				SetDlgItemText(h,IDC_RUN,TEXT("&START"));
				ShowWindow(hMain,SW_HIDE);
				winvisible = false;
				return 1;
			}
			if (winvisible) 
			{
				if (running)
					if (MessageBox(h,TEXT("Stop timer?"),TEXT("Timer is still running"),MB_ICONEXCLAMATION | MB_YESNO)==IDNO) return 1;
				running = false;
				winvisible = false;
				ShowWindow(hMain,SW_HIDE);
				SetDlgItemText(h,IDC_RUN,TEXT("&START"));
				SendMessage(h,WM_SETTEXT,(WPARAM)NULL,(LPARAM)TEXT("TIMER"));
				KillTimer(hMain,0x8001);
			}
			else 
			{
				getcounter();
				GetDlgItemText(hCP,IDC_TITLE,strTitle,50);
				running = true;
				winvisible = true;
				SetDlgItemText(h,IDC_RUN,TEXT("&STOP"));
				blk = CreateSolidBrush(ts.cbackground);
				ShowWindow(hMain,SW_SHOWNORMAL);
				setsize();
				SetTimer(hMain,0x8001,33,NULL);
				lTick = GetTickCount();
				runloop();
			}
			break;

		case IDC_ADD:
			ts.tcount++;
			getcounter();
			lvi.mask = LVIF_TEXT;
			lvi.iItem = ListView_GetItemCount(GetDlgItem(h,IDC_LIST1));
			lvi.iSubItem = 0;
			GetDlgItemText(h,IDC_TITLE,strTmp,50);
			lvi.pszText = strTmp;
			ListView_InsertItem(GetDlgItem(h,IDC_LIST1),&lvi);
			lvi.iSubItem = 1;
			formattime();
			lvi.pszText = strTime;
			ListView_SetItem(GetDlgItem(h,IDC_LIST1),&lvi);
			break;

		case IDC_DELETE:
			if (ListView_GetItemCount(GetDlgItem(h,IDC_LIST1))==0) return 1;
			ts.tcount--;
			ListView_DeleteItem(GetDlgItem(h,IDC_LIST1),ni);
			break;
		}
		break;

	case WM_CLOSE:
		if (running)
			if (MessageBox(h,TEXT("Continue closing?"),TEXT("Timer is still running"),MB_ICONEXCLAMATION | MB_YESNO)==IDNO) return 1;
		EndDialog(h,0);
		PostQuitMessage(0);	
		break;

	default:
		return 0;
	}
	return 1;
}

void runloop()
{
	lTickCurrent = GetTickCount();
	if (lTickCurrent >= lTick)
	{
			FillRect(hdc,&r,blk);
			if (running)
			{
				formattime();
				if (lCountDown <= 0) 
				{
					running = false;
					if (SendMessage(GetDlgItem(hCP,IDC_ALARM),BM_GETCHECK,0,0)==BST_CHECKED)
					{
						PlaySound(ts.alarmfile,NULL,SND_FILENAME | SND_ASYNC);
					}
					SetDlgItemText(hCP,IDC_RUN,TEXT("CLOSE"));
					SendMessage(hCP,WM_SETTEXT,NULL,(LPARAM)TEXT("TIMER"));
				}
				lCountDown--;
				FillRect(hdc,&r,blk);
				SelectObject(hdc,hf);
				SetTextColor(hdc,ts.ctime);
				TextOut(hdc,px,py,strTime,wcslen(strTime));		
			}
			else
			{
				if (blink)
				{
					SetTextColor(hdc,ts.cbackground);
					blink=0;
				}
				else
				{
					SetTextColor(hdc,ts.ctime);
					blink=1;
				}
				SelectObject(hdc,hf);
				TextOut(hdc,px,py,strTime,wcslen(strTime));		
			}
			SelectObject(hdc,hf2);
			SetTextColor(hdc,ts.ctitle);
			TextOut(hdc,tpx,tpy,strTitle,wcslen(strTitle));
			BitBlt(hdc1,0,0,r.right,r.bottom,hdc,0,0,SRCCOPY);
			lTick += 1000;
	}
}


void getcounter()
{
	GetDlgItemText(hCP,IDC_HOUR,strTmp,4);
	lCountDown=(long)(_wtof(strTmp)*3600);
	GetDlgItemText(hCP,IDC_MINUTE,strTmp,4);
	lCountDown+=(long)(_wtof(strTmp)*60);
	GetDlgItemText(hCP,IDC_SECONDS,strTmp,4);
	lCountDown+=(long)_wtof(strTmp);
}

void formattime()
{
	int th=0,tm=0,ts=0;
	int r;

	th = lCountDown / 3600;
	r  = lCountDown % 3600;
	tm = r / 60;
	r  = r % 60;
	ts = r;
	wsprintf(strTime,TEXT("%02d:%02d:%02d"),th,tm,ts);
	SendMessage(hCP,WM_SETTEXT,(WPARAM)NULL,(LPARAM)strTime);

}

void initvars()
{
	GetObject(GetStockObject(DEFAULT_GUI_FONT),sizeof(LOGFONT),&lf);
	lf.lfHeight = ts.timesize;
	wcscpy(lf.lfFaceName,TEXT("IMPACT"));
	hf = CreateFontIndirect(&lf);
	GetObject(GetStockObject(DEFAULT_GUI_FONT),sizeof(LOGFONT),&lf);
	lf.lfHeight = ts.titlesize;
	wcscpy(lf.lfFaceName,TEXT("IMPACT"));
	hf2 = CreateFontIndirect(&lf);
	hdc1 = GetDC(hMain);
	hdc = CreateCompatibleDC(hdc1);
	SetBkMode(hdc,TRANSPARENT);
	wcscpy(strTime,TEXT("00:00:00"));
}

void breaktimestr(wchar_t *s)
{
	wchar_t *ps;
	wchar_t t[12];
	int n;

	memset(t,0,24);

	ps = wcschr(s,':');
	n = ps - s;
	memcpy(t,s,n*2);
	s+=n;
	s++;
	SetDlgItemText(hCP,IDC_HOUR,t);
	ps = wcschr(s,':');
	n = ps - s;
	memcpy(t,s,n*2);
	s+=n;
	s++;
	SetDlgItemText(hCP,IDC_MINUTE,t);
	SetDlgItemText(hCP,IDC_SECONDS,s);
}

void savesettings()
{
	HANDLE hfile;
	DWORD br;

	hfile = CreateFile(TEXT("settings.dat"),GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hfile == INVALID_HANDLE_VALUE) 
	{
		CloseHandle(hfile);
		return;
	}

	WriteFile(hfile,&ts,sizeof(ts),&br,NULL);
	if (br != sizeof(ts)) 
	{
		CloseHandle(hfile);
		return;
	}

	if (ts.tcount > 0)
	{
		for (int tni = 0;tni < ts.tcount;tni++)
		{
			ListView_GetItemText(GetDlgItem(hCP,IDC_LIST1),tni,0,strTmp,64);
			WriteFile(hfile,strTmp,128,&br,NULL);
			ListView_GetItemText(GetDlgItem(hCP,IDC_LIST1),tni,1,strTmp,64);
			WriteFile(hfile,strTmp,128,&br,NULL);
		}
	}
	CloseHandle(hfile);
}

void loaddefaults()
{
	ts.sw = 320;
	ts.sh = 150;
	ts.sx = 0;
	ts.sy = 0;
	ts.ctitle = RGB(255,255,255);
	ts.ctime = RGB(255,255,255);
	ts.cbackground = 0;
	wcscpy(ts.alarmfile,TEXT("alarm.wav"));
}

void loadsettings()
{
	HANDLE hfile;
	DWORD br;

	hfile = CreateFile(TEXT("settings.dat"),GENERIC_READ,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hfile == INVALID_HANDLE_VALUE)  
	{
		CloseHandle(hfile);
		loaddefaults();
		return;
	}

	ReadFile(hfile,&ts,sizeof(ts),&br,NULL);
	if (br != sizeof(ts))
	{
		loaddefaults();
		CloseHandle(hfile);
		return;
	}
	
	CloseHandle(hfile);
	if (CreateFile(ts.alarmfile,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL)==INVALID_HANDLE_VALUE)
	{
		wcscpy(ts.alarmfile,TEXT("alarm.wav"));
	}
}


void loadlist()
{
	HANDLE hfile;
	DWORD br;

	if (ts.tcount > 0)
	{
		hfile = CreateFile(TEXT("settings.dat"),GENERIC_READ,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		SetFilePointer(hfile,sizeof(ts),0,FILE_BEGIN);
		for (int tni = 0;tni < ts.tcount;tni++)
		{
			ReadFile(hfile,strTmp,128,&br,NULL);
			lvi.mask = LVIF_TEXT;
			lvi.iItem = tni;
			lvi.iSubItem = 0;
			lvi.pszText = strTmp;
			ListView_InsertItem(GetDlgItem(hCP,IDC_LIST1),&lvi);
			ReadFile(hfile,strTmp,128,&br,NULL);
			lvi.iSubItem = 1;
			lvi.pszText = strTmp;
			ListView_SetItem(GetDlgItem(hCP,IDC_LIST1),&lvi);

		}
		CloseHandle(hfile);
	}
}


void setsize()
{
		ts.titlesize = (int)ts.sh*0.25f;
		ts.timesize  = (int)ts.sh*0.75f;
		lf.lfHeight = ts.timesize;

		if (hf != NULL)	DeleteObject(hf);
		hf = CreateFontIndirect(&lf);

		lf.lfHeight = ts.titlesize;
		if (hf2 != NULL)	DeleteObject(hf2);
		hf2 = CreateFontIndirect(&lf);

		if (bb != NULL) DeleteObject(bb);
		bb = CreateCompatibleBitmap(hdc1,ts.sw,ts.sh);
		SelectObject(hdc,bb);

		SelectObject(hdc,hf2);
		GetTextExtentPoint(hdc,strTitle,wcslen(strTitle),&szt);
		tpx = (ts.sw-szt.cx)>>1;
		tpy = 0;
		py = szt.cy;

		SelectObject(hdc,hf);
		GetTextExtentPoint(hdc,strTime,wcslen(strTime),&szt);
		px = (ts.sw-szt.cx)>>1;
}

int menucheck()
{
	MENUITEMINFO mii;

	GetMenuItemInfo(LoadMenu(hInstMain,MAKEINTRESOURCE(IDR_MENU2)),IDSHUTDOWN,FALSE,&mii);
	if (mii.fState & MFS_CHECKED) return 1;
	return 0;
}

void setmenucheck(int i)
{
	if (i==0)
		CheckMenuItem(LoadMenu(hInstMain,MAKEINTRESOURCE(IDR_MENU2)),IDSHUTDOWN,MF_UNCHECKED);
	else
		CheckMenuItem(LoadMenu(hInstMain,MAKEINTRESOURCE(IDR_MENU2)),IDSHUTDOWN,MF_CHECKED);
}

void GetLVItemText(HWND lv,wchar_t *s)
{
	LV_ITEM i;

	SendMessage(hCP,LVM_GETITEMTEXT,IDC_LIST1,(LPARAM)&i);
	wcscpy(s,i.pszText);
}
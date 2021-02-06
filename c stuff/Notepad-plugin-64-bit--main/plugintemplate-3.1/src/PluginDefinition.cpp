//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "PluginDefinition.h"
#include "DockingFeature\GoToLineDlg.h"
#include "menuCmdID.h"

//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;

//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE /*hModule*/)
{

}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{

    //--------------------------------------------//
    //-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
    //--------------------------------------------//
    // with function :
    // setCommand(int index,                      // zero based number to indicate the order of command
    //            TCHAR *commandName,             // the command name that you want to see in plugin menu
    //            PFUNCPLUGINCMD functionPointer, // the symbol of function (function pointer) associated with this command. The body should be defined below. See Step 4.
    //            ShortcutKey *shortcut,          // optional. Define a shortcut to trigger this command
    //            bool check0nInit                // optional. Make this menu item be checked visually
    //            );
	ShortcutKey k;
	k._isAlt = true;
	k._isCtrl = true;
	k._isShift = true;
	k._key = 'x';
	setCommand(0, TEXT("Show selected word"), showWord,&k, true);
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
}


//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit) 
{
    if (index >= nbFunc)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//
void hello()
{
    // Open a new document
    ::SendMessage(nppData._nppHandle, NPPM_MENUCOMMAND, 0, IDM_FILE_NEW);

    // Get the current scintilla
    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;
    HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;

    // Say hello now :
    // Scintilla control has no Unicode mode, so we use (char *) here
    ::SendMessage(curScintilla, SCI_SETTEXT, 0, (LPARAM)"Hello, Notepad++!");
}


void showWord()
{
	TCHAR selword[64];
	int r;
	sqlite3* db;
	char cmd[256];
	char *data="Ok";
	char *errMsg;
	static char sqlout[256];
	static wchar_t wsqlout[256];

	auto sqlcallback = [](void* data, int argc, char** argv, char** colName) -> int	{
		const char* cols[] = { "Opcode: ","Desc: ","Operands: ","Flags: " };
		strset(sqlout, 0);
		for (int i = 0; i < argc; i++)
		{
			strcat(sqlout, cols[i]);
			(argv[i]? strcat(sqlout, argv[i]):strcat(sqlout,""));
			strcat(sqlout, "\n\n");
		}
		mbstowcs(wsqlout, sqlout, strlen(sqlout));
		return 0;
	};

	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTWORD, 64, (LPARAM)&selword);

	r = sqlite3_open(".\\plugins\\nppPluginTemplate\\opcodes.dat", &db);

	if (!r)
	{ 
		char tmp[256] = { 0 };
		wcstombs(tmp, selword, wcslen(selword));
		wcsset(wsqlout, 0);
		strcpy(cmd, "select * from ops where opc = upper(\"");
		strcat(cmd, tmp);
		strcat(cmd, "\");");
		r = sqlite3_exec(db, cmd, sqlcallback, (void*)data, &errMsg);
		sqlite3_close(db);
	}

	if (wcslen(selword) && wcslen(wsqlout))	::MessageBox(NULL, wsqlout,selword, MB_OK);
}

void helloDlg()
{
    ::MessageBox(NULL, TEXT("Hello, Notepad++!"), TEXT("Notepad++ Plugin Template"), MB_OK);
}

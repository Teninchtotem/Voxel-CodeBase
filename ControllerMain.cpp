///////////////////////////////////////////////////////////////////////////////
// ControllerMain.cpp
// ==================
// Derived Controller class for main window
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2006-07-09
// UPDATED: 2014-01-01
///////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <commctrl.h>                   // common controls
#include <sstream>
#include "ControllerMain.h"
#include "Controls.h"
#include "Log.h"
using namespace Win;


// handle events(messages) on all child windows that belong to the parent window.
// For example, close all child windows when the parent got WM_CLOSE message.
// lParam can be used to specify a event or message.
bool CALLBACK enumerateChildren(HWND childHandle, LPARAM lParam);



ControllerMain::ControllerMain() : glHandle(0), formHandle(0)
{
}


int ControllerMain::command(int id, int cmd, LPARAM msg)
{
	int temp;
	MENUITEMINFO holder;
	holder.cbSize = sizeof(MENUITEMINFO);
    switch(id)
	{
	case ID_FILE_EXIT:
		::PostMessage(handle, WM_CLOSE, 0, 0);
		break;
	case IDC_SYNC_DISPLAY:
		//Toggle checkbox
		if (GetMenuState(GetMenu(handle), IDC_SYNC_DISPLAY, MF_BYCOMMAND) == MF_CHECKED)
			CheckMenuItem(GetMenu(handle), IDC_SYNC_DISPLAY, MF_UNCHECKED);
		else
			CheckMenuItem(GetMenu(handle), IDC_SYNC_DISPLAY, MF_CHECKED);

		::PostMessage(formHandle, WM_COMMAND, id, 0);
		break;
	case IDC_ADVANCED_CONTROLS:
		//Toggle checkbox
		if (GetMenuState(GetMenu(handle), IDC_ADVANCED_CONTROLS, MF_BYCOMMAND) == MF_CHECKED)
			CheckMenuItem(GetMenu(handle), IDC_ADVANCED_CONTROLS, MF_UNCHECKED);
		else
			CheckMenuItem(GetMenu(handle), IDC_ADVANCED_CONTROLS, MF_CHECKED);

		::PostMessage(formHandle, WM_COMMAND, id, 0);
		break;
	default:
		::PostMessage(formHandle, WM_COMMAND, id, 0);
		break;
    }

    return 0;
}



int ControllerMain::close()
{
    Win::log(""); // blank line
    Win::log("Terminating application...");

	::SendMessage(formHandle, WM_COMMAND, IDC_CLOSE_READY, 0);      // close child windows
    ::EnumChildWindows(handle, (WNDENUMPROC)enumerateChildren, (LPARAM)WM_CLOSE);

	//JOE: Wait for all children to close before destroying itself while(...)
    ::DestroyWindow(handle);    // close itself
    return 0;
}



int ControllerMain::destroy()
{
    ::PostQuitMessage(0);       // exit the message loop
 
    Win::log("Main window is destroyed.");
    return 0;
}



int ControllerMain::create()
{
	CheckMenuItem(GetMenu(handle), IDC_SYNC_DISPLAY, MF_CHECKED);
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_SIZE
// the width and height are for the client area
///////////////////////////////////////////////////////////////////////////////
int ControllerMain::size(int width, int height, WPARAM wParam)
{
    RECT rect;

    // get height of status bar
    HWND statusHandle = ::GetDlgItem(handle, IDC_STATUSBAR);
    ::GetWindowRect(statusHandle, &rect);
    int statusHeight = rect.bottom - rect.top;

    // get height of glDialog
    ::GetWindowRect(formHandle, &rect);
    int formHeight = rect.bottom - rect.top;

    // resize the height of glWin and reposition glDialog & status bar
    int glHeight = height - formHeight - statusHeight;
    ::SetWindowPos(glHandle, 0, 0, 0, width, glHeight, SWP_NOZORDER);
    ::SetWindowPos(formHandle, 0, 0, glHeight, width, formHeight, SWP_NOZORDER);
    ::InvalidateRect(formHandle, 0, TRUE);      // force to repaint
    ::SendMessage(statusHandle, WM_SIZE, 0, 0); // automatically resize width, so send 0s
    ::InvalidateRect(statusHandle, 0, FALSE);   // force to repaint

    // display OpenGL window dimension on the status bar
    std::wstringstream wss;
	wss << "Window Size (Client Area): " << width << " x " << height;
    ::SendMessage(statusHandle, SB_SETTEXT, 0, (LPARAM)wss.str().c_str());

    return 0;
}


int ControllerMain::keyDown(int key, LPARAM lParam)
{
	return 0;
}
int ControllerMain::keyUp(int key, LPARAM lParam)
{
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// enumerate all child windows
///////////////////////////////////////////////////////////////////////////////
bool CALLBACK enumerateChildren(HWND handle, LPARAM lParam)
{
    if(lParam == WM_CLOSE)
	{
		::SendMessage(handle, WM_CLOSE, 0, 0);      // close child windows
    }

    return true;
}

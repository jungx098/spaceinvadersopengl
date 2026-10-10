#ifndef _COMMCTRL_H_SHIM_
#define _COMMCTRL_H_SHIM_
#include "windows.h"

typedef struct tagINITCOMMONCONTROLSEX {
	DWORD dwSize;
	DWORD dwICC;
} INITCOMMONCONTROLSEX;

#define ICC_BAR_CLASSES 0x00000004
BOOL InitCommonControlsEx(const INITCOMMONCONTROLSEX*);
BOOL InitCommonControls(void);
BOOL InitCommonControlEx(const INITCOMMONCONTROLSEX*);

#define TOOLTIPS_CLASS "tooltips_class32"
#define TTF_IDISHWND 0x0001
#define TTF_TRACK 0x0020
#define TTF_ABSOLUTE 0x0080
#define TTF_SUBCLASS 0x0010
#define TTM_ADDTOOL (WM_USER + 50)
#define TTM_UPDATETIPTEXT (WM_USER + 12)
#define TTM_TRACKACTIVATE (WM_USER + 17)
#define TTM_TRACKPOSITION (WM_USER + 18)
#define TTM_SETMAXTIPWIDTH (WM_USER + 24)
#define TTM_SETTITLE (WM_USER + 32)
#define TTS_ALWAYSTIP 0x01
#define TTS_NOPREFIX 0x02
#define TTM_SETMARGIN (WM_USER + 26)
#define TTM_SETTIPTEXTCOLOR (WM_USER + 20)
#define TTM_SETTIPBKCOLOR (WM_USER + 19)
#define TBM_GETPOS (WM_USER)
#define TBM_SETRANGEMIN (WM_USER + 7)
#define TBM_SETRANGEMAX (WM_USER + 8)
#define TBM_SETPOS (WM_USER + 5)

typedef struct tagTOOLINFO {
	UINT cbSize;
	UINT uFlags;
	HWND hwnd;
	UINT_PTR uId;
	RECT rect;
	HINSTANCE hinst;
	LPSTR lpszText;
	LPARAM lParam;
} TOOLINFO;
typedef TOOLINFO* LPTOOLINFO;

#endif

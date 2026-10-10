/* macOS compile shim for the Win32 game sources.
   Declarations only. Linking is checklist item 3.
   This program is free software under GPL-2.0-or-later. */

#ifndef _WINDOWS_
#define _WINDOWS_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int UINT;
typedef unsigned int DWORD;
typedef unsigned int ULONG;
typedef intptr_t LONG;
typedef int BOOL;
typedef int INT;
typedef short SHORT;
typedef unsigned short USHORT;
typedef void VOID;
typedef float FLOAT;
typedef int INT32;
typedef long long INT64;
typedef unsigned int UINT32;
typedef intptr_t LONG_PTR;
typedef uintptr_t UINT_PTR;
typedef uintptr_t ULONG_PTR;
typedef intptr_t INT_PTR;
typedef long HRESULT;
typedef DWORD COLORREF;
typedef long long __int64;
typedef void* HANDLE;
typedef void* HGDIOBJ;
typedef wchar_t WCHAR;
typedef WCHAR OLECHAR;
typedef char CHAR;
typedef char TCHAR;
typedef CHAR* LPSTR;
typedef const CHAR* LPCSTR;
typedef TCHAR* LPTSTR;
typedef const TCHAR* LPCTSTR;
typedef WCHAR* LPWSTR;
typedef const WCHAR* LPCWSTR;
typedef OLECHAR* LPOLESTR;
typedef BYTE* LPBYTE;
typedef void* LPVOID;
typedef const void* LPCVOID;
typedef LONG_PTR LPARAM;
typedef UINT_PTR WPARAM;
typedef LONG_PTR LRESULT;

#define DECLARE_HANDLE(name) typedef HANDLE name
DECLARE_HANDLE(HWND);
DECLARE_HANDLE(HDC);
DECLARE_HANDLE(HGLRC);
DECLARE_HANDLE(HINSTANCE);
DECLARE_HANDLE(HMODULE);
DECLARE_HANDLE(HBITMAP);
DECLARE_HANDLE(HBRUSH);
DECLARE_HANDLE(HFONT);
DECLARE_HANDLE(HCURSOR);
DECLARE_HANDLE(HICON);
DECLARE_HANDLE(HMENU);
DECLARE_HANDLE(HKEY);
DECLARE_HANDLE(HRSRC);
DECLARE_HANDLE(HGLOBAL);
DECLARE_HANDLE(HFILE);
DECLARE_HANDLE(HINTERNET);
DECLARE_HANDLE(HLOCAL);

typedef int (*PROC)(void);
typedef PROC FARPROC;
typedef LRESULT (*WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef LRESULT (*DLGPROC)(HWND, UINT, WPARAM, LPARAM);
typedef WORD ATOM;

#define WINAPI
#define CALLBACK
#define APIENTRY
#define __stdcall
#define __cdecl
#define _cdecl

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#define NULL 0
#endif

#define MAX_PATH 260
#define _MAX_DRIVE 3
#define _MAX_DIR 256
#define _MAX_FNAME 256
#define _MAX_EXT 256
#define TEXT(lit) lit
#define _T(lit) lit

#define ZeroMemory(dest, len) memset((dest), 0, (len))
#define MAKEINTRESOURCE(i) ((LPSTR)(ULONG_PTR)((ULONG_PTR)(i)))
#define RT_BITMAP MAKEINTRESOURCE(2)

#define LOWORD(l) ((WORD)((DWORD)(l) & 0xffff))
#define HIWORD(l) ((WORD)(((DWORD)(l) >> 16) & 0xffff))
#define MAKELONG(a, b) ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#define MAKELPARAM(l, h) ((LPARAM)(DWORD)MAKELONG(l, h))
#define MAKELANGID(p, s) ((((WORD)(s)) << 10) | (WORD)(p))
#define RGB(r, g, b) ((COLORREF)(((BYTE)(r) | ((WORD)((BYTE)(g)) << 8)) | (((DWORD)(BYTE)(b)) << 16)))

#define HWND_DESKTOP ((HWND)0)
#define INVALID_HANDLE_VALUE ((HANDLE)(intptr_t)-1)

#define MB_OK 0x00000000
#define MB_ICONEXCLAMATION 0x00000030
#define MB_TOPMOST 0x00040000
#define MB_ICONINFORMATION 0x00000040

#define WM_QUIT 0x0012
#define WM_KEYDOWN 0x0100
#define WM_KEYUP 0x0101
#define WM_CHAR 0x0102
#define WM_MOUSEMOVE 0x0200
#define WM_LBUTTONDOWN 0x0201
#define WM_LBUTTONUP 0x0202
#define WM_RBUTTONDOWN 0x0204
#define WM_RBUTTONUP 0x0205
#define WM_CREATE 0x0001
#define WM_DESTROY 0x0002
#define WM_CLOSE 0x0010
#define WM_SIZE 0x0005
#define WM_PAINT 0x000F
#define WM_COMMAND 0x0111
#define WM_SYSCOMMAND 0x0112
#define WM_ACTIVATE 0x0006
#define WM_SETFOCUS 0x0007
#define WM_KILLFOCUS 0x0008
#define WM_TIMER 0x0113
#define WM_ENTERIDLE 0x0121
#define WM_GETFONT 0x0031
#define WM_SETFONT 0x0030
#define WM_CTLCOLORSTATIC 0x0138
#define WM_LBUTTONDBLCLK 0x0203
#define WM_MOUSEWHEEL 0x020A
#define WM_SYSKEYDOWN 0x0104
#define WM_SYSKEYUP 0x0105
#define WM_MOVE 0x0003
#define WM_SETCURSOR 0x0020
#define WM_ERASEBKGND 0x0014
#define WM_SHOWWINDOW 0x0018
#define WM_INITDIALOG 0x0110
#define WM_NOTIFY 0x004E
#define WM_HSCROLL 0x0114
#define WM_VSCROLL 0x0115
#define WM_USER 0x0400
#define WM_TOGGLEFULLSCREEN 0x8001
#define WM_SETTEXT 0x000C
#define WM_DRAWITEM 0x002B
#define WM_NCLBUTTONDOWN 0x00A1
#define WM_MOUSEHOVER 0x02A1
#define WM_MOUSELEAVE 0x02A3
#define HTCAPTION 2
#define GCL_HICON (-14)
#define EM_SETSEL 0x00B1
#define CBN_SELCHANGE 1
#define CW_USEDEFAULT ((int)0x80000000)
#define ODS_SELECTED 0x0001
#define TME_HOVER 0x00000001
#define TME_LEAVE 0x00000002

#define WS_OVERLAPPEDWINDOW 0x00CF0000
#define WS_POPUP 0x80000000
#define WS_VISIBLE 0x10000000
#define WS_CHILD 0x40000000
#define WS_EX_APPWINDOW 0x00040000
#define WS_EX_TOPMOST 0x00000008
#define WS_EX_LAYERED 0x00080000
#define WS_EX_TRANSPARENT 0x00000020

#define CS_HREDRAW 0x0002
#define CS_VREDRAW 0x0001
#define CS_OWNDC 0x0020

#define SW_NORMAL 1
#define SW_SHOWNORMAL 1
#define SW_SHOW 5
#define SW_HIDE 0
#define LR_DEFAULTCOLOR 0x00000000
#define BM_SETIMAGE 0x00F7
#define WS_VSCROLL 0x00200000
#define WS_EX_CLIENTEDGE 0x00000200
#define GWL_HINSTANCE (-6)
#define MB_ICONERROR 0x00000010
#define ES_LEFT 0x0000
#define ES_MULTILINE 0x0004
#define ES_AUTOVSCROLL 0x0040
#define ES_AUTOHSCROLL 0x0080
#define ES_WANTRETURN 0x1000
#define PM_REMOVE 0x0001

#define GWL_STYLE (-16)
#define GWL_EXSTYLE (-20)
#define GWL_WNDPROC (-4)
#define GWL_USERDATA (-21)
#define GWLP_WNDPROC (-4)

#define IDOK 1
#define IDCANCEL 2
#define MB_APPLMODAL 0x00000000
#define INTERNET_OPEN_TYPE_DIRECT 1
#define ERROR_INTERNET_EXTENDED_ERROR 12003
#define ERROR_NO_MORE_FILES 18
#define SC_SCREENSAVE 0xF140
#define SC_MONITORPOWER 0xF170
#define SC_RESTORE 0xF120
#define WM_ENTERSIZEMOVE 0x0231
#define WM_EXITSIZEMOVE 0x0232
#define SIZE_MINIMIZED 1
#define SIZE_MAXIMIZED 2
#define SIZE_RESTORED 0
#define PFD_GENERIC_FORMAT 0x00000040
#define FORMAT_MESSAGE_ALLOCATE_BUFFER 0x00000100
#define FORMAT_MESSAGE_FROM_SYSTEM 0x00001000
#define FORMAT_MESSAGE_IGNORE_INSERTS 0x00000200
#define LANG_NEUTRAL 0x00
#define SUBLANG_DEFAULT 0x01

#define VK_ESCAPE 0x1B
#define VK_LEFT 0x25
#define VK_UP 0x26
#define VK_RIGHT 0x27
#define VK_SPACE 0x20
#define VK_RETURN 0x0D
#define VK_BACK 0x08
#define VK_TAB 0x09
#define VK_CONTROL 0x11
#define VK_LWIN 0x5B
#define VK_RWIN 0x5C
#define VK_ADD 0x6B
#define VK_SUBTRACT 0x6D
#define VK_F1 0x70
#define VK_F2 0x71
#define VK_F3 0x72
#define VK_F4 0x73
#define VK_F5 0x74
#define VK_F6 0x75
#define VK_F10 0x79
#define VK_F12 0x7B

typedef struct tagCREATESTRUCT {
	LPVOID lpCreateParams;
	HINSTANCE hInstance;
	HMENU hMenu;
	HWND hwndParent;
	int cy;
	int cx;
	int y;
	int x;
	LONG style;
	LPCSTR lpszName;
	LPCSTR lpszClass;
	DWORD dwExStyle;
} CREATESTRUCT;

#define SS_NOTIFY 0x0100
#define BLACK_BRUSH 4
#define WHITE_BRUSH 0

#define PFD_DRAW_TO_WINDOW 0x00000004
#define PFD_SUPPORT_OPENGL 0x00000020
#define PFD_DOUBLEBUFFER 0x00000001
#define PFD_TYPE_RGBA 0
#define PFD_MAIN_PLANE 0

#define DM_BITSPERPEL 0x00040000
#define DM_PELSWIDTH 0x00080000
#define DM_PELSHEIGHT 0x00100000
#define CDS_FULLSCREEN 4
#define DISP_CHANGE_SUCCESSFUL 0

#define SM_CXSCREEN 0
#define SM_CYSCREEN 1
#define HORZRES 8
#define VERTRES 10
#define PLANES 14
#define LOGPIXELSX 88
#define LOGPIXELSY 90
#define BITSPIXEL 12

#define GENERIC_READ 0x80000000u
#define GENERIC_WRITE 0x40000000u
#define FILE_SHARE_READ 0x00000001
#define CREATE_ALWAYS 2
#define OPEN_EXISTING 3
#define FILE_ATTRIBUTE_NORMAL 0x00000080

#define GMEM_MOVEABLE 0x0002
#define CP_ACP 0

#define ERROR_SUCCESS 0
#define HKEY_LOCAL_MACHINE ((HKEY)(uintptr_t)0x80000002)
#define KEY_READ 0x20019
#define KEY_WRITE 0x20006
#define KEY_ALL_ACCESS 0xF003F
#define REG_OPTION_NON_VOLATILE 0
#define REG_DWORD 4
#define REG_BINARY 3
#define REG_SZ 1

#define DLL_PROCESS_ATTACH 1
#define DLL_PROCESS_DETACH 0

#define IMAGE_BITMAP 0
#define LR_CREATEDIBSECTION 0x00002000
#define DIB_RGB_COLORS 0
#define BI_RGB 0

#define FW_NORMAL 400
#define ANSI_CHARSET 0
#define OUT_DEFAULT_PRECIS 0
#define CLIP_DEFAULT_PRECIS 0
#define ANTIALIASED_QUALITY 4
#define DEFAULT_CHARSET 1

#define S_OK 0
#define FAILED(hr) ((HRESULT)(hr) < 0)
#define SUCCEEDED(hr) ((HRESULT)(hr) >= 0)

#define INTERNET_OPEN_TYPE_PRECONFIG 0

#define THREAD_PRIORITY_LOWEST THREAD_BASE_PRIORITY_MIN
#define THREAD_BASE_PRIORITY_MIN (-2)
#define THREAD_PRIORITY_NORMAL 0
#define THREAD_PRIORITY_HIGHEST 2

#define IDC_ARROW MAKEINTRESOURCE(32512)
#define IDI_APPLICATION MAKEINTRESOURCE(32512)

#define BN_CLICKED 0
#define BST_CHECKED 1
#define BST_UNCHECKED 0
#define CB_ADDSTRING 0x0143
#define CB_DELETESTRING 0x0144
#define CB_SETCURSEL 0x014E
#define CB_GETCURSEL 0x0147
#define BM_SETCHECK 0x00F1
#define BM_GETCHECK 0x00F0
#define EM_SETLIMITTEXT 0x00C5
#define SWP_NOSIZE 0x0001
#define SWP_NOMOVE 0x0002
#define SWP_NOZORDER 0x0004
#define HWND_TOP ((HWND)0)
#define HWND_TOPMOST ((HWND)(intptr_t)-1)

#define LWA_ALPHA 0x00000002
#define LWA_COLORKEY 0x00000001

typedef struct tagPOINT {
	LONG x;
	LONG y;
} POINT;

typedef struct tagRECT {
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
} RECT;
typedef RECT* LPRECT;
typedef const RECT* LPCRECT;

typedef struct tagTRACKMOUSEEVENT {
	DWORD cbSize;
	DWORD dwFlags;
	HWND hwndTrack;
	DWORD dwHoverTime;
} TRACKMOUSEEVENT;

typedef struct tagDRAWITEMSTRUCT {
	UINT CtlType;
	UINT CtlID;
	UINT itemID;
	UINT itemAction;
	UINT itemState;
	HWND hwndItem;
	HDC hDC;
	RECT rcItem;
	ULONG_PTR itemData;
} DRAWITEMSTRUCT;
typedef DRAWITEMSTRUCT* LPDRAWITEMSTRUCT;

typedef struct tagMSG {
	HWND hwnd;
	UINT message;
	WPARAM wParam;
	LPARAM lParam;
	DWORD time;
	POINT pt;
} MSG;
typedef MSG* LPMSG;

typedef struct tagWNDCLASSEX {
	UINT cbSize;
	UINT style;
	WNDPROC lpfnWndProc;
	int cbClsExtra;
	int cbWndExtra;
	HINSTANCE hInstance;
	HICON hIcon;
	HCURSOR hCursor;
	HBRUSH hbrBackground;
	LPCSTR lpszMenuName;
	LPCSTR lpszClassName;
	HICON hIconSm;
} WNDCLASSEX;

typedef struct tagPIXELFORMATDESCRIPTOR {
	WORD nSize;
	WORD nVersion;
	DWORD dwFlags;
	BYTE iPixelType;
	BYTE cColorBits;
	BYTE cRedBits;
	BYTE cRedShift;
	BYTE cGreenBits;
	BYTE cGreenShift;
	BYTE cBlueBits;
	BYTE cBlueShift;
	BYTE cAlphaBits;
	BYTE cAlphaShift;
	BYTE cAccumBits;
	BYTE cAccumRedBits;
	BYTE cAccumGreenBits;
	BYTE cAccumBlueBits;
	BYTE cAccumAlphaBits;
	BYTE cDepthBits;
	BYTE cStencilBits;
	BYTE cAuxBuffers;
	BYTE iLayerType;
	BYTE bReserved;
	DWORD dwLayerMask;
	DWORD dwVisibleMask;
	DWORD dwDamageMask;
} PIXELFORMATDESCRIPTOR;

typedef struct tagDEVMODE {
	WORD dmSize;
	DWORD dmFields;
	DWORD dmBitsPerPel;
	DWORD dmPelsWidth;
	DWORD dmPelsHeight;
} DEVMODE;

typedef struct tagBITMAPINFOHEADER {
	DWORD biSize;
	LONG biWidth;
	LONG biHeight;
	WORD biPlanes;
	WORD biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG biXPelsPerMeter;
	LONG biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagBITMAPINFO {
	BITMAPINFOHEADER bmiHeader;
} BITMAPINFO;

typedef struct tagBITMAP {
	LONG bmType;
	LONG bmWidth;
	LONG bmHeight;
	LONG bmWidthBytes;
	WORD bmPlanes;
	WORD bmBitsPixel;
	LPVOID bmBits;
} BITMAP;

typedef struct tagLOGFONT {
	LONG lfHeight;
	LONG lfWidth;
	LONG lfEscapement;
	LONG lfOrientation;
	LONG lfWeight;
	BYTE lfItalic;
	BYTE lfUnderline;
	BYTE lfStrikeOut;
	BYTE lfCharSet;
	BYTE lfOutPrecision;
	BYTE lfClipPrecision;
	BYTE lfQuality;
	BYTE lfPitchAndFamily;
	char lfFaceName[32];
} LOGFONT;

typedef struct {
	long long QuadPart;
} LARGE_INTEGER;

typedef struct {
	DWORD dwFileAttributes;
	char cFileName[MAX_PATH];
} WIN32_FIND_DATA;

typedef struct tagGUID {
	unsigned int Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char Data4[8];
} GUID;

int MessageBox(HWND, LPCSTR, LPCSTR, UINT);
void OutputDebugString(LPCSTR);
void Sleep(DWORD);
DWORD GetTickCount(void);
DWORD timeGetTime(void);
BOOL QueryPerformanceFrequency(LARGE_INTEGER*);
BOOL QueryPerformanceCounter(LARGE_INTEGER*);

HMODULE GetModuleHandle(LPCSTR);
FARPROC GetProcAddress(HMODULE, LPCSTR);
HMODULE LoadLibrary(LPCSTR);
BOOL FreeLibrary(HMODULE);

BOOL ShowWindow(HWND, int);
BOOL UpdateWindow(HWND);
BOOL DestroyWindow(HWND);
HWND CreateWindowEx(DWORD, LPCSTR, LPCSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID);
#define CreateWindow(cls, title, style, x, y, w, h, parent, menu, inst, param) \
	CreateWindowEx(0, cls, title, style, x, y, w, h, parent, menu, inst, param)
ATOM RegisterClassEx(const WNDCLASSEX*);
BOOL UnregisterClass(LPCSTR, HINSTANCE);
LRESULT DefWindowProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CallWindowProc(WNDPROC, HWND, UINT, WPARAM, LPARAM);
BOOL PeekMessage(LPMSG, HWND, UINT, UINT, UINT);
LRESULT DispatchMessage(const MSG*);
BOOL TranslateMessage(const MSG*);
BOOL WaitMessage(void);
void PostQuitMessage(int);
BOOL PostMessage(HWND, UINT, WPARAM, LPARAM);
LRESULT SendMessage(HWND, UINT, WPARAM, LPARAM);
BOOL GetMessage(LPMSG, HWND, UINT, UINT);

HDC GetDC(HWND);
int ReleaseDC(HWND, HDC);
BOOL AdjustWindowRectEx(RECT*, DWORD, BOOL, DWORD);
HWND GetDesktopWindow(void);
int GetDeviceCaps(HDC, int);
int GetSystemMetrics(int);
BOOL ShowCursor(BOOL);
HCURSOR LoadCursor(HINSTANCE, LPCSTR);
HCURSOR SetCursor(HCURSOR);
HICON LoadIcon(HINSTANCE, LPCSTR);
HANDLE LoadImage(HINSTANCE, LPCSTR, UINT, int, int, UINT);
BOOL GetCursorPos(POINT*);
BOOL ClientToScreen(HWND, POINT*);
BOOL ScreenToClient(HWND, POINT*);
HWND SetCapture(HWND);
BOOL ReleaseCapture(void);
HWND GetCapture(void);
BOOL GetWindowRect(HWND, RECT*);
BOOL GetClientRect(HWND, RECT*);
BOOL InvalidateRect(HWND, const RECT*, BOOL);
BOOL SetWindowPos(HWND, HWND, int, int, int, int, UINT);
BOOL SetWindowText(HWND, LPCSTR);
int GetWindowText(HWND, LPSTR, int);
LONG GetWindowLong(HWND, int);
LONG SetWindowLong(HWND, int, LONG);
ULONG_PTR SetClassLong(HWND, int, LONG);
BOOL EnableWindow(HWND, BOOL);
HWND GetParent(HWND);
HWND ChildWindowFromPoint(HWND, POINT);
BOOL PtInRect(const RECT*, POINT);
BOOL SetForegroundWindow(HWND);

HDC CreateCompatibleDC(HDC);
BOOL DeleteDC(HDC);
HBITMAP CreateDIBSection(HDC, const BITMAPINFO*, UINT, void**, HANDLE, DWORD);
HGDIOBJ SelectObject(HDC, HGDIOBJ);
BOOL DeleteObject(HGDIOBJ);
int GetObject(HANDLE, int, LPVOID);
void* GetStockObject(int);
int FillRect(HDC, const RECT*, HBRUSH);
int FrameRect(HDC, const RECT*, HBRUSH);
HBRUSH CreateSolidBrush(COLORREF);
COLORREF SetBkColor(HDC, COLORREF);
COLORREF SetTextColor(HDC, COLORREF);
BOOL TextOut(HDC, int, int, LPCSTR, int);
HFONT CreateFont(int, int, int, int, int, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, LPCSTR);
HFONT CreateFontIndirect(const LOGFONT*);
int MulDiv(int, int, int);

int ChoosePixelFormat(HDC, const PIXELFORMATDESCRIPTOR*);
BOOL SetPixelFormat(HDC, int, const PIXELFORMATDESCRIPTOR*);
int DescribePixelFormat(HDC, int, UINT, PIXELFORMATDESCRIPTOR*);
BOOL SwapBuffers(HDC);
HGLRC wglCreateContext(HDC);
BOOL wglDeleteContext(HGLRC);
BOOL wglMakeCurrent(HDC, HGLRC);
HGLRC wglGetCurrentContext(void);
HDC wglGetCurrentDC(void);
PROC wglGetProcAddress(LPCSTR);

LONG ChangeDisplaySettings(DEVMODE*, DWORD);

DWORD GetCurrentDirectory(DWORD, LPSTR);
UINT GetSystemDirectory(LPSTR, UINT);
DWORD GetTempPath(DWORD, LPSTR);
BOOL CreateDirectory(LPCSTR, void*);
BOOL RemoveDirectory(LPCSTR);
BOOL DeleteFile(LPCSTR);
HANDLE CreateFile(LPCSTR, DWORD, DWORD, void*, DWORD, DWORD, HANDLE);
BOOL WriteFile(HANDLE, LPCVOID, DWORD, DWORD*, void*);
BOOL ReadFile(HANDLE, LPVOID, DWORD, DWORD*, void*);
BOOL CloseHandle(HANDLE);
HANDLE FindFirstFile(LPCSTR, WIN32_FIND_DATA*);
BOOL FindNextFile(HANDLE, WIN32_FIND_DATA*);
BOOL FindClose(HANDLE);

HRSRC FindResource(HMODULE, LPCSTR, LPCSTR);
HGLOBAL LoadResource(HMODULE, HRSRC);
LPVOID LockResource(HGLOBAL);
BOOL UnlockResource(HGLOBAL);
DWORD SizeofResource(HMODULE, HRSRC);
BOOL FreeResource(HGLOBAL);
HGLOBAL GlobalAlloc(UINT, DWORD);
LPVOID GlobalLock(HGLOBAL);
BOOL GlobalUnlock(HGLOBAL);
HLOCAL LocalFree(void*);

DWORD GetLastError(void);
DWORD FormatMessage(DWORD, LPCVOID, DWORD, DWORD, LPSTR, DWORD, va_list*);

int MultiByteToWideChar(UINT, DWORD, LPCSTR, int, LPWSTR, int);

LONG RegOpenKeyEx(HKEY, LPCSTR, DWORD, DWORD, HKEY*);
LONG RegCreateKeyEx(HKEY, LPCSTR, DWORD, LPSTR, DWORD, DWORD, void*, HKEY*, DWORD*);
LONG RegQueryValueEx(HKEY, LPCSTR, DWORD*, DWORD*, LPBYTE, DWORD*);
LONG RegSetValueEx(HKEY, LPCSTR, DWORD, DWORD, const BYTE*, DWORD);
LONG RegCloseKey(HKEY);

HANDLE SetProp(HWND, LPCSTR, HANDLE);
HANDLE GetProp(HWND, LPCSTR);
HANDLE RemoveProp(HWND, LPCSTR);

BOOL SetThreadPriority(HANDLE, int);
HANDLE GetCurrentThread(void);

BOOL _TrackMouseEvent(TRACKMOUSEEVENT*);
HINSTANCE ShellExecute(HWND, LPCSTR, LPCSTR, LPCSTR, LPCSTR, int);
intptr_t DialogBox(HINSTANCE, LPCSTR, HWND, WNDPROC);
intptr_t DialogBoxParam(HINSTANCE, LPCSTR, HWND, WNDPROC, LPARAM);
BOOL EndDialog(HWND, intptr_t);
HWND GetDlgItem(HWND, int);
BOOL SetDlgItemText(HWND, int, LPCSTR);
UINT GetDlgItemText(HWND, int, LPSTR, int);
BOOL CheckDlgButton(HWND, int, UINT);
UINT IsDlgButtonChecked(HWND, int);
BOOL CheckRadioButton(HWND, int, int, int);
LRESULT SendDlgItemMessage(HWND, int, UINT, WPARAM, LPARAM);

HINTERNET InternetOpen(LPCSTR, DWORD, LPCSTR, LPCSTR, DWORD);
HINTERNET InternetOpenUrl(HINTERNET, LPCSTR, LPCSTR, DWORD, DWORD, DWORD);
BOOL InternetQueryDataAvailable(HINTERNET, DWORD*, DWORD, DWORD);
BOOL InternetReadFile(HINTERNET, LPVOID, DWORD, DWORD*);
BOOL InternetCloseHandle(HINTERNET);
BOOL HttpSendRequest(HINTERNET, LPCSTR, DWORD, LPVOID, DWORD);
DWORD InternetAttemptConnect(DWORD);
BOOL InternetGetLastResponseInfo(DWORD*, LPSTR, DWORD*);

void PathRemoveExtension(char*);
BOOL PathAddExtension(char*, const char*);

BOOL FMOD_DllMain(HINSTANCE, DWORD, LPVOID);
int wsprintf(LPSTR, LPCSTR, ...);

char* _itoa(int, char*, int);
char* _ltoa(long, char*, int);
char* _strdup(const char*);
char* _strlwr(char*);
char* _strupr(char*);
void _splitpath(const char* path, char* drive, char* dir, char* fname, char* ext);
int _stricmp(const char*, const char*);
double _hypot(double, double);
int _vsnprintf(char*, size_t, const char*, va_list);
int _vsntprintf(char*, size_t, const char*, va_list);

#ifdef __cplusplus
}
#endif

#endif

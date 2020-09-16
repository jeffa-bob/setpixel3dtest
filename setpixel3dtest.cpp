// setpixel3dtest.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include <io.h>
#include <fcntl.h>
#include <iostream>
#include "wingdi.h"
#include "setpixel3dtest.h"
#include "3dworld.hpp"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
HWND CURWIN;
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPWSTR    lpCmdLine,
  _In_ int       nCmdShow)
{
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  // TODO: Place code here.

  // Initialize global strings
  LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_SETPIXEL3DTEST, szWindowClass, MAX_LOADSTRING);
  MyRegisterClass(hInstance);

  // Perform application initialization:
  if (!InitInstance(hInstance, nCmdShow))
  {
    return FALSE;
  }

  HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SETPIXEL3DTEST));

  MSG msg;


  world::currentworld newworld;
  SetWindowPos(CURWIN, HWND_TOP, 200, 200, newworld.cam.height, newworld.cam.width, SWP_NOMOVE);

  world::tri triangle = { { {-1, 16, -5}, { 1,12,5 }, {1,15,0 }}, { 0,255,0 } };
  triangle.normal = world::trinormal(triangle).raypoint[1];
  newworld.triworld.push_back(triangle);

  triangle = { { {-2, 19, -5}, { 6,12,5 }, { 5,15,0 }}, { 255,0,0 } };
  triangle.normal = world::trinormal(triangle).raypoint[1];
  newworld.triworld.push_back(triangle);

  newworld.window = GetDC(CURWIN);

  float curdirect = 0;

  newworld.renderscreen();

  /*
  AllocConsole();
  HANDLE stdHandle;
  int hConsole;
  FILE* fp;
  stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
  hConsole = _open_osfhandle((long)stdHandle, _O_TEXT);
  fp = _fdopen(hConsole, "w");
  freopen_s(&fp, "CONOUT$", "w", stdout);
  */

  // Main message loop:
  while (GetMessage(&msg, nullptr, 0, 0))
  {
    if (msg.message == WM_KEYDOWN) {
      switch (msg.wParam)
      {
      case VK_LEFT:
        //newworld.cam.pos.x -= 1;
        //newworld.cam.camdir.raypoint[1].x -= 1;
        curdirect += 0.785;
        newworld.setcamangle(curdirect);
        newworld.renderscreen();
        std::cout << newworld.cam.pos.x << " " << newworld.cam.pos.y << " " << newworld.cam.pos.z << "   " << newworld.cam.camdir.raypoint[1].x << " " << newworld.cam.camdir.raypoint[1].y << " " << newworld.cam.camdir.raypoint[1].z <<std::endl;
        break;

      case VK_RIGHT:
        //newworld.cam.pos.x += 1;
        //newworld.cam.camdir.raypoint[1].x += 1;
        curdirect -= 0.785;
        newworld.setcamangle(curdirect);
        newworld.renderscreen();
        std::cout << newworld.cam.pos.x << " " << newworld.cam.pos.y << " " << newworld.cam.pos.z << "   " << newworld.cam.camdir.raypoint[1].x << " " << newworld.cam.camdir.raypoint[1].y << " " << newworld.cam.camdir.raypoint[1].z << std::endl;
        break;

      case VK_UP:
        newworld.cam.pos.y += 1;
        newworld.cam.camdir.raypoint[1].y += 1;
        newworld.renderscreen();
        std::cout << newworld.cam.pos.x << " " << newworld.cam.pos.y << " " << newworld.cam.pos.z << "   " << newworld.cam.camdir.raypoint[1].x << " " << newworld.cam.camdir.raypoint[1].y << " " << newworld.cam.camdir.raypoint[1].z << std::endl;
        break;

      case VK_DOWN:
        newworld.cam.pos.y -= 1;
        newworld.cam.camdir.raypoint[1].y -= 1;
        newworld.renderscreen();
        std::cout << newworld.cam.pos.x << " " << newworld.cam.pos.y << " " << newworld.cam.pos.z << "   " << newworld.cam.camdir.raypoint[1].x << " " << newworld.cam.camdir.raypoint[1].y << " " << newworld.cam.camdir.raypoint[1].z << std::endl;
        break;

      default:
        break;
      }
    }
    else
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
  WNDCLASSEXW wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SETPIXEL3DTEST));
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SETPIXEL3DTEST);
  wcex.lpszClassName = szWindowClass;
  wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

  return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
  hInst = hInstance; // Store instance handle in our global variable

  HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

  if (!hWnd)
  {
    return FALSE;
  }

  CURWIN = hWnd;
  ShowWindow(CURWIN, nCmdShow);
  UpdateWindow(CURWIN);

  return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
  case WM_COMMAND:
  {
    int wmId = LOWORD(wParam);
    // Parse the menu selections:
    switch (wmId)
    {
    case IDM_ABOUT:
      DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
      break;
    case IDM_EXIT:
      DestroyWindow(hWnd);
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
    }
  }
  break;
  case WM_PAINT:
  {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    // TODO: Add any drawing code that uses hdc here...
    EndPaint(hWnd, &ps);
  }
  break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  UNREFERENCED_PARAMETER(lParam);
  switch (message)
  {
  case WM_INITDIALOG:
    return (INT_PTR)TRUE;

  case WM_COMMAND:
    if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
    {
      EndDialog(hDlg, LOWORD(wParam));
      return (INT_PTR)TRUE;
    }
    break;
  }
  return (INT_PTR)FALSE;
}

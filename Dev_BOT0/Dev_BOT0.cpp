// Dev_BOT0.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Dev_BOT0.h"
#include <string>
#include "camera.h"
#define MAX_LOADSTRING 100
#define IDT_TIMER 1 // Define timer ID

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// Forward declaration of UpdateClock function
void UpdateClock(HWND hWnd);

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
    LoadStringW(hInstance, IDC_DEVBOT0, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DEVBOT0));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEVBOT0));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DEVBOT0);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

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
    case WM_CREATE:
        // Start the timer when the window is created
        SetTimer(hWnd, IDT_TIMER, 1000, NULL); // Update every 1 second
        // Start camera capture when the window is created
        StartCameraCapture(hWnd);
        break;
    case WM_TIMER:
        // Handle timer event
        if (wParam == IDT_TIMER) {
            UpdateClock(hWnd);
        }
        break;
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
        // Cleanup and exit
        KillTimer(hWnd, IDT_TIMER); // Stop the timer
        // Cleanup and exit
        StopCameraCapture(); // Stop camera capture
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


// Function to update the window content with the current time
void UpdateClock(HWND hWnd) {
    // Get current time
    SYSTEMTIME currentTime;
    GetLocalTime(&currentTime);

    // Determine greeting based on time
    std::string greeting;
    if (currentTime.wHour >= 5 && currentTime.wHour < 12) {
        greeting = "Good morning";
    }
    else if (currentTime.wHour >= 12 && currentTime.wHour < 18) {
        greeting = "Good afternoon";
    }
    else {
        greeting = "Good evening";
    }

    // Format time string
    TCHAR timeString[50]; // Format: HH:MM:SS Greeting
    _stprintf_s(timeString, _T("%02d:%02d:%02d %hs"), currentTime.wHour, currentTime.wMinute, currentTime.wSecond, greeting.c_str());

    // Update window title with time string
    SetWindowText(hWnd, timeString);
}

// Function to start camera capture
void StartCameraCapture(HWND hWnd) {
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (SUCCEEDED(hr)) {
        hr = CoCreateInstance(CLSID_MediaCapture, nullptr, CLSCTX_ALL, IID_PPV_ARGS(&pMediaCapture));
        if (SUCCEEDED(hr)) {
            hr = pMediaCapture->InitializeAsync(nullptr).get();
            if (SUCCEEDED(hr)) {
                // Set the preview window to our application window
                IMediaCaptureInitializationSettings* pSettings;
                hr = pMediaCapture->get_InitializationSettings(&pSettings);
                if (SUCCEEDED(hr)) {
                    hr = pSettings->put_PreviewWindow(reinterpret_cast<HWND>(hWnd));
                    if (SUCCEEDED(hr)) {
                        hr = pMediaCapture->StartPreviewAsync().get();
                    }
                    pSettings->Release();
                }
            }
        }
    }
    if (FAILED(hr)) {
        // Handle error
    }
}

// Function to stop camera capture
void StopCameraCapture() {
    if (pMediaCapture) {
        pMediaCapture->StopPreviewAsync().get();
        pMediaCapture->Close();
        pMediaCapture->Release();
        pMediaCapture = nullptr;
    }
}
#pragma once

#include <windows.media.capture.h>
#pragma comment(lib, "windowsapp")

// Global Variables:
IMediaCapture *pMediaCapture = nullptr;

// Forward declarations of functions included in this code module:
void StartCameraCapture(HWND hWnd);
void StopCameraCapture();

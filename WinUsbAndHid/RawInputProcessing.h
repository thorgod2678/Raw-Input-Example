#pragma once
#include <Windows.h>

BOOL ProcessRawInput(HRAWINPUT data, RAWINPUT* output);

BOOL RegisterRawInput(RAWINPUTDEVICE rawinput[]);
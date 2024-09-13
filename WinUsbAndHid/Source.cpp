#include<Windows.h>
#include<string>
#include<sstream>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <richedit.h>   // For Rich Edit control
#include <commctrl.h>



#define SL 1
#define EL 2

BOOL ll;
LPCWSTR text;

LPCWSTR UShortToLPCWSTR(USHORT value) {
	static std::wstring wideString;
	wideString = std::to_wstring(value);
	return wideString.c_str();
}


LRESULT CALLBACK Proc(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp) {
	
	if (msg == WM_COMMAND) {
		if (wp == SL) {
			ll = 1;
		}
		if (wp == EL) {
			ll = 0;
		}
	}

	if (msg == WM_PAINT) {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		// All painting occurs here, between BeginPaint and EndPaint.
		HBRUSH brush = CreateSolidBrush(RGB(0, 150, 0));
		FillRect(hdc, &ps.rcPaint,brush);
		DeleteObject(brush);
		EndPaint(hwnd, &ps);
	}
	if (msg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}
	if (msg == WM_INPUT) {
		if (ll) {
			UINT dwSize = 0;
			GetRawInputData((HRAWINPUT)lp, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));

			if (dwSize > 0) {
				RAWINPUT* pRawInput = (RAWINPUT*)malloc(dwSize);
				if (pRawInput) {
					if (GetRawInputData((HRAWINPUT)lp, RID_INPUT, pRawInput, &dwSize, sizeof(RAWINPUTHEADER)) == dwSize) {
						// Process raw input and show information in a dialog box
						HANDLE hDevice = pRawInput->header.hDevice;
						

						UINT size = 0;
						GetRawInputDeviceInfoW(hDevice, RIDI_DEVICENAME, NULL, &size);
						WCHAR* deviceName = new WCHAR[size];
						if (!GetRawInputDeviceInfoW(hDevice, RIDI_DEVICENAME, deviceName, &size)) {
							MessageBoxW(hwnd, L"Can't get Device name", L"Device Data", MB_ICONWARNING);
						}
						

							
						

						RID_DEVICE_INFO deviceInfo;
						deviceInfo.cbSize = sizeof(RID_DEVICE_INFO);
						UINT cbSize = sizeof(RID_DEVICE_INFO);
						if (GetRawInputDeviceInfoW(hDevice, RIDI_DEVICEINFO, &deviceInfo, &cbSize) <= 0) {
							MessageBox(hwnd, L"Unknown Device", L"error", MB_ICONERROR);
						}
						std::wstringstream info2;
						info2 << L"Device Extra Details:"
							L"\n Vendor Id: " << deviceInfo.hid.dwVendorId
							<< L"\n Product Id: " << deviceInfo.hid.dwProductId
							<< L"\n Version Number: " << deviceInfo.hid.dwVersionNumber
							<< L"\n Mouse Sample rate: " << deviceInfo.mouse.dwSampleRate
							<< L"\n Mouse Button count: " << deviceInfo.mouse.dwNumberOfButtons
							<< L"\n Mouse Horizontal Wheel: " << deviceInfo.mouse.fHasHorizontalWheel
							<< L"\n No. of keys: " << deviceInfo.keyboard.dwNumberOfKeysTotal
							<< L"\n Keyboard Mode: " << deviceInfo.keyboard.dwKeyboardMode
							<< L"\n Function key count: " << deviceInfo.keyboard.dwNumberOfFunctionKeys
							<< L"\n Indicator count: " << deviceInfo.keyboard.dwNumberOfIndicators
							<< L"\n Sub Type: " << deviceInfo.keyboard.dwSubType
							<< L"\n Type: " << deviceInfo.keyboard.dwType;


						HWND y = GetDlgItem(hwnd, 4);
						SetWindowTextW(y, info2.str().c_str());

						//pRawInput->header.
						std::wstring fn(friendlyName);
						std::wstringstream info; 
						info << L"Device Details: "
							<< L"\n Device Path: " << deviceName
							<< L"\n HID Data in bytes: " << pRawInput->data.hid.bRawData
							<< L"\n Device Type: " << pRawInput->header.dwType
							
						
							
							<< L"\n\nKeyboard: "
							<< L"\n Windows MSG Key code: " << std::to_wstring(pRawInput->data.keyboard.VKey)
							<< L"\n Scan Code: " << std::to_wstring(pRawInput->data.keyboard.MakeCode)
							<< L"\n Message: " << std::to_wstring(pRawInput->data.keyboard.Message)
							<< L"\n Flags: " << std::to_wstring(pRawInput->data.keyboard.Flags)

							<< L"\n\nMouse: "
							<< L"\n Relative X: " << std::to_wstring(pRawInput->data.mouse.lLastX)
							<< L"\n Relative Y: " << std::to_wstring(pRawInput->data.mouse.lLastY)
							<< L"\n Buttons: " << std::to_wstring(pRawInput->data.mouse.ulButtons)
							<< L"\n Extra Info: " << std::to_wstring(pRawInput->data.mouse.ulExtraInformation)
							<< L"\n Raw Buttons: " << std::to_wstring(pRawInput->data.mouse.ulRawButtons)
							<< L"\n Button Flags: " << std::to_wstring(pRawInput->data.mouse.usButtonFlags)
							<< L"\n Flags: " << std::to_wstring(pRawInput->data.mouse.usFlags);
						     
							
						text = info.str().c_str();
						
						//MessageBox(hwnd, deviceName, L"dd", MB_ICONSTOP);
							
						//MessageBoxW(hwnd, info.str().c_str(), L"Device Data", MB_OK);
						HWND x = GetDlgItem(hwnd, 3);
						SetWindowTextW(x,info.str().c_str());

						//UpdateWindow(hwnd);
					}
					else {
						MessageBoxW(hwnd, L"Can't get input info", L"Error", MB_ICONERROR);
					}
					free(pRawInput);
					// Retrieve device information


					
				}

			}
		}
	}
	

	return DefWindowProc(hwnd, msg, wp, lp);
}
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hpi, LPSTR lpCmdLine, int nCmdShow) {
	LoadLibrary(TEXT("Riched20.dll"));

	LPCWSTR classname = L"Class1";
	WNDCLASS wc = {};
	wc.hInstance = hInst;
	wc.lpfnWndProc = Proc;
	wc.lpszClassName = classname;
	RegisterClass(&wc);

	HWND main = CreateWindowExW(0,classname,L"Raw Input Inspector",WS_OVERLAPPEDWINDOW,0,0,1380,720,NULL,NULL,hInst,NULL);
	ShowWindow(main, nCmdShow);
	
	HWND startll = CreateWindowExW(0, L"Button", L"Start listening", WS_CHILD|BS_PUSHBUTTON, 10, 10, 150, 50, main, (HMENU)1, hInst, NULL);
	ShowWindow(startll, nCmdShow);

	HWND endll = CreateWindowExW(0, L"Button", L"End listening", WS_CHILD | BS_PUSHBUTTON, 10, 90, 150, 50, main, (HMENU)2, hInst, NULL);
	ShowWindow(endll, nCmdShow);

//	HWND text = CreateWindowExW(0, L"STATIC", L"dd", WS_CHILD | SS_EDITCONTROL, 10, 160, 800, 500, main, (HMENU)3, hInst, NULL);
	//ShowWindow(text, nCmdShow);
	LoadLibrary(TEXT("Msftedit.dll"));
	
	HWND RDeviceD = CreateWindowExW(0,MSFTEDIT_CLASS,TEXT("Device Details"),
		ES_MULTILINE|WS_VISIBLE|WS_CHILD|WS_BORDER|WS_TABSTOP|ES_READONLY
	,10,160,700,500,main,HMENU(3),hInst,NULL);
	ShowWindow(RDeviceD, nCmdShow);

	HWND RDEVICEL = CreateWindowExW(0, MSFTEDIT_CLASS, TEXT("Device List"),
		ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP | ES_READONLY
		, 730, 160, 600, 300, main, HMENU(4), hInst, NULL);
	ShowWindow(RDEVICEL, nCmdShow);

	RAWINPUTDEVICE rd[2];
	rd[0].usUsagePage = 0x01;
	rd[0].usUsage = 0x06;
	rd[0].dwFlags = 0;
	rd[0].hwndTarget = main;

	rd[1].usUsagePage = 0x01;
	rd[1].usUsage = 0x02;
	rd[1].dwFlags = 0;
	rd[1].hwndTarget = main;



	if (RegisterRawInputDevices(rd, 2, sizeof(rd[0])) == FALSE)
	{
		DWORD error = GetLastError();
		std::wstring errorMessage = L"Not registered. Reason: " + std::to_wstring(error);
		MessageBoxW(main, errorMessage.c_str(), L"Registration Error", MB_OK | MB_ICONERROR);
	
	}


	MSG message;
	while (GetMessageW(&message, main, 0, 0)>0) {
		TranslateMessage(&message);
		DispatchMessage(&message);

	}

	return 0;
}
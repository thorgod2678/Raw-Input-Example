#include "RawInputProcessing.h"

BOOL ProcessRawInput(HRAWINPUT data,RAWINPUT* output)
{
	UINT dwSize = 0;
	GetRawInputData(data, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));

	if (dwSize > 0) {
		output = (RAWINPUT*)malloc(dwSize);
		if (GetRawInputData(data, RID_INPUT, output, &dwSize, sizeof(RAWINPUTHEADER)) == dwSize) {

			
			return 1;
		}
		return false;
	}
}

BOOL RegisterRawInput(RAWINPUTDEVICE rawinput[])
{


	if (RegisterRawInputDevices(rawinput, sizeof(rawinput), sizeof(rawinput[0])) == FALSE)
	{
		DWORD error = GetLastError();
		return error;
		

	}



	return 1;
}

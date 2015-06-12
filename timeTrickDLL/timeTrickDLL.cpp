#include <Windows.h>
#include <iostream>
#include <signal.h>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

SYSTEMTIME st, save;

void performAction();

__declspec(dllexport) void changeTime(unsigned int timer)
{
	GetSystemTime(&save);

	// Change local time
	GetSystemTime(&st);
	st.wYear = 2015;
	st.wMonth = 02;
	st.wDay = 24;
	SetSystemTime(&st);

	int i = 1;
	while (i < timer)
	{
		performAction();
		i++;
	}

	// Reseting time
	SetSystemTime(&save);
}

void performAction(){
	Sleep(1000);
	if(save.wSecond < 59)
		save.wSecond += 1;
	else if (save.wMinute < 59)
	{
		save.wMinute += 1;
		save.wSecond = 0;
	}
	else if (save.wHour < 24)
	{
		save.wHour += 1;
		save.wMinute = 0;
		save.wSecond = 0;
	}
	else
		cout << "Veuillez resynchroniser l'holorge."<< endl;
}

#ifdef __cplusplus
}
#endif
#include <Windows.h>
#include <iostream>
#include <signal.h>

using namespace std;

SYSTEMTIME st, save;
bool running = TRUE;

BOOL WINAPI ConsoleHandler(
    DWORD dwCtrlType   //  control signal type
);

void performAction();

int main(int argc, char *argv[])
{
    if (SetConsoleCtrlHandler( (PHANDLER_ROUTINE)ConsoleHandler,TRUE)==FALSE)
    {
        // unable to install handler...
        // display message to the user
        cout << "Unable to install handler!" << endl;
        return -1;
    }

	int i = 1;
    while(TRUE)
    {
		cout << "Ready to launch the execution ?"<<endl;
		system("pause");

		GetSystemTime(&save);

		// Change local time
		GetSystemTime(&st);
		st.wYear = 2015;
		st.wMonth = 02;
		st.wDay = 24;
		SetSystemTime(&st);
		running = true;

		cout << "Time Changed for 60secs, launch now the LEDA-Based program" << endl;
		cout << "Enter CTRL+C to reset time before the end of the timer." << endl;

		int i = 1;
		while (i < 60 && running)
		{
			performAction();
			i++;
		}

		// Reseting time
		SetSystemTime(&save);

		cout << endl << "============================" << endl << endl;
    }
}

BOOL WINAPI ConsoleHandler(DWORD CEvent)
{
    switch(CEvent)
    {
		case CTRL_C_EVENT:
			std::cout << "Stop Sleeping, reseting time." << endl;
			running = FALSE;
			break;
		default :
			SetSystemTime(&save);
    }
    return TRUE;
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
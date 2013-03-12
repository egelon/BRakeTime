#include <windows.h>
#include <fstream>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
using namespace std;

int SLEEP_MINUTES;
int display_info_startup;

void readConfiguration()
{
	ifstream myReadFile;
	myReadFile.open("config.txt");
	char crap[50];
	if (myReadFile.is_open()) 
	{
		while (!myReadFile.eof())
		{
			myReadFile >> crap >> SLEEP_MINUTES;
			myReadFile >> crap >> display_info_startup;
		}
	}
	myReadFile.close();
}

void writeNewConfiguration(int a,int b)
{
	ofstream outputFile;
	outputFile.open("config.txt",ios::out);
	outputFile<<"SLEEP_MINUTES "<<a<<endl;
	outputFile<<"DISPLAY_INFO_BOX "<<b;
	outputFile.close();
}

void printIntroduction(char* appName, int vers, int subvers)
{
	printf("-==[ %s ]==-\n",appName);
	printf("by: Georgi Sabev\n");
	printf("Version %d.%d\n",vers,subvers);
	printf("------------------------------------------------\n");
}

void printInstructions()
{
	printf("Press F5 to display the console\n");
	printf("Press F6 to save and start a new timer\n");
	printf("Press F7 to reset the timer\n");
	printf("Press F8 to exit\n");
	printf("\n");
}

int WinMain(__in  HINSTANCE hInstance, __in  HINSTANCE hPrevInstance, __in  LPSTR lpCmdLine, __in  int nCmdShow)
{
	readConfiguration();
	HWND hwndC = GetConsoleWindow() ;
	bool flag = true;
	bool console_open = false;
	int time;
	int message;
	int min = 0;
	int tens=0;
	int sec = 0;
	char text[1024];
	memset(text, '\0', 1024);
	if (display_info_startup)
		MessageBox( hwndC, "Press F5 to display the console\nPress F6 to save and start a new timer\nPress F7 to reset the timer\nPress F8 to exit\n\nCheck the config file if you want to see this message in the future.", "BreakTime", MB_ICONASTERISK );
	while (flag)
	{
		if (GetAsyncKeyState(VK_F8)) 
		{
			printf("\n Exiting...\n");
			MessageBox( hwndC, "Exiting BreakTime.", "Exit", MB_ICONASTERISK );
			break;
		}
		if (GetAsyncKeyState(VK_F7)) 
		{
			MessageBox( hwndC, "Countdown reset", "Reset", MB_ICONASTERISK );
			printf("\nCountdown reset\n");
			min = 0;
			tens = 0;
			sec = 0;
		}
		if (GetAsyncKeyState(VK_F6)) 
		{
			Sleep (150);
			AllocConsole();
			HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
			int hCrt = _open_osfhandle((long) handle_out, _O_TEXT);
			FILE* hf_out = _fdopen(hCrt, "w");
			setvbuf(hf_out, NULL, _IONBF, 1);
			*stdout = *hf_out;

			HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
			hCrt = _open_osfhandle((long) handle_in, _O_TEXT);
			FILE* hf_in = _fdopen(hCrt, "r");
			setvbuf(hf_in, NULL, _IONBF, 128);
			*stdin = *hf_in;
			printf("\b \b");
			printf("\nConfigure and save a new timer\n");
			printf("Minutes: ");
			printf("\b \b");
			scanf("%d",&time);
			printf("Display the info message at startup?: ");
			scanf("%d",&message);
			writeNewConfiguration(time,message);
			SLEEP_MINUTES = time;
			min = 0;
			tens = 0;
			sec = 0;
			FreeConsole();
		}
		if (GetAsyncKeyState(VK_F5)) 
		{
			Sleep(150);
			if (console_open == false)
				console_open = true;
			else
				console_open = false;
			if (console_open)
			{
				//open it up
				AllocConsole();
				HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
				int hCrt = _open_osfhandle((long) handle_out, _O_TEXT);
				FILE* hf_out = _fdopen(hCrt, "w");
				setvbuf(hf_out, NULL, _IONBF, 1);
				*stdout = *hf_out;

				HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
				hCrt = _open_osfhandle((long) handle_in, _O_TEXT);
				FILE* hf_in = _fdopen(hCrt, "r");
				setvbuf(hf_in, NULL, _IONBF, 128);
				*stdin = *hf_in;

				printIntroduction("BreakTime", 1, 1);
				printInstructions();
				printf("Minutes: %d\n",SLEEP_MINUTES);
				printf("Countdown in progress...\n");
			}
			else
			{
				//close it
				FreeConsole();
			}
		}
		Sleep(500);
		printf("-");
		Sleep(500);
		printf("\b \b");
		printf("|");
		sec ++;
		if (sec >= 10)
		{
			printf("\b \b");
			printf("\b \b");
			printf("\b \b");
			printf("\b \b");
			printf("\b \b");
			printf("\b \b");
			printf("\b \b");
			printf("\b \b");
			printf("\b \b");
			printf("\b \b");
			printf("*");
			sec = 0;
			tens++;
		}
		if (tens >=6)
		{
			printf("\b \b");
			printf("\b \b");
			printf("\b \b");
			printf("\b \b");
			printf("\b \b");
			printf("\b \b");
			printf("$");
			sec = 0;
			tens = 0;
			min ++;
		}
		int response;
		if (min>=SLEEP_MINUTES)
		{
			memset(text,'\0', 1024);
			if (SLEEP_MINUTES == 1)
				sprintf_s(text,1024,"%d minute has passed. Please, take a break.", min);
			else
				sprintf_s(text,1024,"%d minutes have passed. Please, take a break.", min);
			response = MessageBox( hwndC, text, "Time for a break!", MB_ICONWARNING | MB_OKCANCEL | MB_DEFBUTTON1 | MB_SETFOREGROUND | MB_SYSTEMMODAL );
			switch(response)
			{
				case IDOK:
					MessageBox( hwndC, "Currently on break.", "On break", MB_ICONASTERISK );
					for (int k=0; k<SLEEP_MINUTES; k++)
						printf("\b \b");
				break;
				case IDCANCEL:
					flag = false; 
				break;
			}
			min = 0;
		}
	}
	return 0;
}
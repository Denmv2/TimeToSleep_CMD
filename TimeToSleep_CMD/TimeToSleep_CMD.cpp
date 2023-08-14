#include "TimeToSleep_CMD.h"


int main(int argc, char* argv[])
{

	HANDLE token; // Token for initialization
	TOKEN_PRIVILEGES tkp; // Token privilegs
	FILEPATH = argv[0]; //Program path
	ParrentMenu::SetConsoleInfo(); //Set current Console info (color, etc.)

	for (int i = 1; i < argc; i++)  // Check "-SAFEMODE" argument
	{
		std::string argument = argv[i];
		(argv[i] == argument) ? SAFE_MODE = 1 : SAFE_MODE = 0;
	}

	cout << "Initialization..." << endl;
	Sleep(300);
	cout << endl;

	try // try to initialize program 
	{
		initialization(token, tkp);

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		std::cout << "\nInitialization succesful!" << std::endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ParrentMenu::GetConsoleInfo());
		Sleep(200);
	} 
	catch (const MyExeption& ex)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "\nInitialization failed!" << endl;
		cout << ex.what() << ex.GetErrorCode() << endl; // return Erro code 
		ERROR_CODE = ex.GetErrorCode();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ParrentMenu::GetConsoleInfo());
		char input = NULL;

		cout << "\nOpen in safe mode? (y/n) ";  // Ask user to open program in safe mode
		while (input != 'y')
		{
			input = _getch();

			switch (input)
			{
			case 'y':
				SAFE_MODE = true;
				break;

			case 'n':
				cout << "\n\nExiting..." << endl;
				Sleep(500);
				return 0;
				break;

			default:
				break;
			}
		}

	}

	ParrentMenu* Menu = new ParrentMenu("MAIN MENU", 5, {"Shutdown timer", "Restart timer", "Sleep timer", "Hibernation timer", "Turn off all timers"});
	/* Create Menu object */

	while (MENU) //Menu Loop
	{
		system("cls");
		PrintLogo();
		Menu->PrintMenu(); // Print Menu
		Menu->UpdateMenu(_getch(),&Menu); // Waiting action by user
	} 
	delete Menu; // Delete menu object when menu closed.

	return 0;
}


void initialization(HANDLE& token, TOKEN_PRIVILEGES& tkp)
{

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token)) // Get the current process token handle so we can get shutdown privilege. 
	{
		throw MyExeption("Error code: ", GetLastError());
	}

	if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid)) // Get the LUID for shutdown privilege. 
	{
		throw MyExeption("Error code: ", GetLastError());
	}

	tkp.PrivilegeCount = 1;  // one privilege to set    

	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(token, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0)) // Get shutdown privilege for this process. 
	{
		throw MyExeption("Error code: ", GetLastError());
	}


}

void PrintLogo()
{
	if (SAFE_MODE) // Print Safe mode and Error code 
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "	SAFE MODE	"; 
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "Error code: " << ERROR_CODE << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ParrentMenu::GetConsoleInfo());
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);

	/*std::cout <<
		"   _____ _          _____    _____ _             \n"
		"  |_   _|_|_____ __|_   _|__|   __| |___ ___ ___ \n"
		"    | | | |     | -_|| || . |__   | | -_| -_| . |\n"
		"    |_| |_|_|_|_|___||_||___|_____|_|___|___|  _|\n"
		"                                            |_|  \n";*/

	std::cout << "  ==============================================================\n";
	std::cout << " |    _____ _               _____    ____  _                    |\n";
	std::cout << " |   |_   _(_)_ __ ___   __|_   _|__/ ___|| | ___  ___ _ __     |\n";
	std::cout << " |     | | | | '_ ` _ \\ / _ \\| |/ _ \\___ \\| |/ _ \\/ _ \\ '_ \\    |\n";
	std::cout << " |     | | | | | | | | |  __/| | (_) |__) | |  __/  __/ |_) |   |\n";
	std::cout << " |     |_| |_|_| |_| |_|\\___||_|\\___/____/|_|\\___|\\___| .__/    |\n";
	std::cout << " |                                                    |_|       |\n";
	std::cout << "  ==============================================================\n\n\n";

	/*std::cout << "    _____ _               _____    ____  _                    \n";
	std::cout << "   |_   _(_)_ __ ___   __|_   _|__/ ___|| | ___  ___ _ __     \n";
	std::cout << "     | | | | '_ ` _ \\ / _ \\| |/ _ \\___ \\| |/ _ \\/ _ \\ '_ \\    \n";
	std::cout << "     | | | | | | | | |  __/| | (_) |__) | |  __/  __/ |_) |   \n";
	std::cout << "     |_| |_|_| |_| |_|\\___||_|\\___/____/|_|\\___|\\___| .__/    \n";
	std::cout << "                                                    |_|       \n";
	std::cout << "                                                              \n";*/

	/*std::cout << "////////////////////////////////////////////////////////////////////\n";
	std::cout << "///    _____ _               _____    ____  _                    ///\n";
	std::cout << "///   |_   _(_)_ __ ___   __|_   _|__/ ___|| | ___  ___ _ __     ///\n";
	std::cout << "///     | | | | '_ ` _ \\ / _ \\| |/ _ \\___ \\| |/ _ \\/ _ \\ '_ \\    ///\n";
	std::cout << "///     | | | | | | | | |  __/| | (_) |__) | |  __/  __/ |_) |   ///\n";
	std::cout << "///     |_| |_|_| |_| |_|\\___||_|\\___/____/|_|\\___|\\___| .__/    ///\n";
	std::cout << "///                                                    |_|       ///\n";
	std::cout << "///                                                              ///\n";
	std::cout << "////////////////////////////////////////////////////////////////////\n\n";*/
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

MyExeption::MyExeption(const char* message, int ErrorCode) : exception(message)
{
	this->ErrorCode = ErrorCode;
}

int MyExeption::GetErrorCode() const
{
	return ErrorCode;
}




#include "Menu.h"


extern std::string FILEPATH; //Program path

extern bool MENU; // Loop active Menu

extern bool SAFE_MODE; // Safe mode 

static CONSOLE_SCREEN_BUFFER_INFO ConsoleInfo; // Console info

static std::thread T_SleepMode; // Thread for Sleep/Hibernation timer



////////////////////////////////////////////////////////////////////////// - Parent Menu class

	ParrentMenu::ParrentMenu(std::string MenuName,int MenuLenght, std::initializer_list<std::string> MasMenu)
	{
		cursor = 1; // Set cursor 
		this->MenuName = "\t" + MenuName + "\n"; //set menu name with tabulation
		this->MenuLenght = MenuLenght; //Set menu lenght
		Var_Menu = new string[MenuLenght]; // Create Menu massive object
		std::copy(MasMenu.begin(), MasMenu.end(), Var_Menu); // Transferring a menu from an incoming variable to a menu variable
	}

	ParrentMenu::~ParrentMenu()
	{
		delete[] Var_Menu; // Delete Menu object
		Var_Menu = nullptr; // Clearing the pointer
	}


void ParrentMenu::PrintMenu()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN);
	std::cout << MenuName << std::endl; // Print menu name
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GetConsoleInfo());

	for (int i = 0; i < MenuLenght; i++)
	{
		if ((i + 1) == cursor)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE | BACKGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			std::cout <<"#       -> "  << Var_Menu[i]; //print selected menu
			for (size_t j = 0; (54 - Var_Menu[i].length() > j); j++) // Calculate menu line
			{
				std::cout << " "; // Print selected menu line 
			}
			std::cout << std::endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GetConsoleInfo());
		}
		else
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			std::cout << "#\t" << Var_Menu[i] << std::endl; // Print non selected menu line
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GetConsoleInfo());
		}
	}
}

void ParrentMenu::UpdateMenu(int MenuValue, ParrentMenu** PMenu)
{
	switch (MenuValue) 
	{
	case 72: // up arrow (code 72 in the library conio.h)
		(cursor == 1) ? cursor = MenuLenght : --cursor;
		break;
	case 80: // down arrow (code 80 in the library conio.h)
		(cursor == MenuLenght) ? cursor = 1 : ++cursor;
		break;
	case 13: // Enter (code 13 in the library conio.h)
		SelectMenu(PMenu);
		break;
	case 27: // Esc (code 27 in the library conio.h)
		EscMenu(PMenu);
		break;
	}
}

void ParrentMenu::SelectMenu(ParrentMenu** PMenu)
{
	switch (cursor) //Create New Menu
	{
	case 1:
		*PMenu = nullptr;
		*PMenu = new TimerMenu("SHUTDOWN TIMER MENU", 3, {"Hours: ", "Minutes: ", "Seconds: "}, false);
		delete this;
		break;

	case 2:
		*PMenu = nullptr;
		*PMenu = new TimerMenu("RESTART TIMER MENU", 3, {"Hours: ", "Minutes: ", "Seconds: "}, true);
		delete this;
		break;

	case 3:
		*PMenu = nullptr;
		*PMenu = new SleepMenu("SLEEP TIMER MENU", 3, {"Hours: ", "Minutes: ", "Seconds: "}, false);
		delete this;
		break;

	case 4:
		*PMenu = nullptr;
		*PMenu = new SleepMenu("HIBERNATION TIMER MENU", 3, {"Hours: ", "Minutes: ", "Seconds: "}, true);
		delete this;
		break;

	case 5:

		if (SAFE_MODE) //Check if SafeMode on
		{
			system("shutdown -a");
			PrintMessage("Shutdown/Restart timer is disabled!", false, 0);
		}
		else
		{
			if (AbortSystemShutdown(NULL)) // Disable Shutdown/Restart timer
			{
				PrintMessage("Shutdown/Restart timer is disabled!", false, 0);
			}
			else
			{
				PrintMessage("Shutdown/Restart not active!", true, 0);
			}
		}

		if (T_SleepMode.joinable()) // Disable Sleep/Hibernation timer
		{
			PrintMessage("Sleep/Hibernation timer is disabled!", false);
			SleepMenu::DisableT_SleepModeTimer(true);
		}
		else
		{
			PrintMessage("Sleep/Hibernation timer not active!", true, 1);
		}

		break;

	default:
		break;
	}
}

void ParrentMenu::EscMenu(ParrentMenu** PMenu)
{
	MENU = false; // Close Menu
}

void ParrentMenu::PrintMessage(string Msg, bool Err, DWORD Sec)
{
	Sec *= 1000; // Get milliseconds
	(Err == false) ? SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY)
	: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
	/* Select current color for text */
	
	std::cout << "\n\n";
	for (size_t i = 0; (Msg.length() + 3 > i); i++) // Calculate border for text
	{
		std::cout << '-'; // Print border 
	}

	std::cout << std::endl << Msg << std::endl;

	for (size_t i = 0; (Msg.length() + 3 > i); i++) // Calculate border for text
	{
		std::cout << '-'; // Print border 
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GetConsoleInfo());
	Sleep(Sec);
}

void ParrentMenu::SetConsoleInfo()
{
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsoleInfo); // Save current CMD info
}

WORD ParrentMenu::GetConsoleInfo()
{
	return ConsoleInfo.wAttributes; //Get saved CMD info
}

////////////////////////////////////////////////////////////////////////// - Timer Menu class

	 TimerMenu::TimerMenu(std::string MenuName, int MenuLenght, std::initializer_list<std::string> MasMenu, bool Reboot) : ParrentMenu(MenuName, MenuLenght, MasMenu)
	 {
		 this->Reboot = Reboot;
	 }


void TimerMenu::PrintMenu()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN);
	std::cout << MenuName << std::endl; // Print menu name
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GetConsoleInfo());


	for (int i = 0; i < MenuLenght; i++)
	{
		if ((i + 1) == cursor)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE | BACKGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			std::cout << "#       -> " << Var_Menu[i] << Time[i]; //print selected menu

			for (size_t j = 0; (54 - (Var_Menu[i].length() + NumbLenght(Time[i])) > j); j++) // Calculate menu line
			{
				std::cout << " "; // Print selected menu line 
			}
			std::cout << std::endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GetConsoleInfo());
		}
		else
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			std::cout << "#\t" << Var_Menu[i] << Time[i] << std::endl; // Print non selected menu line
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GetConsoleInfo());
		}





		//((i + 1) == cursor) ? std::cout << "-> " << Var_Menu[i] << Time[i] << std::endl : std::cout << Var_Menu[i] << Time[i] << std::endl;
	}

}

void TimerMenu::UpdateMenu(int MenuValue, ParrentMenu** PMenu)
{
	switch (MenuValue)
	{
	case 72: // up arrow (code 72 in the library conio.h)
		(cursor == 1) ? cursor = MenuLenght : --cursor;
		break;
	case 80: // down arrow (code 80 in the library conio.h)
		(cursor == MenuLenght) ? cursor = 1 : ++cursor;
		break;
	case 75: // Стрелка влево (код 75 в библиотеке conio.h)
		UpdateTime(false);

		break;
	case 77: // Стрелка вправо (код 77 в библиотеке conio.h)
		UpdateTime(true);
		break;
	case 13: // Enter (code 13 in the library conio.h)
		SelectMenu(PMenu);
		break;
	case 27: // Esc (code 27 in the library conio.h)
		EscMenu(PMenu);
		break;
	}
}

void TimerMenu::SelectMenu(ParrentMenu** PMenu)
{
	if (SAFE_MODE) //Check if SafeMode on
	{
		std::string CommandLine = "shutdown ";

		(Reboot == false) ? CommandLine.append("/s /t ") : CommandLine.append("/r /t ");
		CommandLine.append(std::to_string(ConvertationTime()));
		system(CommandLine.c_str()); // Set Shutdown/Restart timer (SAFE MODE)
		PrintMessage("Timer is set!");
		EscMenu(PMenu);
	}
	else
	{
		(AbortSystemShutdown(NULL) == 1) ? PrintMessage("Timer has been changed!") : PrintMessage("Timer is set!");
		InitiateSystemShutdownEx(NULL, NULL, ConvertationTime(), true, Reboot, NULL); // Set Shutdown/Restart timer
		EscMenu(PMenu);
	}
}

void TimerMenu::EscMenu(ParrentMenu** PMenu)
{
	*PMenu = nullptr; // Go down to the menu below
	*PMenu = new ParrentMenu("MAIN MENU", 5, {"Shutdown timer", "Restart timer", "Sleep timer", "Hibernation timer", "Turn off all timers"});

	delete this;

}

void TimerMenu::UpdateTime(bool Add)
{
	switch (cursor - 1)
	{
	case 0:
		if (Add) // Increase or decrease the number
		{
			(Time[cursor - 1] == 168) ? Time[cursor - 1] = 0 : Time[cursor - 1]++; // Checking boundaries and setting a timer
		}
		else
		{
			(Time[cursor - 1] == 0) ? Time[cursor - 1] = 168 : Time[cursor - 1]--; // Checking boundaries and setting a timer
		}
		break;

	case 1:
		if (Add) // Increase or decrease the number
		{
			(Time[cursor - 1] == 60) ? Time[cursor - 1] = 0 : Time[cursor - 1]++; // Checking boundaries and setting a timer
		}
		else
		{
			(Time[cursor - 1] == 0) ? Time[cursor - 1] = 60 : Time[cursor - 1]--; // Checking boundaries and setting a timer
		}
		break;

	case 2:
		if (Add) // Increase or decrease the number
		{
			(Time[cursor - 1] == 60) ? Time[cursor - 1] = 0 : Time[cursor - 1]++; // Checking boundaries and setting a timer
		}
		else
		{
			(Time[cursor - 1] == 0) ? Time[cursor - 1] = 60 : Time[cursor - 1]--; // Checking boundaries and setting a timer
		}
		break;



	default:
		break;
	}
}

long int TimerMenu::ConvertationTime()
{
	return ((Time[0] * 60 * 60) + (Time[1] * 60) + Time[2]); // Convert time into seconds
}

int TimerMenu::NumbLenght(int Number)
{
	int numberOfDigits = 0;
	if (Number == 0) { return 1; }
	else
	{
		while (Number > 0)
		{
			Number /= 10;
			numberOfDigits++;
		}
		return numberOfDigits; // Getting the length of a number in int type for the correct display of the menu line
	}

}

////////////////////////////////////////////////////////////////////////// - Sleep Menu class

	 SleepMenu::SleepMenu(std::string MenuName, int MenuLenght, std::initializer_list<std::string> Menumas, bool Reboot) : TimerMenu(MenuName, MenuLenght, Menumas, Reboot)
{

}


void SleepMenu::SelectMenu(ParrentMenu** PMenu)
	 {

		if (!DisableT_SleepModeTimer(true)) // Checking the active timer
		{
			long int Time = ConvertationTime();

			T_SleepMode = std::thread(SleepMenu::SetT_SleepModeTimer, Time, Reboot); // Set Sleep/Hibernation timer 
			PrintMessage(" Timer is set!   Do not close the program so that the timer does not reset! ", false, 3);
			EscMenu(PMenu);
		}
		else
		{
			PrintMessage("Disable old timer!", false, 1);
		}
	 }

void SleepMenu::SetT_SleepModeTimer(int Seconds, BOOL Reboot)
{
	std::this_thread::sleep_for((std::chrono::seconds)Seconds); // Execution delay in seconds
	SetSuspendState(Reboot, TRUE, TRUE); // Putting the computer into sleep/hibernation mode
	DisableT_SleepModeTimer(false); // Close the program
}

bool SleepMenu::DisableT_SleepModeTimer(bool RestartProgram)
{
	if (T_SleepMode.joinable()) // Checking the active timer
	{

			if (RestartProgram) // Checking to restart the program
			{
				std::string StartFile = "start " + FILEPATH; 
				system("cls");
				system(StartFile.c_str()); // Open new program
			}

		T_SleepMode.detach(); // deatach thread with timer 
		MENU = false; // Close current program 
		return 1; 
	}
	else
	{
		return 0;
	}
}

std::thread& SleepMenu::GetT_SleepMode()
{
	return T_SleepMode; // Get link to the current thread
}


#pragma once
#include <string>
#include <iostream>
#include <conio.h>
#include "Windows.h"
#include <Powrprof.h>
#pragma comment(lib, "Powrprof.lib")
#include <chrono>
#include <thread>


using std::string;


////////////////////////////////////////////////////////////////////////// - parent Menu class

class ParrentMenu
{

public:

	ParrentMenu(std::string MenuName,int MenuLenght, std::initializer_list<std::string> Menumas);

	~ParrentMenu();

	virtual void PrintMenu(); // Print menu in to CMD

	virtual void UpdateMenu(int MenuValue, ParrentMenu** PMenu); // Updating the movement in the menu

	virtual void SelectMenu(ParrentMenu** PMenu); // Selecting a dedicated menu

	virtual void EscMenu(ParrentMenu** PMenu); // // Close current Menu

	void PrintMessage(string Msg, bool Err = false, DWORD Sec = 1); // Print message in CMD for user


	static void SetConsoleInfo(); // Save current CMD info

	static WORD GetConsoleInfo(); // Get saved CMD info

protected:

	int cursor;
	int MenuLenght;
	string* Var_Menu; // Menu
	string MenuName; // Menu name

};


////////////////////////////////////////////////////////////////////////// - Timer Menu class

class TimerMenu : public ParrentMenu
{

public:

	TimerMenu(std::string MenuName, int MenuLenght, std::initializer_list<std::string> Menumas, bool Reboot = false);

	void PrintMenu() override; // Print menu in to CMD

	void UpdateMenu(int MenuValue, ParrentMenu** PMenu) override; // Updating the movement in the menu

	void SelectMenu(ParrentMenu** PMenu) override; // Selecting a dedicated menu

	void EscMenu(ParrentMenu** PMenu) override; // Close current Menu

	void UpdateTime(bool Add); // Setting the time and checking the boundaries

	long int ConvertationTime(); // Get time in seconds

	int NumbLenght(int Number); // Getting the length of a number in int type for the correct display of the menu line

protected:
	bool Reboot; // Shutdown/Reboot, Sleep/Hibernation
	unsigned short Time[3]{ 0,0,0 };
	
};


////////////////////////////////////////////////////////////////////////// - Timer Menu class

class SleepMenu : public TimerMenu
{
public:

	SleepMenu(std::string MenuName, int MenuLenght, std::initializer_list<std::string> Menumas, bool Reboot = false);

	void SelectMenu(ParrentMenu** PMenu) override; // Selecting a dedicated menu

	static void SetT_SleepModeTimer(int Seconds, BOOL Reboot); // Set new thread for sleep timer

	static bool DisableT_SleepModeTimer(bool RestartProgram); // Disable sleep timer

	static std::thread& GetT_SleepMode(); // Get sleep mode timer

};




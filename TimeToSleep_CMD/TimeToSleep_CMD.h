#pragma once
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "Menu.h"
#include <winreg.h>
#include <powrprof.h>

using std::cout;
using std::endl;

bool SAFE_MODE = false;  // Safe mode 
int ERROR_CODE = 0;   // Error code for incorrect startup
bool MENU = true;    // Loop active Menu
string FILEPATH; //Program path


class MyExeption : public std::exception // Custom Exeption class for return Error code. 
{
public:
	MyExeption(const char* message, int ErrorCode);

	int GetErrorCode() const;

	

private:
	int ErrorCode;
};  

void initialization(HANDLE& token, TOKEN_PRIVILEGES& tkp); //  Initialization program before start

void PrintLogo(); // Print Program Logo




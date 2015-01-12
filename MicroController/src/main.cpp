/*
 * main.cpp
 *
 *  Created on: Jul 22, 2012
 *      Author: huy
 */
#include <iostream>
#include <string>
#include <cctype>
#include "utility.h"
#include "Microcontroller.h"
#include "MicrocontrollerFactory.h"


using namespace MicrocontrollerEmulation;

int main() {
	// Microcontroller Factory and Microcontroller pointer
	MicrocontrollerFactory * factory = new MicrocontrollerFactory();
	Microcontroller * microcontroller = NULL;

	// Display greeting
	std::cout << "Welcome to Microcontroller Emulator!\n"
			<< "Type 'h' if you need help" << std::endl;

	// Get validated command and utilize
	std::string commandLine;
	do {
		// Get command line and create input string stream
		commandLine = getCommand();

		// Utilize user command, and call corresponding function
		if (commandLine.length()) {
			utilize(commandLine, factory, microcontroller);
		}
	} while (!(commandLine.length() && tolower(commandLine[0]) == 'q'));

	// Display farewell
	std::cout << "Thanks for using Microcontroller Emulator!\n"
			<< "The program will now exit!" << std::endl;

	// Terminate program
	return 0;
}


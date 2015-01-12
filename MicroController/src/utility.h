/*
 * utility.h
 *
 *  Created on: Jan 11, 2012
 *      Author: huy
 */

#ifndef SRC_UTILITY_H_
#define SRC_UTILITY_H_

#include <string>
#include "Microcontroller.h"
#include "MicrocontrollerFactory.h"

namespace MicrocontrollerEmulation {
// Function prototypes
const std::string getCommand();	// Get user command
const bool isValidCommand(const std::string& command);// Check if an input is a valid command
const std::string toUpper(const std::string& input);// Convert an input string to upper-case
const std::string toLower(const std::string& input);// Convert an input string to lower-case
const bool isValidHex(const std::string& input);// Check if an input is a valid hexadecimal value
void utilize(const std::string& commandLine,
		const MicrocontrollerFactory * factory,
		Microcontroller *& microcontroller);// Utilize command and call corresponding function
void validateExecution(const Microcontroller * microcontroller,
		const int& singal);	// Validate execution
void save(const Microcontroller * microcontroller);	// Save microcontroller state
void load(Microcontroller * microcontroller);	// Load microcontroller state
Microcontroller * connect(const MicrocontrollerFactory * factory,
		const std::string& type = "");	// Connect (create) microcontroller
void display(const Microcontroller * microcontroller);// Display all memory of specified microcontroller
void execute(Microcontroller * microcontroller);	// Execute from current PC
void go(Microcontroller * microcontroller, const bool& withParam = false,
		const int& location = 0);	// Execute from a specific location
void displayMenu();	// Display Help menu
void look(const Microcontroller * microcontroller,
		const bool& withParam = false, const int& location = 0);// Look at a specific memory location
void modify(Microcontroller * microcontroller, const bool& withParam = false,
		const int& location = 0, const int& value = 0);	// Modify a specific memory location
void reset(Microcontroller * microcontroller);	// Reset microcontroller
void status(const Microcontroller * microcontroller);// Display PC and registers
}

#endif /* SRC_UTILITY_H_ */

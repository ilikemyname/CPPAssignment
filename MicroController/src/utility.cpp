/*
 * utility.cpp
 *
 *  Created on: Jan 11, 2012
 *      Author: huy
 */

#include "utility.h"
#include "Microcontroller.h"
#include "MicrocontrollerFactory.h"
#include <iostream>
#include <string>
#include <cctype>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <unistd.h>

namespace MicrocontrollerEmulation
{
	// Function to get command from user
	const std::string getCommand ()
	{
		// Command line string
		std::string command;

		// Prompt user for command
		std::cout << "> ";

		// Get user command line
		getline(std::cin, command);

		// If UNIX pipe is used, print command on screen
		if (!isatty(STDIN_FILENO))
		{
			std::cout << command << std::endl;
		}

		// Re-prompt user until valid command line is inserted
		while (!isValidCommand(command))
		{
			// Display error message
			std::cerr << "Invalid command! "
					  << "Type 'h' for help." << std::endl;

			// Prompt user for command
			std::cout << "> ";

			// Get user command line
			getline(std::cin, command);
		}

		// Return validated command line
		return command;
	}

	// Check if input is a valid command
	const bool isValidCommand (const std::string& input)
	{
		// If input is not empty, validate command
		if (input.length())
		{
			// Get command character (lower-cased)
			char command = tolower(input[0]);

			// Check for Load, Save, Display, Execute,
			// Help, Reset, Status and Quit commands
			if (command == '<' || command == '>' || command == 'd'
					|| command == 'e' || command == 'h' || command == 'r'
					|| command == 's' || command == 'q')
			{
				// If input is not single-character, return failure
				if (input.length() > 1)
				{
					return false;
				}
			}
			else if (command == 'c' || command == 'g' || command == 'l'
						|| command == 'm')
			{
				// Check for Connect, Go, Look and Modify commands

				// If input is single-character, validate parameter(s)
				if (input.length() > 1)
				{
					// If input length is 2, return failure
					if (input.length() == 2)
					{
						return false;
					}

					// Number of spaces and their positions
					int spaces = 0;
					size_t previous = std::string::npos, current;

					// Start finding spaces in the input string
					current = input.find(" ");

					// If first space is not after command character,
					// return failure
					if (current != 1)
					{
						return false;
					}

					// Stop finding if no more spaces found
					while (current != std::string::npos)
					{
						// If 2 contiguous spaces detected, return failure
						if (previous != std::string::npos &&
								current == previous + 1)
						{
							return false;
						}

						// Increment space count by 1
						spaces++;

						// Update space positions
						previous = current;
						current = input.find(" ", previous + 1);
					}

					// Create input string stream
					// and ignore command character (followed by a space)
					std::istringstream stream(input);
					stream.ignore(2);

					// If number of spaces is 1 and command is not of the
					// first 3, or number of spaces is 2 and command is
					// not of the last one, return failure
					if ((spaces == 1 &&	!(command == 'c'
							|| command == 'g' || command == 'l'))
						|| (spaces == 2 && command != 'm') || spaces > 2)
					{
						return false;
					}
				}
			}
			else
			{
				// Else, if command is invalid, return failure
				return false;
			}
		}

		// Else, return success
		return true;
	}

	// Convert an input string to upper-case
	const std::string toUpper (const std::string& input)
	{
		// Output string
		std::string output = "";

		// Convert each letter of input to upper-case and save to output
		for (int i = 0; i < (int) input.length(); i++)
		{
			output += toupper(input[i]);
		}

		// Return upper-cased string
		return output;
	}

	// Convert an input string to lower-case
	const std::string toLower (const std::string& input)
	{
		// Output string
		std::string output = "";

		// Convert each letter of input to lower-case and save to output
		for (int i = 0; i < (int) input.length(); i++)
		{
			output += tolower(input[i]);
		}


		// Return lower-cased string
		return output;
	}

	// Check if an input is a valid hexadecimal value
	const bool isValidHex (const std::string& input)
	{
		// Loop through input string to find invalid character
		for (int i = 0; i < (int) input.length(); i++)
		{
			// Get current character
			char current = tolower(input[i]);

			// If invalid charcter found, return failure
			if (((current < '0' || current > '9') && (current < 'a'
					|| current > 'f') && i != 1) || (current == 'x'
						&& input[0] != '0'))
			{

				return false;
			}
		}

		// Else, return success
		return true;
	}

	// Utilize command and call corresponding function
	void utilize (const std::string& commandLine,
			const MicrocontrollerFactory * factory,
			Microcontroller *& microcontroller)
	{
		// Command character
		char command;

		// Create input string stream from command line string
		std::istringstream stream(commandLine);

		// Get command character
		stream >> command;
		command = tolower(command);

		// Invalid status (0 = valid, 1 = invalid at parameter #1,
		// 2 = invalid at parameter #2) and last space position
		int invalid = 0, lastSpacePosition;

		// Loop through command line to find invalid character
		for (int i = 1, spaces = 0; (command == 'g'
					|| command == 'l' || command == 'm')
					&& !invalid	&& i < (int) commandLine.length(); i++)
		{
			// If space detected, increment space counter
			// and update last space position
			if (commandLine[i] == ' ')
			{
				spaces++;
				lastSpacePosition = i;
			}
			else
			{
				// Else, validate character

				// Get current character
				char current = tolower(commandLine[i]);

				// If invalid character found, set status code
				if (((current < '0' || current > '9') && (current < 'a'
						|| current > 'f') && i != lastSpacePosition + 2)
							|| (current == 'x'
								&& commandLine[lastSpacePosition + 1] != '0'))
				{
					invalid = spaces;
				}
			}
		}

		// If command is not Connect, Help and Quit and microcontroller
		// is not connected, display error message
		if (!(command == 'c' || command == 'h'
				|| command == 'q' || microcontroller))
		{
			std::cerr << "Microcontroller not found! "
					  << "Please connect to a microcontroller!" << std::endl;
		}
		else
		{
			// Else, call corresponding function with parameter(s)
			switch (command)
			{
				case '<':
					load(microcontroller);
					break;
				case '>':
					save(microcontroller);
					break;
				case 'c':
					// Insert parameter(s) if existed
					if (commandLine.length() > 1)
					{
						// Get parameter(s)
						std::string type;
						stream >> type;

						// Call parameterized function
						microcontroller = connect(factory, type);
					}
					else
					{
						// Else, call function with no parameter
						microcontroller = connect(factory);
					}
					break;
				case 'd':
					display(microcontroller);
					break;
				case 'e':
					execute(microcontroller);
					break;
				case 'g':
					// Insert parameter(s) if existed
					if (commandLine.length() > 1)
					{
						// If invalid character found,
						// set negative (invalid) value for parameter(s)
						if (invalid)
						{
							go(microcontroller, true, -1);
						}
						else
						{
							// Else, get validated parameter(s)
							// and call function

							// Get parameter(s)
							int location;
							stream >> std::hex
								   >> std::setw(commandLine.length() - 2)
								   >> location;

							// Call parameterized function
							go(microcontroller, true, location);
						}
					}
					else
					{
						// Else, call function with no parameter
						go(microcontroller);
					}
					break;
				case 'h':
					displayMenu();
					break;
				case 'l':
					// Insert parameter(s) if existed
					if (commandLine.length() > 1)
					{
						// If invalid character found,
						// set negative (invalid) value for parameter(s)
						if (invalid)
						{
							look(microcontroller, true, -1);
						}
						else
						{
							// Else, get validated parameter(s)
							// and call function

							// Get parameter(s)
							int location;
							stream >> std::hex
								   >> std::setw(commandLine.length() - 2)
								   >> location;

							// Call parameterized function
							look(microcontroller, true, location);
						}
					}
					else
					{
						// Else, call function with no parameter
						look(microcontroller);
					}
					break;
				case 'm':
					// Insert parameter(s) if existed
					if (commandLine.length() > 1)
					{
						// If invalid character found,
						// set negative (invalid) value for parameter(s)
						if (invalid == 1)
						{
							modify(microcontroller, true, -1, 0);
						}
						else if (invalid == 2)
						{
							modify(microcontroller, true, 0, -1);
						}
						else
						{
							// Else, get validated parameter(s)
							// and call function

							// Get parameter(s)
							int location;
							int value;
							stream >> std::hex
								   >> std::setw(commandLine.length() - 2)
								   >> location >> value;

							// Call parameterized function
							modify(microcontroller, true, location, value);
						}
					}
					else
					{
						// Else, call function with no parameter
						modify(microcontroller);
					}
					break;
				case 'r':
					reset(microcontroller);
					break;
				case 's':
					status(microcontroller);
					break;
			}
		}
	}

	// Validate execution
	void validateExecution (const Microcontroller * microcontroller,
			const int& signal)
	{
		// Display error message for corresponding signal
		switch (signal)
		{
			case Microcontroller::SIGWEED:
				std::cerr << "SIGWEED. Program executed past top of memory"
						  << std::endl;
				break;
			case Microcontroller::SIGOP:
				std::cerr << "SIGOP. Invalid opcode. Program Counter = 0x"
						  << std::hex << std::setw(2) << std::setfill('0')
						  << microcontroller->getPC()
						  << std::endl;
				break;
			case Microcontroller::HALT:
				std::cout << "Program halted" << std::endl;
				break;
		}
	}

	// Load microcontroller state
	void load (Microcontroller * microcontroller)
	{
		// Create output string stream to build filename
		std::string filename = "save." + toLower(microcontroller->getType());

		// Open file for reading
		std::ifstream fstream(filename.c_str());

		// If file exists, read from file
		if (fstream)
		{
			// Pass file stream to microcontroller and check for status
			int status = microcontroller->setState(fstream);

			// If state not loaded, display error message
			if (status)
			{
				std::cout << "Unknown state at line " << status
						  << std::endl;
			}
			else
			{
				// Else, display success message
				std::cerr << "Loaded successfully" << std::endl;
			}

			// Close file
			fstream.close();
		}
		else
		{
			// Else, display error message
			std::cerr << "Saved slot not found!" << std::endl;
		}
	}

	// Save microcontroller state
	void save (const Microcontroller * microcontroller)
	{
		// Create output string stream to build filename
		std::string filename = "save." + toLower(microcontroller->getType());

		// Open file for writing
		std::ofstream fstream(filename.c_str(), std::ofstream::trunc);

		// If file exists, write to file
		if (fstream)
		{
			// Write to stream
			fstream << microcontroller->getState();

			// Display success message
			std::cout << "Saved successfully" << std::endl;

			// Close file
			fstream.close();
		}
		else
		{
			// Else, display error message
			std::cerr << "Cannot save to slot!" << std::endl;
		}
	}

	// Connect to microcontroller
	Microcontroller * connect (const MicrocontrollerFactory * factory, const std::string& type)
	{
		// Type input
		std::string typeInput = "";

		// If type is provided, fetch it
		if (type.length())
		{
			// Convert type input to upper-case
			typeInput = toUpper(type);
		}
		else
		{
			// Else, get microcontroller type from user

			// Prompt user for a type
			std::cout << "Please type in one of these microcontroller types:\n"
					  << MicrocontrollerFactory::TYPES[0];
			for (int i = 1; i < MicrocontrollerFactory::numberOfTypes(); i++)
			{
				std::cout << ", " << MicrocontrollerFactory::TYPES[i];
			}
			std::cout << ".\n"
					  << "> type? ";
			getline(std::cin, typeInput);

			// Convert type input to upper-case
			typeInput = toUpper(typeInput);
		}

		// Create microcontroller from factory
		Microcontroller * microcontroller =
			factory->createMicrocontroller(typeInput);

		// If microcontroller is created, display success message
		if (microcontroller)
		{
			std::cout << typeInput << " selected" << std::endl;

			// Initialize microcontroller
			reset(microcontroller);
		}
		else
		{
			// If no microcontroller created, display error
			std::cerr << "Invalid type" << std::endl;
		}

		// Return pointer to microcontroller
		return microcontroller;
	}

	// Display all memory
	void display (const Microcontroller * microcontroller)
	{
		// Display column header
		std::cout << "    ";
		for (int i = 0; i < 0x10; i++)
		{
			// Separate display by byte
			if (i == 0 || i == 8)
			{
				std::cout << ' ';
			}

			std::cout << std::hex
					  << " 0" << i;
		}
		std::cout << '\n' << std::endl;

		// Display memory content
		for (int i = 0; i < microcontroller->getMemorySize(); i += 0x10)
		{
			// Display row header
			std::cout << std::hex << std::setw(4) << std::setfill('0')
					  << i;

			// Display memory row content
			for (int j = 0; j < 0x10; j++)
			{
				// Separate display by byte
				if (j == 0 || j == 8)
				{
					std::cout << ' ';
				}

				std::cout << ' '
						  << std::hex << std::setw(2) << std::setfill('0')
						  << (int) microcontroller->look(i + j);
			}
			std::cout << std::endl;
		}
	}

	// Execute from current PC
	void execute (Microcontroller * microcontroller)
	{
		// Execute and check for status
		validateExecution(microcontroller, microcontroller->execute());
	}

	// Execute from a specific location
	void go (Microcontroller * microcontroller, const bool& withParam,
			const int& location)
	{
		// Memory location input
		int locationInput;

		// If memory address id provided, use it
		if (withParam)
		{
			locationInput = location;
		}
		else
		{
			// Else, get location from user

			// Prompt user for memory location
			std::string input;
			std::cout << "> location? ";
			getline(std::cin, input);

			// If input string is empty or contains space(s) or of wrong type,
			// set negative (invalid) location
			if (!(input.length() && input.find(" ") == std::string::npos
						&& isValidHex(input)))
			{
				locationInput = -1;
			}
			else
			{
				// Else, get validated parameter(s)

				// Create input string stream and parse value
				std::istringstream stream(input);
				stream >> std::hex >> std::setw(10)
					   >> locationInput;
			}
		}

		// If memory address is valid, start executing
		if (locationInput >= 0
				&& locationInput < microcontroller->getMemorySize())
		{
			// Check for execution status
			validateExecution(microcontroller,
					microcontroller->execute(locationInput));
		}
		else
		{
			// Else, display error message
			std::cerr << "Invalid address" << std::endl;
		}
	}

	// Function to display Help Menu
	void displayMenu ()
	{
		std::cout << "\nThis is the Microcontroller Emulation Program.\n"
				  << "Usage: main\n"
				  << "       main < {command file}\n\n"
				  << "List of available commands (case-insensitive):\n"
				  << "  <               Load saved state\n"
				  << "  >               Save current state\n"
				  << "  c [type]        Connect to microcontroller ('Create')\n"
				  << "                  Microcontroller type can be entered directly or\n"
				  << "                  prompted later. Possible types are:\n"
				  << "                  " << MicrocontrollerFactory::TYPES[0];

		// List all microcontroller types
		for (int i = 1; i < MicrocontrollerFactory::numberOfTypes(); i++)
		{
			std::cout << ", " << MicrocontrollerFactory::TYPES[i];
		}
		std::cout << ".\n"
				  << "  d               Display all memory\n"
				  << "  e               Execute from current PC\n"
				  << "  g [addr]        Execution from a specific location ('Go')\n"
				  << "                  Memory location (addr) can be entered directly\n"
				  << "                  or prompted later. Memory location (addr) must\n"
				  << "                  be in hexadecimal format.\n"
				  << "  h               Display Help\n"
				  << "  l [addr]        Look at memory\n"
				  << "                  Memory location (addr) can be entered directly\n"
				  << "                  or prompted later. Memory location (addr) must\n"
				  << "                  be in hexadecimal format.\n"
				  << "  m [addr] [val]  Modify memory\n"
				  << "                  Memory location (addr) and value (val) can be\n"
				  << "                  entered directly or prompted later. Memory\n"
				  << "                  location (addr) and value (val) must be in\n"
				  << "                  hexadecimal format. Only the right most byte\n"
				  << "                  of the value is stored.\n"
				  << "  r               Reset microcontroller\n"
				  << "  s               Display PC and registers ('Status')\n"
				  << "  q               Quit the program\n" << std::endl;
	}

	// Look at a specific memory location
	void look (const Microcontroller * microcontroller, const bool& withParam,
			const int& location)
	{
		// Memory location input
		int locationInput;

		// If memory address is provided, use it
		if (withParam)
		{
			locationInput = location;
		}
		else
		{
			// Else, get location from user

			// Prompt user for memory location
			std::string input;
			std::cout << "> location? ";
			getline(std::cin, input);

			// If input string is empty or contains space(s) or of wrong type,
			// set negative (invalid) location
			if (!(input.length() && input.find(" ") == std::string::npos
					&& isValidHex(input)))
			{
				locationInput = -1;
			}
			else
			{
				// Else, get validated parameter(s)

				// Create input string stream and parse value
				std::istringstream stream(input);
				stream >> std::hex >> std::setw(10)
					   >> locationInput;
			}
		}

		// If memory address is valid, display the value
		if (locationInput >= 0
				&& locationInput < microcontroller->getMemorySize())
		{
			std::cout << "The value is: 0x"
					  << std::hex << std::setw(2) << std::setfill('0')
					  << (int) microcontroller->look(locationInput)
					  << std::endl;
		}
		else
		{
			// Else, display error message
			std::cerr << "Invalid address" << std::endl;
		}
	}

	// Modify a specific memory location
	void modify (Microcontroller * microcontroller, const bool& withParam,
			const int& location, const int& value)
	{
		// Value input
		int locationInput, valueInput;

		// Input string
		std::string input;

		// If memory location and new value are provided, fetch them
		if (withParam)
		{
			locationInput = location;
			valueInput = value;
		}
		else
		{
			// Else, get memory location and new value from user

			// Prompt user for memory location
			std::cout << "> location? ";
			getline(std::cin, input);

			// If input string is empty or contains space(s) or of wrong type,
			// set negative (invalid) location
			if (!(input.length() && input.find(" ") == std::string::npos
					&& isValidHex(input)))
			{
				locationInput = -1;
			}
			else
			{
				// Else, get validated parameter(s)

				// Create input string stream and parse value
				std::istringstream stream(input);
				stream >> std::hex >> std::setw(10)
					   >> locationInput;
			}
		}

		// If location is valid, check for new value
		if (locationInput >= 0
				&& locationInput < microcontroller->getMemorySize())
		{
			// If new value is not provided, get new value from user
			if (!withParam)
			{
				// Display old value
				std::cout << "Old value: 0x"
						  << std::hex << std::setw(2) << std::setfill('0')
						  << (int) microcontroller->look(locationInput)
						  << std::endl;

				// Prompt user for new value
				std::cout << "new? ";
				getline(std::cin, input);

				// If input string is empty or contains spaces(s) or of wrong
				// type, set negative (invalid) value
				if (!(input.length() && input.find(" ") == std::string::npos
						&& isValidHex(input)))
				{
					valueInput = -1;
				}
				else
				{
					// Else, get validated parameter(s)

					// Create input string stream and parse value
					std::istringstream stream(input);
					stream >> std::hex >> std::setw(10)
						   >> valueInput;
				}
			}

			// If value is valid, modify memory content
			if (valueInput >= 0)
			{
				microcontroller->modify(locationInput,
						(unsigned char) (valueInput & 0xFF));
			}
			else
			{
				// Else, display error message
				std::cerr << "Invalid hex value" << std::endl;
			}
		}
		else
		{
			// Else, display error message
			std::cerr << "Invalid address" << std::endl;
		}
	}

	// Reset microcontroller
	void reset (Microcontroller * microcontroller)
	{
		// Re-initialize microcontroller
		microcontroller->initialize();

		// Display informational message
		std::cout << "Microcontroller reset" << std::endl;
	}

	// Display PC and registers
	void status (const Microcontroller * microcontroller)
	{
		std::cout << microcontroller->statusString();
	}
}



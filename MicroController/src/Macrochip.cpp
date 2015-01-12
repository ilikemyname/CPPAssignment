/*
 * Macrochip.cpp
 *
 *  Created on: Jan 11, 2012
 *      Author: huy
 */

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <sstream>
#include "Macrochip.h"

namespace MicrocontrollerEmulation
{
	// Initialize initial PC, memory size and W register value
	const int Macrochip::PC = 0, Macrochip::MEM_SIZE = 1536,
		  Macrochip::VIDEO_MEM_SIZE = 1000, Macrochip::VIDEO_WIDTH = 40,
		  Macrochip::VIDEO_HEIGHT = 25;
	const unsigned char Macrochip::W = 0;

	// Display content of video memory
	void Macrochip::displayScreen () const
	{
		std::cout << "Output Screen:\n"
				  << std::setw(44) << std::setfill('-') << '-' << std::endl;
		for (int i = 0; i < 25; i++)
		{
			std::cout << "| ";
			for (int j = 0; j < 40; j++)
			{
				// Get current character
				unsigned char character = look(i * 40 + j);

				// Display it
				if (character >= 0x20)
				{
					std::cout << std::setw(1) << std::setfill(' ')
							  << character;
				}
				else
				{
					std::cout << ' ';
				}
			}
			std::cout << " |" << std::endl;
		}
		std::cout << std::setw(44) << std::setfill('-') << '-' << std::endl;
	}

	// Reset microcontroller to initial state
	void Macrochip::initialize ()
	{
		// Reset PC and register W to initial value
		setPC(PC);
		registerW = W;

		// If memory is not allocated, do allocation
		if (!getMemory())
		{
			setMemory(new unsigned char[MEM_SIZE]());
		}
		else
		{
			// Else, re-initialize memory to 0
			std::fill_n(getMemory(), MEM_SIZE, 0);
		}
	}

	// Execute from current PC or from a specific location
	const int Macrochip::execute (const int& location)
	{
		// If location is provided, set pc to that
		if (location != -1)
		{
			setPC(location);
		}

		// Execute program until halt opcode found
		while (look(getPC()) != 0xFF)
		{
			// Get current PC and opcode
			int pc = getPC();
			unsigned char opcode = look(getPC());

			// If PC go outside memory, return SIGWEED opcode
			if (pc >= MEM_SIZE)
			{
				return Microcontroller::SIGWEED;
			}

			// Temporary value and memory address
			int address;
			unsigned char value;

			// Fetch, Decode and Execute instruction
			switch (opcode)
			{
				case 0x50:
					// Move value to W

					// Set W to specified value
					registerW = look(pc + 1);

					// Update PC
					setPC(pc + 2);
					break;
				case 0x51:
					// Move W to memory

					// Get target memory location
					address = ((int) look(pc + 1) << 8) | look(pc + 2);

					// Modify memory content
					modify(address, registerW);

					// Update PC
					setPC(pc + 3);
					break;
				case 0x5A:
					// Add value to W

					// Get and add value to W
					registerW += look(pc + 1);

					// Update PC
					setPC(pc + 2);
					break;
				case 0x5B:
					// Subtract value from W

					// Get and subtract value from W
					registerW -= look(pc + 1);

					// Update PC
					setPC(pc + 2);
					break;
				case 0x6E:
					// Go to address

					// Get target memory location
					address = ((int) look(pc + 1) << 8) | look(pc + 2);

					// Update PC
					setPC(address);
					break;
				case 0x70:
					// Brach if not equal

					// Get comparison value
					value = look(pc + 1);

					// Get target memory location
					address = ((int) look(pc + 2) << 8) | look(pc + 3);

					// If comparison value is the same as W, branch
					if (value == registerW)
					{
						setPC(address);
					}
					else
					{
						// Else, ignore
						setPC(pc + 4);
					}
				default:
					// If invalid opcode found, return SIGOP signal
					return Microcontroller::SIGOP;
					break;
			}
		}

		// If halt opcode catch, return HALT signal
		return Microcontroller::HALT;
	}

	// Look at a specific memory location
	const unsigned char Macrochip::look (const int& location) const
	{
		// If location input is outside memory, return 0
		if (location < 0 || location >= MEM_SIZE)
		{
			return 0;
		}

		// Else, return memory content
		return getMemory()[location];
	}

	// Modify a specific memory location
	void Macrochip::modify (const int& location, const unsigned char& value)
	{
		// If location input is valid, modify memory content
		if (location >= 0 && location < MEM_SIZE)
		{
			getMemory()[location] = value;

			// If data is written on video memory, display screen
			if (location < VIDEO_MEM_SIZE)
			{
				displayScreen();
			}
		}
	}

	// Return PC and registers
	const std::string Macrochip::statusString () const
	{
		// Create output string stream
		std::ostringstream stream;

		// Add status to string stream
		stream << "Status:\n"
			   << " - PC: 0x"
			   << std::hex << std::setw(3) << std::setfill('0')
			   << getPC() << '\n'
			   << " - Register W: 0x"
			   << std::hex << std::setw(2) << std::setfill('0')
			   << (int) registerW << std::endl;

		// Return status string
		return stream.str();
	}

	// Get current state
	const std::string Macrochip::getState () const
	{
		// Create output string stream
		std::ostringstream sstream;

		// Add PC and register(s) to output
		sstream << "PC=" << getPC()
				<< "\nW=" << (int) registerW
				<< std::endl;

		// Loop through memory to save non-zero values
		for (int i = 0; i < MEM_SIZE; i++)
		{
			// Get current value
			unsigned char value = look(i);

			// If value is non-zero, save to output
			if (value)
			{
				sstream << i << '=' << (int) value << std::endl;
			}
		}

		// Return state string
		return sstream.str();
	}

	// Set state from stream
	const int Macrochip::setState (std::istream& stream)
	{
		// Line index
		int index = 0;

		// Line string
		std::string line;

		// Temporary location and value
		int location, value;

		// Fetch each line until EOF reached
		while (getline(stream, line))
		{
			// Increment line index by 1
			index++;

			// Create input stream
			std::istringstream sstream(line);

			// Modify PC state
			if (line.find("PC") != std::string::npos)
			{
				// Ignore 'PC='
				sstream.ignore(3);

				// If location cannot be retrieved, return failure (non-zero)
				if (!(sstream >> location))
				{
					return index;
				}

				// Modify PC
				setPC(location);
			}
			else if (line.find("W") != std::string::npos)
			{
				// Modify register W state

				// Ignore 'W='
				sstream.ignore(2);

				// If value cannot be retrieved, return failure (non-zero)
				if (!(sstream >> value))
				{
					return index;
				}

				// Modify register W
				registerW = value;
			}
			else
			{
				// Else, modify memory content

				// If location cannot be retrieved, return failure (non-zero)
				if (!(sstream >> location))
				{
					return index;
				}

				// Ignore '='
				sstream.ignore(1);

				// If value cannot be retrieved, return failure (non-zero)
				if (!(sstream >> value))
				{
					return index;
				}

				// Modify memory content
				modify(location, value);
			}
		}

		// If no error occurs, return success (0)
		return 0;
	}
}



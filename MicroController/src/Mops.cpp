/*
 * Mops.cpp
 *
 *  Created on: Jan 11, 2012
 *      Author: huy
 */

#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "Mops.h"

namespace MicrocontrollerEmulation
{
	// Initialize initial PC and memory size value
	const int Mops::PC = 0, Mops::MEM_SIZE = 1024;

	// Reset microcontroller to initial state
	void Mops::initialize ()
	{
		// Reset PC to initial value
		setPC(PC);

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
	const int Mops::execute (const int& location)
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
				case 0x0A:
					// Add value to memory

					// Get target memory location
					address = ((int) look(pc + 2) << 8) | look(pc + 3);

					// Get and add value to target
					value = look(address) + look(pc + 1);

					// Modify memory content
					modify(address, value);

					// Update PC
					setPC(pc + 4);
					break;
				case 0x13:
					// Subtract value from memory

					// Get target memory location
					address = ((int) look(pc + 2) << 8) | look(pc + 3);

					// Get and add value to target
					value = look(address) - look(pc + 1);

					// Modify memory content
					modify(address, value);

					// Update PC
					setPC(pc + 4);
					break;
				case 0x16:
					// Go to address

					// Get target memory location
					address = ((int) look(pc + 1) << 8) | look(pc + 2);

					// Update PC
					setPC(address);
					break;
				case 0x17:
					// Branch relative

					// Get offset value
					value = look(pc + 1);

					// Update PC
					setPC(pc + (int)((char) value));
					break;
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
	const unsigned char Mops::look (const int& location) const
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
	void Mops::modify (const int& location, const unsigned char& value)
	{
		// If location input is valid, modify memory content
		if (location >= 0 && location < MEM_SIZE)
		{
			getMemory()[location] = value;
		}
	}

	// Return PC and registers
	const std::string Mops::statusString () const
	{
		// Create output string stream
		std::ostringstream stream;

		// Add status to string stream
		stream << "Status:\n"
			   << " - PC: 0x"
			   << std::hex << std::setw(3) << std::setfill('0')
			   << getPC() << std::endl;

		// Return status string
		return stream.str();
	}

	// Get current state
	const std::string Mops::getState () const
	{
		// Create output string stream
		std::ostringstream sstream;

		// Add PC to output
		sstream << "PC=" << getPC() << std::endl;

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
	const int Mops::setState (std::istream& stream)
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



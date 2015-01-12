/*
 * Macrochip.h
 *
 *  Created on: Jan 11, 2012
 *      Author: huy
 */

#ifndef SRC_MACROCHIP_H_
#define SRC_MACROCHIP_H_

#include <string>
#include <iostream>
#include "Microcontroller.h"

namespace MicrocontrollerEmulation
{
	class Macrochip : public Microcontroller
	{
	private:
		static const int PC, MEM_SIZE, VIDEO_MEM_SIZE, VIDEO_WIDTH, VIDEO_HEIGHT;	// Initial PC, memory size, video memory size, video width and video height
		static const unsigned char W;	// Initial value of register W
		unsigned char registerW;	// Special purpose register W

	public:
		Macrochip(const std::string& type) : Microcontroller(type) {}	// Constructor with type

	private:
		void displayScreen() const;	// Display content of video memory

	public:
		const int getMemorySize() const { return MEM_SIZE; }	// Get size of memory
		void initialize();	// Reset microcontroller to initial state
		const int execute(const int& location = -1);	// Execute from current PC or from a specific location
		const unsigned char look(const int& location) const;	// Look at a specific memory location
		void modify(const int& location, const unsigned char& value);	// Modify a specific memory location
		const std::string statusString() const;	// Return PC and registers
		const std::string getState() const;	// Get current state
		const int setState(std::istream& stream);	// Set state from stream
	};
}



#endif /* SRC_MACROCHIP_H_ */

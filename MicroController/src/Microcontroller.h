/*
 * MicroController.h
 *
 *  Created on: Jan 11, 2012
 *      Author: huy
 */

#ifndef SRC_MICROCONTROLLER_H_
#define SRC_MICROCONTROLLER_H_

#include <string>
#include <iostream>

namespace MicrocontrollerEmulation {

class Microcontroller {

private:
	int pc;	// Program Counter (PC)
	unsigned char * memory;	// Memory pointer
	std::string type;	// Microcontroller type

public:
	enum {
		SUCCESS, SIGWEED, SIGOP, HALT
	};	// Execution signals

public:
	Microcontroller(const std::string& typeInput) :
			memory(NULL), type(typeInput) {
	}	// Constructor with type name
	~Microcontroller();	// Destructor

protected:
	void setPC(const int& location) {
		pc = location;
	}	// Set PC value
	void setMemory(unsigned char * pointer) {
		memory = pointer;
	}	// Set memory pointer
	unsigned char * getMemory() const {
		return memory;
	}	// Get memory pointer
public:
	const int getPC() const {
		return pc;
	}	// Get Program Counter
	const std::string& getType() const {
		return type;
	}	// Get microcontroller type

	// Get size of memory
	virtual const int getMemorySize() const = 0;
	// Reset microcontroller to initial state
	virtual void initialize() = 0;
	// Execute from current PC or from a specific location
	virtual const int execute(const int& location = -1) = 0;
	// Look at a specific memory location
	virtual const unsigned char look(const int& location) const = 0;
	// Modify a specify memory location
	virtual void modify(const int& location, const unsigned char& value) = 0;
	// Return PC and registers
	virtual const std::string statusString() const = 0;
	// Get current state
	virtual const std::string getState() const = 0;
	// Set state from stream
	virtual const int setState(std::istream& stream) = 0;
};
}

#endif /* SRC_MICROCONTROLLER_H_ */

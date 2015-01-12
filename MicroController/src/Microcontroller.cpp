/*
 * MicroController.cpp
 *
 *  Created on: Jan 11, 2012
 *      Author: huy
 */

#include "Microcontroller.h"

namespace MicrocontrollerEmulation
{
	// Destructor
	Microcontroller::~Microcontroller () {
		// Delete memory array
		delete[] memory;
	}
}



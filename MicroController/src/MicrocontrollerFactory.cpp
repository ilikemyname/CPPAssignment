/*
 * MicrocontrollerFactory.cpp
 *
 *  Created on: Jan 11, 2012
 *      Author: huy
 */

#include <string>
#include "MicrocontrollerFactory.h"
#include "Mops.h"

/* RULES FOR NEW MICROCONTROLLER PLUG-INS:
   - New microcontroller classes must extend "Microcontroller" base class
     and implement all abstract methods in that class.
   - Memory size Get method must be public.
   - There must be a constructor with parameter "type" (in string format)
     that call the corresponding parameterized base class constructor.
*/

namespace MicrocontrollerEmulation
{
	// Types of microcontroller
	// Please append type names here after adding new plug-ins
	const std::string MicrocontrollerFactory::TYPES[] = {"R500", "PIC32F42", "34HC22"};

	// Get number of microcontroller types
	const int MicrocontrollerFactory::numberOfTypes ()
	{
		return sizeof(TYPES) / sizeof(std::string);
	}

	// Create microcontroller of specified type
	// Add check conditions for pluged-in types here
	Microcontroller * MicrocontrollerFactory::createMicrocontroller (const std::string& type) const
	{
		// Check for type R500 (Mops)
		if (type == TYPES[0])
		{
			return new Mops(type);
		}

		// Check for type PIC32F42 (Macrochip)
		if (type == TYPES[1])
		{
			return new Macrochip(type);
		}

		// Check for type 34HC22 (Rotamola)
		if (type == TYPES[2])
		{
			return NULL;
			//return new Rotamola(type);
		}

		return NULL;
	}
}



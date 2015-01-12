/*
 * MicrocontrollerFactory.h
 *
 *  Created on: Jan 11, 2012
 *      Author: huy
 */

#ifndef SRC_MICROCONTROLLERFACTORY_H_
#define SRC_MICROCONTROLLERFACTORY_H_

#include <string>
#include <map>
#include "Microcontroller.h"

namespace MicrocontrollerEmulation
{
	class MicrocontrollerFactory
	{
	public:
		static const std::string TYPES[];	// Types of microcontroller

	public:
		// Get number of microcontroller types
		static const int numberOfTypes();
		// Create microcontroller of specified type
		Microcontroller * createMicrocontroller(const std::string& type) const;
	};
}



#endif /* SRC_MICROCONTROLLERFACTORY_H_ */

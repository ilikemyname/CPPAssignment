# CPPAssignment
Collection of assignments from course c++

1. Description:
The program emulates 3 microchips which allow user to modify memery using list of commands.

2. Instruction:
The project is implemented with the support of Eclipse IDE. You can import the project to Eclipse. Build and Run.

3. Files:
The program contains these main files:

    main.cpp: Start-up code. It executes the main loop of the program: get user command, call corresponding function and get user command again.
    utility.cpp and utility.h: Utility functions. It contains facade function for microcontroller processing, and other utility functions, such as: get command, check for valid input and convert string.
    Microcontroller.cpp and Microcontroller.h: Base (abstract) class of microcontroller. It declares and defines common member data and methods of a microcontroller.
    MicrocontrollerFactory.cpp and MicrocontrollerFactory.h: Microcontroller producer. It serves as a factory that create specific microcontrollers based on their types. It is also the center for maintaining plug-ins through type definition and instantiating selection.
    Other *.cpp and *.h files: Plug-ins. They extend base microcontroller class and represent additional microcontroller type.

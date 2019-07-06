# flying-stickman
A super mario clone created as part of a multi-stage project for an object-oriented design course.

## Description  
This project was written in C++ using the Qt framework. As such, it can be compiled using Qt Creator 5.11+.
It may be necessary to alter the main.cpp to specify where the config file is on your machine. However, I've added a dialog to pick the config if the default path does not exist.

This is the third stage of a three-stage project for the course Objected Oriented Design at the University of Sydney.
For the last two stages, we were given someone else's code and had to build upon it to complete that stage. To see the behaviour of the previous two stages, the stage can be set in the config file.

## Design Patterns
For this stage, the Observer/Subject pattern was used for communication between objects. The Flyweight pattern in order to facilitate efficient use of memory.

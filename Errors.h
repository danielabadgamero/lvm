#ifndef ERRORS_H
#define ERRORS_H

#define ERROR(msg) { std::cout << "Error: " << msg << '\n'; return; }

#define E1 ERROR("No command provided.")
#define E2 ERROR("Command not found.")

#endif
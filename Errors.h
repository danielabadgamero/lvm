#ifndef ERRORS_H
#define ERRORS_H

#define ERROR(msg) { std::cout << "Error: " << msg << '\n'; return; }

#define E1 ERROR("No command provided.")
#define E2(cmd) ERROR("\"" + cmd + "\""": Command not found.")
#define E3(arg) ERROR("Missing \"" + arg + "\" argument.");
#define E4(arg) ERROR("Too many arguments: \"" + arg + "\"");
#define E5(arg) ERROR("\"" + arg + "\""": Directory not found.")
#define E6(arg) ERROR("\"" + arg + "\""": File doesn't exist.")

#endif
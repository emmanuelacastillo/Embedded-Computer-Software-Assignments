#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <inttypes.h>

/*

Prompts the user to enter a 4 digit number

msg is a C style null terminated string for prompt.

eg. usage

int password;
password=InputNumber("Enter Password");

*/
uint16_t InputNumber(char *msg);

#endif

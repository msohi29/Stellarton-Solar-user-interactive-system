#pragma once

#include <stdio.h>
#include <stdlib.h>

// Creating date data type /
typedef struct date
{
	unsigned int day;
	unsigned int month;
	unsigned int year;
}DATE;
// Macro to determine if the year is a leap year /
#define LYEAR(x) ((x)%400 || ((x)%100 != 0 && (x)%4 ==0))

#define STRYEAR 1900 //Starting year in refrence to the program
#define MXDAY 366 //Maximum number of days in a year (365 + null)

// Declaring global variables /
DATE newdate;
DATE Reminder;
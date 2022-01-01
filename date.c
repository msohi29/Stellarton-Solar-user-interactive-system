#define _CRT_SECURE_NO_WARNINGS

#include "date.h"
// 2 Dimentional array to encompass leap and non - leap years /
unsigned int years[2][12] = {
{31, 28, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31},
{31, 29, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31} };


void Enter_Date()
{
	// This function works on reading year, monthand date from the command prompt /
		int flag = 1;

	while (flag) {
		printf("Enter date (dd/mm/yyyy): ");
		scanf("%d/%d/%d", &newdate.day, &newdate.month, &newdate.year);
		//if (Check_Date(newdate.year, newdate.month, newdate.day) != 1) flag = 0; //break the loop if return is a valid date
		flag = 0;
	}
}

void Reminder_Date(DATE y) {
	//This functions determines the date in which the reminder is to be sent
	int x;
	x = LYEAR(y.year);
	Reminder.day = y.day + 30; // increment day by 30
	Reminder.month = y.month;
	Reminder.year = y.year;
	if (Reminder.day <= years[x][y.month - 1]) { //Normal addition without the addition of a month
		printf("Payment reminder will be sent on >%d/%d/%d", Reminder.day, Reminder.month, Reminder.year);
	}
	else { //increment month by 1
		Reminder.day = Reminder.day - years[x][y.month - 1];
		if (y.month == 12) { // in the case it is december the year is increased by 1 then the month is set to 1
			Reminder.year = y.year + 1;
			Reminder.month = 1;
			printf("Payment reminder will be sent on >%d/%d/%d", Reminder.day, Reminder.month, Reminder.year);
		}
		else {
			Reminder.month = y.month + 1; // only increment month by 1
			printf("Payment reminder will be sent on >%d/%d/%d", Reminder.day, Reminder.month, Reminder.year);
		}
	}
}
int Check_Date(unsigned int year, unsigned int month, unsigned int day) {
	//This function checks the validity of the date entered from Enter_Date, 
	//returing a true if an error is cought and false if there are no issues with the date
	int date;
	int x;

	x = LYEAR(year);

	if (year < STRYEAR) { //Condition: the entered year is less than program starting year
		printf("\nThe year you entered >%4d< is less than the Starting year >%4d<\n", year, STRYEAR);
		return 1;
	}
	if (month > 12 || month < 0) { //Condition: the entered month is greater than the 12 month in a year
		printf("\nThe month you have entered >%2d< doesn't exist in the calander\n", month);
		return 1;
	}
	if (day > years[x][month - 1]) { //Condition: day out of bound of given month
		printf("\nThe date entered >%d< does not match the number of days in the month >%d<, days are >%d<\n", day, month,
			years[x][month - 1]);
		return 1;
	}
}

unsigned month_base[2][12] = {
{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334},
{0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335} };


//This function is  credited to: Dr. Larry Hughes
long encode(DATE y)
{

	return (y.day - 1) +
		month_base[LYEAR(y.year)][y.month - 1] +
		(y.year - STRYEAR) * MXDAY;
}
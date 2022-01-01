/*
*												reorder.c
* This file contains the following functions:
* 1.) open_file_reorder
* 2.) find_reorders
*/

#define _CRT_SECURE_NO_WARNINGS

#include "Task7.h"

#define MAX_ENCODED 6
#define MAX_NAME 17

void open_file_reorder()
{
	//This functions generates a ORDER txt file with the current date encoded in the name.

	//First we define a local string of length that will accomodate the name with encoded date and '.txt'
	//in it
	char fname[MAX_NAME] = "ORDER";

	char date[MAX_ENCODED];

	//converts the current date (encoded) into a string and stores it in the passed variable
	snprintf(date, MAX_ENCODED, "%ld", encode(newdate));

	//Concates the date string into the file name
	strncat(fname, date, MAX_ENCODED + 1);

	//concates ".txt" into the file name
	strncat(fname, ".txt", 5);

	//create the file with name fname.
	reorder = fopen(fname, "a+");
}

int find_reorders(FILE* pfd, FILE*sfd)
{
	HEADER_P hrec_p;
	HEADER_S hrec_s;

	//Retrive the header from the backorder file
	fseek(pfd, 0, SEEK_SET);
	fread(&hrec_p, sizeof(HEADER_P), 1, pfd);

	//storing the available_index 
	int stop = hrec_p.available_index;

	int i = 1;
	int j = 1;
	int cont = 1;

	fseek(sfd, 0, SEEK_SET);
	fread(&hrec_s, sizeof(HEADER_S), 1, sfd);

	while ((i != stop))
	{
		fseek(pfile, i * sizeof(PFREC), SEEK_SET);
		fread(&newproduct, sizeof(PFREC), 1, pfd);

		if (newproduct.stock <= newproduct.restock || newproduct.stock == 0)
		{
			fprintf(reorder, "Date: >%d/%d/%d<\nProduct ID: >%d<\nClassification: >%s<\n"
				"manufacturer code: >%s<\nin stock: >%d<\nmanufacturer: >%s<\n", newdate.day, newdate.month, newdate.year
				, newproduct.pid, newproduct.classification, newproduct.manufacturer_code, newproduct.stock, newproduct.manufacturer);	

			while (((unsigned)j < (hrec_s.available_index - START)) && (cont != 0))
			{
				fseek(sfd, (j * sizeof(SFREC)), SEEK_SET);
				fread(&newsupplier, sizeof(SFREC), 1, sfile);

				switch (strcmp(newproduct.manufacturer, newsupplier.manufacturer))
				{
				case 0:
					fprintf(reorder, "supplier contact: >%s<\ntelephone no. >%s<\nemail: >%s<\nmanufacturer: >%s<\n\n"
							, newsupplier.Contact, newsupplier.TelephoneNum, newsupplier.Email, newsupplier.manufacturer);
					cont = 0;
					break;
				}
				j++;
			}
			cont = 1;
			j = 1;
		}
		i++;
	}
}
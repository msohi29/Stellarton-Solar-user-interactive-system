#pragma once

#include "Task7.h"

//#include<stdio.h>
//#include<stdlib.h>

/*
Supplier: Manufacturer	Contact	Company	Address	Telephone
*/

// Pointer to supplier file /
FILE * sfile;

// Defining length of different fields of SUPPLIER /
#define Manufacturer_len   20
#define Cont_len           50
#define comp_len           50
#define Saddress_len       60
#define TelephoneNum_len   20
#define Email_len          50
#define MAX_FIELDS_SUPPLIERS 6

//defining the possible states of a record.
// 0 - DELETED
// 1 - ACTIVE
typedef enum state_s { DELETED_S, ACTIVE_S}STATUS_S;

/*Making suppliers header data type */
typedef struct header_s
{
	unsigned int available_index;
}HEADER_S;

// Creating supplier data type /
typedef struct suppliers
{
	char manufacturer[Manufacturer_len];
	char Contact[Cont_len];
	char company[comp_len];
	char address[Saddress_len];
	char TelephoneNum[TelephoneNum_len];
	char Email[Email_len];
	STATUS_S status;
	unsigned int sid;
}SUPPLIERS;

// Linking supplier and header /
typedef union supplier_file
{
	struct suppliers srec;
	struct header_s hrec;
}SFREC;

// Macro that expands into strncpy for suppliers /
#define STRCPY_S(x, y) strncpy(supplier_field_ptrs[y], x, suppliers_field_size[y])

// Decleration of array of pointer to fields of suppliers /
char* supplier_field_ptrs[MAX_FIELDS_SUPPLIERS];
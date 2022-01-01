/*
										Customers.h
*/

//#include "Task7.h"

#include<stdio.h>
#include<stdlib.h>

// Defining length of different fields of CUSTOMER /
#define cname_len   20       
#define busi_name   11
#define address_len 50
#define town_len    32
#define prov_len    3
#define pcode_len   8
#define phone_len   13
#define MAX_FIELDS_CUST 7
/*
customer: Name(FN LN), Business Name, Street Address,
		  Town, Province, Postal Code, Telephone
*/

// Pointer to customer file /
FILE* cfile;

//defining the possible states of a record.
// 0 - DELETED
// 1 - ACTIVE
typedef enum state_c { DELETED_C, ACTIVE_C}STATUS_C;

/*Making customer header data type */
typedef struct header_c
{
	unsigned int available_index;
}HEADER_C;

// Creating Customer data type /
typedef struct customer
{
	char cname[cname_len];
	char business_name[busi_name];
	char street_address[address_len];
	char town[town_len];
	char province[prov_len];
	char postal_code[pcode_len];
	char phone_number[phone_len];
	STATUS_C status;
	unsigned int cid;
}CUSTOMER;

// Linking customer and header /
typedef union customer_file
{
	struct customer crec;
	struct header_c hrec;
}CFREC;

// Decleration of array of pointer to fields of customers /
char* customer_fields_ptr[MAX_FIELDS_CUST];

// Macro that expands into strncpy for customers /
#define STRCPY_C(x, y)\
        strncpy(customer_fields_ptr[y], x, customer_fields_size[y])
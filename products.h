#pragma once

#include "Task7.h"
//#include<stdio.h>
//#include<stdlib.h>

/*
product: Name	Classification	Manufacturer	Unit Cost	Manufacturer code	In Stock
*/
// Pointer to products file /
FILE * pfile;

// Defining length of different fields of SUPPLIER /

#define Pname_len   80
#define Class_len   20
#define Manu_len    20    //common
#define Cost_len    50
#define ManuC_len   32
#define stock_len     3
#define DECIMAL_COUNT 2
#define MAX_FIELDS_PRODUCT 7

#define TRUE 1
#define FALSE 0
// Macro to check if the character is a digit /
#define ISDIGIT(x) ('0'<=(x) && (x)<='9')

// Defining global variable for products /
char stocks[stock_len];
char cost[Cost_len];
char restock[stock_len];

//defining the possible states of a record.
// 0 - DELETED
// 1 - ACTIVE
typedef enum state_p { DELETED_P, ACTIVE_P}STATUS_P;

/*Making product header data type */
typedef struct header_p
{
	unsigned int available_index;
}HEADER_P;

// Creating product data type /
typedef struct product
{
	char name[Pname_len];
	char classification[Class_len];
	char manufacturer[Manu_len];
	int unit_cost;
	char manufacturer_code[ManuC_len];
	int stock;
	STATUS_P status;
	int restock;
	unsigned int pid;
}PRODUCT;

// Linking product and header /
typedef union product_file
{
	struct product prec;
	struct header_p hrec;
}PFREC;

// Macro that expands into strncpy for products /
#define STRCPY_P(x, y)\
        strncpy(product_field_ptrs[y], x, product_field_size[y])

// Decleration of array of pointer to fields of products /
char* product_field_ptrs[MAX_FIELDS_PRODUCT];
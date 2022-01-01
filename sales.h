#pragma once

#include<stdio.h>
#include<stdlib.h>
#include "date.h"

#define TRUE  1
#define FALSE 0

#define START 1000

#define MAX_PROD 20

FILE* sales_file;
FILE* due_file;

/*
Sales: cid, 1{products}20, 1{Quantity}20, total_cost, saleid, payment date, reminder date;
*/

typedef struct header_sale
{
	unsigned int available_index;
}HEADER_SALE;

typedef struct sales
{
	unsigned int cid;
	int products[MAX_PROD];
	int quantity[MAX_PROD];
	int total_cost;
	unsigned int saleid;
	DATE purchase_date;
	DATE reminder_date;
}SALE;

typedef union sale_file
{
	struct sales srec;
	struct header_sale hrec;
}SALEREC;

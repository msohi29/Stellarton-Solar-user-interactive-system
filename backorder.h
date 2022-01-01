
#pragma once

#include<stdio.h>
#include<stdlib.h>

#define cname_len 20
#define pname_len 80

/*
*   backorders: backorderid, cid, pid, amount_ordered
*/

typedef enum status_b {FILLED_B, ACTIVE_B} STATUS_B;

typedef struct header_b
{
	unsigned int available_index;
}HEADER_B;

typedef struct backorder
{
	unsigned int backorderid;
	unsigned int saleid;
	unsigned int cid;
	unsigned int pid;
	STATUS_B status;
	int amount_ordered;
}BACKORDER;

typedef union bfrec
{
	HEADER_B hrec;
	BACKORDER brec;
}BFREC;
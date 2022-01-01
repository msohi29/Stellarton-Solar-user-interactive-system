/*
*												Backorder.c
* This file contains the functions:
* 1.) backorders
* 2.) open_file_ordersfilled
* 3.) fill_back_order
*/

#define _CRT_SECURE_NO_WARNINGS

#include "Task7.h"

#define MAX_ENCODED 6
#define MAX_NAME_F 25

void backorders(FILE* salefd, FILE* cfd, FILE* pfd, unsigned int pid, unsigned int cid, unsigned int saleid, int quantity) {
	HEADER_B hrec_b;

	fseek(bfile, 0, SEEK_SET); // set pointer to beginning of backorders.dat
	fread(&hrec_b, sizeof(HEADER_B), 1, bfile); // reads the content of the header to buffer header called hrec_b
	fseek(cfd, (cid - START + 1) * sizeof(CFREC), SEEK_SET); // set pointer to beginning of customers file
	fread(&newcustomer, sizeof(CFREC), 1, cfd); // reads the content of the record with the passed customer id to a buffer customer
	fseek(pfd, pid * sizeof(PFREC), SEEK_SET); // set pointer to beinning of product file
	fread(&newproduct, sizeof(PFREC), 1, pfd); //reads the content of the record with the passed product id to a buffer product
	newbackorder.backorderid = hrec_b.available_index; // assignt the backorder id to the next available header
	newbackorder.cid = cid; // assign customer id to backorder custumer id
	newbackorder.pid = pid; // assign product id to backorder product id
	newbackorder.saleid = saleid; // assign sale id to backorder sale id
	newbackorder.amount_ordered = quantity;

	newbackorder.status = ACTIVE_B;

	//print contents of backorder record
	printf("\nBackorder Issued: %d/%d/%d\nBackorder ID: %u	Sale ID: %u\nCustomer ID: >%u< Customer name: >%s<\n"
		"Product ID: >%u< Product Name: >%s<	Quantity: >%d<\n"
		, newdate.day, newdate.month, newdate.year, newbackorder.backorderid, saleid, newbackorder.cid, newcustomer.cname,
		newproduct.pid, newproduct.name, newbackorder.amount_ordered);

	fseek(bfile, newbackorder.backorderid * sizeof(BFREC), SEEK_SET);
	fwrite(&newbackorder, sizeof(BFREC), 1, bfile);

	hrec_b.available_index++; //increment header
	fseek(bfile, 0, SEEK_SET); // point to sales file header
	fwrite(&hrec_b, sizeof(HEADER_B), 1, bfile); //write new header to backrecords header
}

void open_file_ordersfilled()
{
	//This functions generates a ORDERSFILLED txt file with the current date encoded in the name.

	//First we define a local string of length that will accomodate the name with encoded date and '.txt'
	//in it
	char fname[MAX_NAME_F] = "ORDERSFILLED";

	char date[MAX_ENCODED];

	//converts the current date (encoded) into a string and stores it in the passed variable
	snprintf(date, MAX_ENCODED, "%ld", encode(newdate));

	//Concates the date string into the file name
	strncat(fname, date, MAX_ENCODED + 1);

	//concates ".txt" into the file name
	strncat(fname, ".txt", 5);

	//create the file with name fname.
	ordersfilled = fopen(fname, "a+t");
}

void fill_back_order(FILE* cfd, FILE* pfd, FILE* bfd)
{
	HEADER_B hrec_b;

	//Retrive the Header record from the backorders file
	fseek(bfd, 0, SEEK_SET);
	fread(&hrec_b, sizeof(HEADER_B), 1, bfd);

	//stores the available_index into a local variable
	int max = hrec_b.available_index;

	int bid = 0;
	int quantity_recieved = 0;

	//Get a valid backorder id from the user
	//Here a correct id should be less than the available_index in the backorders file and greater than 0.
	do
	{
		printf("Enter the backorder id to fill: ");
		scanf("%d", &bid);
		getchar();

	} while (bid >= hrec_b.available_index || bid <= 0);
	//exit the loop if bid < hrec_b.available_index && bid > 0

	//Read the backorder record stored at the given ID.
	fseek(bfile, bid * sizeof(BFREC), SEEK_SET);
	fread(&newbackorder, sizeof(BFREC), 1, bfile);

	//Print the details of the backorder to the screen
	printf("Backorder id: >%d<\nProduct ID: >%d< Amount Ordered: >%d<\n", newbackorder.backorderid, newbackorder.pid, newbackorder.amount_ordered);
	
	//check if the backorder has already been filled
	if (newbackorder.status == FILLED_B)
	{
		printf("This order has already been completed\n");
		return;
	}
	//If not then we update the record status to filled and get the amount of items sent back by the supplier
	//Use them to fullfill the backorder and the remaining items go back to the product stock in storage.
	else
	{
		do {
			printf("Enter the amount of items shipped from the supplier: ");
			scanf("%d", &quantity_recieved);
		} while (quantity_recieved <= 0);

		newbackorder.status = FILLED_B;
		
		//Get the details of the customer who placed the backorder
		fseek(cfile, (newbackorder.cid - START + 1) * sizeof(CFREC), SEEK_SET);
		fread(&newcustomer, sizeof(CFREC), 1, cfile);
		
		//Print the details of the customer into the ORDERSFILLED file
		fprintf(ordersfilled, "Customer Name: >%s<	Telephone number: >%s<", newcustomer.cname, newcustomer.phone_number);
		
		//Get the details of the product that the customer places a BACKORDER of
		fseek(pfile, newbackorder.pid * sizeof(PFREC), SEEK_SET);
		fread(&newproduct, sizeof(PFREC), 1, pfile);

		//Print the details of the product that the customer placed a backorder of into the file
		fprintf(ordersfilled, "Product Name: >%s<	Quantity: >%d<\n", newproduct.name, newbackorder.amount_ordered);

		//Reduce the customers ordered amount from the stock received from the supplier
		newproduct.stock = quantity_recieved - newbackorder.amount_ordered;

		printf("Stock in inventory: %d\n", newproduct.stock);

		//store the updated stock into the products file
		fseek(pfd, newbackorder.pid * sizeof(PFREC), SEEK_SET);
		fwrite(&newproduct, sizeof(PFREC), 1, pfd);

		//write the updated backorder record into the file
		fseek(bfd, newbackorder.backorderid * sizeof(BFREC), SEEK_SET);
		fwrite(&newbackorder, sizeof(BFREC), 1, bfd);
	}
}

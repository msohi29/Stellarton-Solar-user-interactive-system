/*
* This c file contains the functions for customers:
*
* The functions in this c file are:
*
*  1.) open_rel_c
*  2.) initialize_customer_fields_ptr
*  3.) print_customer
*  4.) add_customers
*  5.) print_c
*  6.) delete_customer
*  7.) update_customer
*/

#define _CRT_SECURE_NO_WARNINGS  //This allows many fucntions such as strtok and strcpy on visual studio without errors

#include "Task7.h"

//Defining a global variable buffer local to Customers.c
//The size of this buffer is equal to the biggest string. In this case address.

#define INPUT_LEN  60
char input[INPUT_LEN];

CUSTOMER del_customer;

int open_rel_c(FILE** file)
{
	/*
	 This fucntions gets the name of the customer dat file and links it to the passed file pointer
	  -The file is opened in "r+" mode meaning that the file must exist and is opened for read and write
	*/

	char fname[fname_len];
	printf("Enter Customers database file name: ");
	fgets(fname, fname_len, stdin);

	//_set_fmode(_O_BINARY);   //Read and write the file in binary

	//if name of a file that doesn't exist is passed then the outcome is non-zero and prints a diagnostic.
	if ((*file = fopen(fname, "r+b") )!= 0)
	{
		printf("Error opening file >%s<\n", fname);
		getchar();
		return -1;
	}

	//prints a disgnostic of sucessful open.
	printf("Openned file >%s<\n", fname);
}

void initalize_customer_fields_ptr(CUSTOMER* cust_field_adr)
{
	/*
	* This function initializes the array of pointers to the fileds of customers at run time.
	* This array runs in paralled with the prompts and sizes for customers.
	*/
	customer_fields_ptr[0] = (char*)&cust_field_adr->cname;
	customer_fields_ptr[1] = (char*)&cust_field_adr->business_name;
	customer_fields_ptr[2] = (char*)&cust_field_adr->street_address;
	customer_fields_ptr[3] = (char*)&cust_field_adr->town;
	customer_fields_ptr[4] = (char*)&cust_field_adr->province;
	customer_fields_ptr[5] = (char*)&cust_field_adr->postal_code;
	customer_fields_ptr[6] = (char*)&cust_field_adr->phone_number;
}


//This initializes the array of sizes of the fields of customers at compile time.
int customer_fields_size[MAX_FIELDS_CUST] = { cname_len, busi_name, address_len, town_len, prov_len,
										pcode_len, phone_len };

void print_customer(CUSTOMER* cust_field_adr)
{
	/*
	* This function prints the contents of a variable of type CUSTOMER.
	*/
	printf("Customer id: %d\n", cust_field_adr->cid);
	printf("Name: %s\n", cust_field_adr->cname);
	printf("Business: %s\n", cust_field_adr->business_name);
	printf("Street Address: %s\n", cust_field_adr->street_address);
	printf("Town: %s\n", cust_field_adr->town);
	printf("Province: %s\n", cust_field_adr->province);
	printf("Postal Code: %s\n", cust_field_adr->postal_code);
	printf("Phone number: %s\n\n", cust_field_adr->phone_number);
}

//This is an array of strings of prompts to get input related to add customer.
//This array runs in parallel with the array of pointers and sizes for customers.
char* adding_prompts_customers[MAX_FIELDS_CUST] =
{ "Enter customer name: ",
"Enter customer business name: ",
"Enter customer street address: ",
"Enter customer town: ",
"Enter customer province: ",
"Enter customer postal code: ",
"Enter customer phone number: " };

void add_customers(FILE* cfd)
{
	/*
		This function adds a customer into the customer database.
		-The fucntion is based on the following structure for customers:
			customer: customer id, Name(FN LN), Business Name, Street Address, Town, Province, Postal Code, Telephone
	*/
	HEADER_C hrec;
	int cid;

	//Goes to the first record in the customers database file
	//This record is initialized as the header and contains the data for a record index available for writing into.
	fseek(cfd, 0, SEEK_SET);
	fread(&hrec, sizeof(HEADER_C), 1, cfd);

	cid = hrec.available_index;
	printf("New customer id is: %u\n", cid);

	int i = 0;

	//This while loop runs through the table
	//Starts at 0 and Ends at end of the table.
	while (i < MAX_FIELDS_CUST)
	{
		printf("%s", adding_prompts_customers[i]); //prints the prompts for add customers
		fgets(input, INPUT_LEN, stdin);							   //gets the input from user
		STRCPY_C(input, i);						   //This is macro, it expands into a strncpy
		i++;									   //Traverse the table
	}

	newcustomer.status = ACTIVE_C;
	newcustomer.cid = cid;

	//As the customer id starts at 1000,
	//newcustomer.cid - START + 1 translates it into the corresponding index to the id in the file
	fseek(cfd, (newcustomer.cid - START + 1) * sizeof(CFREC), SEEK_SET);

	//copy newcustomer data into file at index = newcustomer.cid - START + 1
	fwrite(&newcustomer, sizeof(CFREC), 1, cfd);

	//increment the available index and update the header in the file
	hrec.available_index++;
	fseek(cfd, 0, SEEK_SET);
	fwrite(&hrec, sizeof(HEADER_C), 1, cfd);
}

void print_c(FILE* file, int cid)
{
	/*
		This function prints a specified customer record (parameter cid) from a sprcified file (parameter file)
	*/

	fseek(file, (cid - START + 1) * sizeof(CFREC), SEEK_SET);
	fread(&newcustomer, sizeof(CUSTOMER), 1, file);
	printf("ID: >%d<\nStatus: %d\nName: >%s<\nBusiness name: >%s<\nStreet addres: >%s<\n"
		"Town name: >%s<\nProvince:>%s<\nPostal Code: >%s<\nPhone Number: >%s<\n\n",
		newcustomer.cid, newcustomer.status, newcustomer.cname, newcustomer.business_name,
		newcustomer.street_address, newcustomer.town, newcustomer.province,
		newcustomer.postal_code, newcustomer.phone_number);
}

void delete_customer(FILE* cfd)
{
	HEADER_C hrec;
	int cid;

	fseek(cfd, 0, SEEK_SET);
	fread(&hrec, sizeof(HEADER_C), 1, cfd);

	do {
		printf("Enter the customer id: ");
		scanf("%d", &cid);
	} while (cid > hrec.available_index || cid < START);

	fseek(cfd, ((cid - START + 1) * sizeof(CFREC)), SEEK_SET);
	fread(&del_customer, sizeof(CFREC), 1, cfd);

	if (del_customer.status == DELETED_C)
	{
		printf("This customer is already delete!!\n");
		return;
	}
	else {
		del_customer.status = DELETED_C;

		fseek(cfd, ((cid - START + 1) * sizeof(CFREC)), SEEK_SET);
		fwrite(&del_customer, sizeof(CFREC), 1, cfd);
		print_c(cfd, cid);
	}

}

//This is an array of strings of prompts to get input related to update customer.
//This array runs in parallel with the array of pointers and sizes for customers.
char* updating_prompts_customers[MAX_FIELDS_CUST] =
{ "Update customer name ",
"Update customer business name ",
"Update customer street address ",
"Update customer town ",
"Update customer province ",
"Update customer postal code ",
"Update customer phone number " };

void update_customer(FILE* cfd)
{
	/*
	This function updates a customer from the customer database.
	-The fucntion is based on the following structure for customers:
		customer: customer id, Name(FN LN), Business Name, Street Address, Town, Province, Postal Code, Telephone
*/
	HEADER_C hrec; //header
	int cid; //record number
	int option = -1; // a holder to store the option entered by the user
	int i = 0;
	int ext = -1; // a holder to store the users choice to update another customer or exit the loop
	fseek(cfd, 0, SEEK_SET); //set pointer to first header
	fread(&hrec, sizeof(HEADER_C), 1, cfd); //read first 


	do {
		do {
			printf("Enter customer record number (enter 0 to exit): ");
			scanf("%d", &cid); //saves the enterd id into the record id
			getchar();
		} while (cid < 1000 || cid >= hrec.available_index); // the do while continues to operate until a valid id is entered

		fseek(cfd, (cid - START + 1) * sizeof(CFREC), SEEK_SET); // points to wanted record
		fread(&newcustomer, sizeof(CFREC), 1, cfd); // reads the content of the record into a buffer called newcustomer

		if (newcustomer.status == ACTIVE_C) { // Checks whether the customer still exists in the database
			printf("Customer found:\n");
			print_customer(&newcustomer); // prints the content of selected customer
			printf("Chose option\n");
			while (i < MAX_FIELDS_CUST) {
				// shows theupdate options for customer
				printf("%d.%s\n", i, updating_prompts_customers[i]);
				i++;
			}
			i = 0; // reset i for next itteration
			printf("7.Exit\n");
			scanf("%d", &option); // Reads selected option from user
			getchar();
			if (option < 7) { // if a field is updated the new fields of customers are printed
				printf("%s:", updating_prompts_customers[option]); //prompt user to update selected option
				fgets(input, INPUT_LEN, stdin); //read change from user
				STRCPY_C(input, option); // update customer field in buffer record
				fseek(cfd, (cid - START + 1) * sizeof(CFREC), SEEK_SET); //point to wanted record
				fwrite(&newcustomer, sizeof(CFREC), 1, cfd); // write new updated fields to desired record
			}
		}
		else {
			printf("Customer >%d< is deleted\n", cid); // if the record is deleted this is statement is printed
		}
		printf("Enter 1. to update more customers\n"); // propt user to either exit or update another customer
		printf("      0. to exit\n");
		printf("Option: ");
		scanf("%d", &ext); // read ext character from user
	} while (option != 7 && ext != 0); //exit while loop if 7 is entered
}
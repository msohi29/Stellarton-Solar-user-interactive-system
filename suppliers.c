/*
											Suppliers.c
*/
#define _CRT_SECURE_NO_WARNINGS
#include "Task7.h"

//defining global variable buffer local to suppliers.c
#define INPUT_LEN 60
char input[INPUT_LEN];

SUPPLIERS del_supplier;

int open_rel_s(FILE** file)
{
	//function gets name of supplier and links it to the passed file pointer
	//file opened in r+ mode meaning that the file has to exist and is opened for reading and writing
	char fname[fname_len];
	printf("Enter suppliers file name: ");
	gets(fname);

	//_set_fmode(_O_BINARY); //read and write file in binary
//if name of a file that doesnt exist is passed the outcome in none-zero and a diagnostic is printed
	if (*file = fopen(fname, "r+b") != 0)
	{
		printf("Error opening file >%s<\n", fname);
		getchar();
		return -1;
	}
	//printing diagnostic of successful open
	printf("Openned file >%s<\n", fname);
}


void init_supplier_fields(SUPPLIERS* supplier_field_address)
{
	/*
* This function initializes the array of pointers to the fileds of suppliers at run time.
* This array runs in paralled with the prompts and sizes for suppliers.
*/

	supplier_field_ptrs[0] = (char*)&supplier_field_address->manufacturer;
	supplier_field_ptrs[1] = (char*)&supplier_field_address->Contact;
	supplier_field_ptrs[2] = (char*)&supplier_field_address->company;
	supplier_field_ptrs[3] = (char*)&supplier_field_address->address;
	supplier_field_ptrs[4] = (char*)&supplier_field_address->TelephoneNum;
	supplier_field_ptrs[5] = (char*)&supplier_field_address->Email;
}

void print_supplier_fields(SUPPLIERS* supplier_field_address) {
	//This function prints the contents of a variable of type SUPPLIER. 

	printf("Manufacturer: %s\n", supplier_field_address->manufacturer);
	printf("Contact: %s\n", supplier_field_address->Contact);
	printf("Company: %s\n", supplier_field_address->company);
	printf("Address: %s\n", supplier_field_address->address);
	printf("Telephone number: %s\n", supplier_field_address->TelephoneNum);
	printf("Email: %s\n", supplier_field_address->Email);
	putchar('\n');
}
//This is an array of strings of prompts to get input related to add supplier.
//This array runs in parallel with the array of pointers and sizes for suppliers.
char* adding_prompts_supplier[MAX_FIELDS_SUPPLIERS] =
{ "Enter Supplier Manufacturer: ",
  "Enter Supplier Contact: ",
  "Enter Supplier Company: ",
  "Enter Supplier Address: ",
  "Enter Supplier Telephone number: ",
  "Enter Supplier Email: ",
};
//This initializes the array of sizes of the fields of suppliers at compile time.
int suppliers_field_size[MAX_FIELDS_SUPPLIERS] = { Manufacturer_len, Cont_len, comp_len, Saddress_len, TelephoneNum_len, Email_len };

void add_Supplier(FILE* spfd)
{
	/*
	This function adds a supplier into the supplier database.
	-The function is based on the following structure for supplier:
		supplier: manufacturer, contact, company, Address, Telephonenumber, email
*/
	HEADER_S hrec;
	int sid;
	//Goes to the first record in the suppliers database file
		//This record is initialized as the header and contains the data for a record index available for writing into.
	fseek(spfd, 0, SEEK_SET);
	fread(&hrec, sizeof(HEADER_S), 1, spfd);

	printf("New supplier id is: %u\n", hrec.available_index);
	sid = hrec.available_index;

	if (fseek(spfd, (sid - START + 1) * sizeof(SFREC), SEEK_SET) < 0)
	{
		printf("Invalid sid\n");
		getchar();
		return;
	}
	int i = 0;
	//This while loop runs through the table
		//Starts at 0 and Ends at end of the table.
	while (i < MAX_FIELDS_SUPPLIERS)
	{
		printf("%s", adding_prompts_supplier[i]);
		gets(input);
		//gets the input from user
		STRCPY_S(input, i);
		//This is macro, it expands into a strncpy
		i++;
		//traverse table
	}

	newsupplier.status = ACTIVE_S;
	newsupplier.sid = sid;
	//As the supplier id starts at 1000,
		//newsupplier.sid - START + 1 translates it into the corresponding index to the id in the file
	fseek(spfd, (newsupplier.sid - START + 1) * sizeof(SFREC), SEEK_SET);

	fwrite(&newsupplier, sizeof(SFREC), 1, spfd);

	hrec.available_index++;
	fseek(spfd, 0, SEEK_SET);
	fwrite(&hrec, sizeof(HEADER_S), 1, spfd);
}

void print_s(FILE* file, int sid)
{
	/*
		This function prints a specified customer record (parameter cid) from a sprcified file (parameter file)
	*/

	fseek(sfile, (sid - START + 1)* sizeof(SFREC), SEEK_SET);
	fread(&newsupplier, sizeof(SFREC), 1, sfile);
	printf("ID: >%d< Status: %d Manufacturer: >%s< Contact: >%s< Company: >%s<"
		"Address: >%s< Telephone: >%s< Email: >%s<\n",
		newsupplier.sid, newsupplier.status, newsupplier.manufacturer, newsupplier.Contact, newsupplier.company, newsupplier.address,
		newsupplier.TelephoneNum, newsupplier.Email);
}

void delete_supplier()
{
	HEADER_S hrec;
	int sid = 0;

	fseek(sfile, 0, SEEK_SET);
	fread(&hrec, sizeof(SFREC), 1, sfile);

	do {
		printf("Enter the supplier id: ");
		scanf("%d", &sid);
	} while (sid > hrec.available_index || sid < START);

	printf("reached\n");

	fseek(sfile, ((sid - START + 1) * sizeof(SFREC)), SEEK_SET);
	fread(&del_supplier, sizeof(SFREC), 1, sfile);

	getchar();
	printf("Supplier name: %s\nSupplier ID: %d\n", del_supplier.manufacturer, del_supplier.sid);
	getchar();

	if (del_supplier.status == DELETED_S)
	{
		printf("This supplier is already delete!!\n");
	}
	else 
	{
		del_supplier.status = DELETED_S;

		fseek(sfile, ((sid - START + 1) * sizeof(SFREC)), SEEK_SET);
		fwrite(&del_supplier, sizeof(SFREC), 1, sfile);
		print_s(sfile, sid);
		printf("end\n");
		getchar();
		return;
	}
}

//This is an array of strings of prompts to get input related to update supplier.
//This array runs in parallel with the array of pointers and sizes for suppliers.
char* updating_prompts_suppliers[MAX_FIELDS_SUPPLIERS] =
{ "Update Supplier Manufacturer: ",
  "Update Supplier Contact: ",
  "Update Supplier Company: ",
  "Update Supplier Address: ",
  "Update Supplier Telephone number: ",
  "Update Supplier Email: ",
};

void update_supplier(FILE* sfd)
{
	/*
	This function updates a customer from the supplier database.
	-The fucntion is based on the following structure for suppliers:
		supplier: manufacturer, contact, company, Address, Telephone number, Email
*/
	HEADER_S hrec; //header
	int sid; //record number
	int option = -1; // a holder to store the option entered by the user
	int ext = -1; // a holder to store the users choice to update another customer or exit the loop
	int i = 0;
	fseek(sfd, 0, SEEK_SET); //set pointer to first header
	fread(&hrec, sizeof(HEADER_S), 1, sfd); //read first 


	do {
		do {
			printf("Enter supplier record number (enter 0 to exit)\n");
			scanf("%d", &sid); //saves the enterd id into the record id
			getchar();

		} while (sid < 1000 || sid >= hrec.available_index); // the do while continues to operate until a valid id is entered

		fseek(sfd, (sid - START + 1) * sizeof(CFREC), SEEK_SET); // points to wanted record
		fread(&newsupplier, sizeof(SFREC), 1, sfd); // reads the content of the record into a buffer called newsupplier

		printf("sid: >%d< manufacturer: >%s<\n", newsupplier.sid, newsupplier.manufacturer);

		if (newsupplier.status == ACTIVE_S) { // Checks whether the supplier still exists in the database
			printf("Supplier found:\n");
			print_supplier_fields(&newsupplier); // prints the content of selected supplier
			printf("Chose option\n");

			while (i < MAX_FIELDS_SUPPLIERS) {
				// shows the update options for supplier
				printf("%d.%s\n", i, updating_prompts_suppliers[i]);
				i++;

			}
			i = 0; // reset i for next itteration

			printf("7.Exit\n");
			scanf("%d", &option); // Reads selected option from user

			getchar();
			if (option < 7) { // if a field is updated the new fields of suppliers are printed
				printf("%s:", updating_prompts_suppliers[option]); //prompt user to update selected option
				gets(input); //read change from user
				STRCPY_S(input, option); // update supplier field in buffer record
				fseek(sfd, (sid - START + 1) * sizeof(CFREC), SEEK_SET); //point to wanted record
				fwrite(&newsupplier, sizeof(CFREC), 1, sfd); // write new updated fields to desired record
			}
		}
		else {
			printf("Customer >%d< is deleted\n", sid); // if the record is deleted this is statement is printed
		}
		printf("Enter 1. to update more suppliers\n");
		printf("      0. to exit\n");
		printf("Option: ");
		scanf("%d", &ext);
	} while (option != 7 && ext != 0); //exit while loop if 7 is entered or ext = 0
}
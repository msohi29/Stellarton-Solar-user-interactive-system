/*
	This c file contains the functions for making a sale.

	List of functions:

	1.) open_rel_sale
	2.) make_sale
	3.) print_sale
*/

#define _CRT_SECURE_NO_WARNINGS

#include "Task7.h"

#define MAX_ENCODED 6
#define MAX_NAME 17

int open_rel_sale(FILE** file)
{
	/*
	 This fucntions gets the name of the sales dat file and links it to the passed file pointer
	  -The file is opened in "r+" mode meaning that the file must exist and is opened for read and write
	*/
	printf("Enter Sales file name: ");
	gets(fname);

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

int make_sale(FILE* salefd, FILE* pfd, FILE* cfd)
{
	/*
	* This function makes a sale based on the following structure:
				Sales: customer id, 1{products}20, 1{Quantity}20, total_cost, saleid, payment date, reminder date
	*/
	HEADER_C hrec_c;

	HEADER_P hrec1;
	PRODUCT purchaseprod;

	HEADER_SALE hrec;

	SALE newsale;

	int saleid;
	int quantity;
	//get the available index to make a sale into from the file header 
	fseek(salefd, 0, SEEK_SET);
	fread(&hrec, sizeof(HEADER_SALE), 1, salefd);

	newsale.saleid = hrec.available_index;
	printf("New sale id is: %u\n", newsale.saleid);

	//get the purchase date from the global variables newdate
	//store it into field purchase_date of newsale
	newsale.purchase_date = newdate;

	//get the reminder date from the global variables reminder
	//store it into field reminder_date of newsale
	newsale.reminder_date = Reminder;

	//get the cid from the keyboard
	printf("Enter the customer id: ");
	scanf("%u", &newsale.cid);

	//get the available_index from customer file
	fseek(cfd, 0, SEEK_SET);
	fread(&hrec_c, sizeof(HEADER_C), 1, cfd);

	//The available index for customers is used to check if the enter cid is invalid or not
	//because the available_index marks the spot in the customer file for a empty record
	//thus is invalid for existing customers
	//We also check if the enter customer id is less than 1000, as the valid customer id start at 1000

	if (newsale.cid >= hrec_c.available_index || newsale.cid < START)
	{
		printf("Invalid Customer ID. If you want to add a new customer please chose option 1.\n");
		getchar();
		return -1;
	}
	fseek(cfd, (newsale.cid - START + 1) * sizeof(CFREC), SEEK_SET);
	fread(&newcustomer, sizeof(CFREC), 1, cfd);

	if (newcustomer.status == DELETED_C)
	{
		printf("This customer record is no longer active.\n");
		printf("Please make a new account to make a purchase\n");
		return -1;
	}

	int option = -1, i = 0;

	//initalize total_cost
	newsale.total_cost = 0;

	//Loop runs until option is set to done
	//or i is greater than maximum number of products

	while (option != Done && i < MAX_PROD)
	{
		printf("Enter the product number and quantity: ");
		scanf("%d %d", &newsale.products[i], &newsale.quantity[i]);

		//check if valid quantity is entered. As a sale would be considered if a product is bought (i.e. quantity > 0)
		while (newsale.quantity[i] <= 0)
		{
			printf("Enter a valid quantity. Quantity: ");
			scanf("%d", &newsale.quantity[i]);
		}

		//get the available index for product from database header for products
		fseek(pfd, 0, SEEK_SET);
		fread(&hrec1, sizeof(HEADER_P), 1, pfd);

		//The available index for products is used to check if the enter pid is invalid or not
		//because the available_index marks the spot in the product file for a empty record
		//thus the product is invalid if the record is empty
		//We also check if the enter product id is greater than 0, as the valid product id start at 1

		while (newsale.products[i] >= hrec1.available_index || newsale.products[i] <= 0)
		{
			printf("This product does not exist. Please enter a valid id and quantity: ");
			scanf("%d %d", &newsale.products[i], &newsale.quantity[i]);
		}
		//Now we have a product that exists in the database

		//We extract the data field of that product
		fseek(pfd, newsale.products[i] * sizeof(PFREC), SEEK_SET);
		fread(&purchaseprod, sizeof(PFREC), 1, pfd);

		//If their is no stock for that product
		//We make a backorder for the customer and fill the order later.
		if (purchaseprod.stock <= 0)
		{
			printf("The product is out of stock. A backorder will be issued for your convenience.\n");
			quantity = newsale.quantity[i];
			backorders(salefd, cfd, pfd, purchaseprod.pid, newsale.cid, newsale.saleid, quantity);
		}
		//If the asked quantity is more than the available stock, we give the user two chocies:
		// 1. to Place a back order and get the amount in stock.
		// 2. Purchase the available stock only
		else if (newsale.quantity[i] > purchaseprod.stock && purchaseprod.stock != 0)
		{
			int choice = 0;

			printf("The amount asked is not available. Available stock is: >%d<\n", purchaseprod.stock);
			printf("Please select a choice:\n");
			printf("1. Place a back order and get the amount in stock >%d<\n", purchaseprod.stock);
			printf("2. Purchase the available stock only\n");
			scanf("%d", &choice);

			switch (choice)
			{
			case 1:
				quantity = newsale.quantity[i] - purchaseprod.stock;
				backorders(salefd, cfd, pfd, purchaseprod.pid, newsale.cid, newsale.saleid, quantity);
				purchaseprod.stock = 0;
				break;

			case 2:
				newsale.quantity[i] = purchaseprod.stock;
				break;
			}
		}
		//If the asked quantity is less than the avaiable stock, we simply reduce the asked quantity from the
		//available stock.
		else {
			purchaseprod.stock = purchaseprod.stock - newsale.quantity[i];
		}

		//update the stock of the product we purchased in the product database
		fseek(pfd, newsale.products[i] * sizeof(PFREC), SEEK_SET);
		fwrite(&purchaseprod, sizeof(PFREC), 1, pfd);

		//calculate the total cost (i.e total amount payable)
		newsale.total_cost += (purchaseprod.unit_cost * newsale.quantity[i]);

		//check if user wants to add more products into the array of product id's
		printf("Enter 1. to add more products or 0. to exit: ");
		scanf("%d", &option);
		i++;
	}

	//Inform the user that they have added the maximum number of distinct products.
	if (i == MAX_PROD)
	{
		printf("Maxium products added in this sale. Please create a new sale to"
			"add more products.\n");
		getchar();
		getchar();
	}
	else
		//in this case we have spaces in the array of products which have been left undefined
		//so we set them to zero. This also helps reduce the prints later in print_sale fucntion.
	{
		for (int z = i; z < MAX_PROD; z++)
		{
			newsale.products[z] = 0;
			newsale.quantity[z] = 0;
		}
	}

	//write the newsale into the sale database.
	fseek(salefd, newsale.saleid * sizeof(SALEREC), SEEK_SET);
	fwrite(&newsale, sizeof(SALE), 1, salefd);

	//update the next available index in the header and 
	//write the header into the sales database 
	hrec.available_index++;
	fseek(salefd, 0, SEEK_SET);
	fwrite(&hrec, sizeof(HEADER_SALE), 1, salefd);

	saleid = hrec.available_index - 1;

	//call the print_sale function.
	print_sale(sales_file, cfile, pfile, saleid);
	//print_sale(sales_file, cfile, pfile);
	return 1;
}

void print_sale(FILE* salefd, FILE* cfd, FILE* pfd, unsigned int id)
//void print_sale(FILE* salefd, FILE* cfd, FILE* pfd)
{
	/*
	*	This fucntion displays the data stored in a sale as well as the data related to the products purchased and
	*   the customer who bought it.
	*/
	//clears the screen to improve visibility
	#ifdef _WIN64
		system("cls");			//Clears the prompts from the enter_date function

	#elif __APPLE__
		#include<TargetConditionals.h>
		
		#ifdef TARGET_OS_MAC
			system("clear");
		#endif
	#elif __unix__
		system("clear");
	#endif
	//SALE newsale;
	HEADER_SALE hrec;

	CUSTOMER cust;

	PRODUCT prod;

	unsigned int saleid;

	//we get the available index from the sales header 
	fseek(salefd, 0, SEEK_SET);
	fread(&hrec, sizeof(hrec), 1, salefd);

	//we decrement the available index by 1, this gives us the recent sale we want to print
	//newsale.saleid = hrec.available_index - 1;

	printf("ID: %u\n", id);

	saleid = id;

	printf("\nSale id: >%u<\n\n", saleid);

	//get sale data fields
	fseek(salefd, saleid * sizeof(SALEREC), SEEK_SET);
	fread(&newsale, sizeof(SALE), 1, salefd);

	//print the purchase date and reminder data for payment
	printf("Purchase date (mm/dd/yyyy): %d/%d/%d\n", newsale.purchase_date.month, newsale.purchase_date.day
		, newsale.purchase_date.year);

	printf("Payment due date date (mm/dd/yyyy): %d/%d/%d\n", newsale.reminder_date.month, newsale.reminder_date.day
		, newsale.reminder_date.year);

	printf("\ncid: >%u<\n", newsale.cid);

	//get data for customer fields
	fseek(cfd, (unsigned)((unsigned)newsale.cid - START + 1) * sizeof(CFREC), SEEK_SET);
	fread(&cust, sizeof(CUSTOMER), 1, cfd);

	//output relevant customer data 
	printf("Customer name: >%s<\n", cust.cname);
	printf("Customer phone number: >%s<\n", cust.phone_number);
	printf("Customer Delivery address: >%s, %s, %s, %s<\n", cust.street_address, cust.postal_code,
		cust.town, cust.province);

	int i = 0;

	printf("Products purchased: \n\n");

	//This while loop only prints the fields of array of products in sale
	//which have a valid product in them
	//As we set all the unfilled spaces in array to 0. we can stop printing product data for all filled array fields.
	while (newsale.products[i] != 0)
	{
		fseek(pfd, (newsale.products[i]) * sizeof(PFREC), SEEK_SET);
		fread(&prod, sizeof(PRODUCT), 1, pfd);

		printf("Product ID: >%u<\n", newsale.products[i]);
		printf("Product Name: %s\n", prod.name);
		printf("Quantity: >%d<\n", newsale.quantity[i]);
		printf("Unit Cost: >$%d.%02d<\n", prod.unit_cost / 100, prod.unit_cost % 100);
		int x;
		x = newsale.quantity[i] * prod.unit_cost;
		printf("Cost: $%d.%02d\n\n", x / 100, x % 100);

		i++;
	}

	printf("Total cost: >$%d.%02d<\n", newsale.total_cost / 100, newsale.total_cost % 100);

}

int Reminder_due(FILE* salefd, FILE* cfd)
{
	HEADER_SALE hrec; // declare buffer header

	//_set_fmode(_O_TEXT); // Read and write the file in text

	/*Naming the text file to be concated with encoded name*/
	char fname[MAX_NAME] = "PAY-DUE";

	char encoded_date_c1[MAX_ENCODED];

	long encoded_date = 0;
	encoded_date = encode(newdate);
	snprintf(encoded_date_c1, MAX_ENCODED, "%ld", encoded_date);
	strncat(fname, encoded_date_c1, MAX_ENCODED);
	strncat(fname, ".txt", 5);
	/**/

	fseek(sales_file, 0, SEEK_SET); // point to beginging of sales file
	fread(&hrec, sizeof(HEADER_SALE), 1, sales_file); // read the header to buffer header hrec



	due_file = fopen(fname, "a+"); // open file PAY-DUE in appendix mode

	if (hrec.available_index == 1) //if file is empty close sales file
	{
		return 0;
	}

	int i = 1;

	while (i != hrec.available_index) { // while loop runs till the end of file
		fseek(salefd, i * sizeof(SALEREC), SEEK_SET); //set pointer to i record of sales file
		fread(&newsale, sizeof(SALEREC), 1, salefd); // copy contents of record to sales buffer
		fseek(cfd, i * sizeof(CFREC), SEEK_SET); // set poiner to i record of customers file
		fread(&newcustomer, sizeof(CFREC), 1, cfd); // copy content of record to customer buffer

		if ((encode(newdate) - encode(newsale.purchase_date)) == 30) // checks the difference between today's date and purchase date of record
		{
			fprintf(due_file, "Sale ID: %u		Customer ID: %u		" //print content of reminder if the difference is 30 days 
				"Customer Name: %s	Customer Phone#: %s		Total Due: %d.%02d\n"
				, newsale.saleid, newsale.cid, newcustomer.cname, newcustomer.phone_number,
				newsale.total_cost / 100, newsale.total_cost % 100);
		}
		i++; //increment record counter
	}

	fclose(due_file); // close file
	return 0;
}
/*
* This c file contains the functions for customers:
*
*The functions in this c file are :
*
* 1.) open_rel_p
* 2.) init_product_fields
* 3.) number_builder
* 4.) dollar_to_int
* 5.) add_products
* 6.) print_product_fields
* 7.) print_p
* 8.) delete_product
* 9.) update_product
*/

//This flag allows many fucntions such as strtok and strcpy on visual studio without errors
#define _CRT_SECURE_NO_WARNINGS 

#include "Task7.h"

//Defining a global variable buffer local to products.c
//The size of this buffer is equal to the biggest string. In this case product name.
#define INPUT_LEN 81
char input[INPUT_LEN];

PRODUCT del_product;

int open_rel_p(FILE** file)
{
	/*
 This fucntions gets the name of the products dat file and links it to the passed file pointer
  -The file is opened in "r+" mode meaning that the file must exist and is opened for read and write
*/
	char fname[fname_len];
	printf("Enter products file name: ");
	fgets(fname, fname_len, stdin);

	//_set_fmode(_O_BINARY);
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

//This initializes the array of sizes of the fields of products at compile time.
int product_field_size[MAX_FIELDS_PRODUCT] = { Pname_len, Class_len, Manu_len, Cost_len, ManuC_len, stock_len , stock_len};

void init_product_fields(PRODUCT* product_field_address)
{
	/*
	* This function initializes the array of pointers to the fileds of products at run time.
	* This array runs in paralled with the prompts and sizes for products.
	*/
	product_field_ptrs[0] = (char*)&product_field_address->name;
	product_field_ptrs[1] = (char*)&product_field_address->classification;
	product_field_ptrs[2] = (char*)&product_field_address->manufacturer;
	product_field_ptrs[3] = (char*)cost;
	product_field_ptrs[4] = (char*)&product_field_address->manufacturer_code;
	product_field_ptrs[5] = (char*)stocks;
	product_field_ptrs[6] = (char*)restock;
}

//This is an array of strings of prompts to get input related to add product.
//This array runs in parallel with the array of pointers and sizes for products.
char* adding_prompts_products[MAX_FIELDS_PRODUCT] =
{ "Enter Product Name: ",
  "Enter Product Classification: ",
  "Enter Product Manufacturer: ",
  "Enter Product Cost: ",
  "Enter Product Manufacturer Code: ",
  "Enter Product Stocks: ",
  "Enter Product Restock level: ",
};

int number_builder(char* cptr)
{
	/*
	* This function takes the string given to it and converts it into an integer
	*/
	int i = 0;
	int num = 0;

	while (ISDIGIT(*cptr))
	{
		/*The while loop works on taking the individual characters and subtracters the character '0'
		adding the number to the previous nth number*/
		num = 10 * num + (*cptr - '0');
		// increment the character pointer to the next number
		cptr++;
	}

	return num;
}

//This function is credited to: Dr. Larry Hughes
int dollar_to_int(char* cptr)
{
	// This function takes in a string of dollars then changes it to a decimal integer
	int decimal_count;
	int num;
	int decimal_flag = FALSE;

	num = 0;
	while (*cptr)
	{
		if (ISDIGIT(*cptr))
		{
			// Similar to te operations of number_builder
			num = num * 10 + (*cptr - '0');
		}
		else if (*cptr == '.')
		{
			// When encountering a decimal point the code it skips it then starts calculating the decimal places
				cptr++;
			decimal_flag = TRUE;

			for (int i = 0; i < DECIMAL_COUNT; i++)
			{
				if (ISDIGIT(*cptr))
				{
					num = num * 10 + (*cptr - '0');
					cptr++;
				}
				else if (*cptr == NUL)
				{
					num *= 10;
				}
			}
			break;
		}
		cptr++;
	}

	if (decimal_flag == FALSE)
	{
		num *= 100;
	}

	return num;
}

void add_products(FILE* pfd)
{
	/*
		This function adds a product into the products database.
		-The fucntion is based on the following structure for products:
			product:Name	Classification	Manufacturer	Unit Cost	Manufacturer code	In Stock
	*/
	HEADER_P hrec;
	int pid;
	//Goes to the first record in the products database file
	//This record is initialized as the header and contains the data for a record index available for writing into.
	fseek(pfd, 0, SEEK_SET);
	fread(&hrec, sizeof(HEADER_P), 1, pfd);

	printf("New product id is: %u\n", hrec.available_index);
	pid = hrec.available_index;

	int i = 0;

	//This while loop runs through the table
	//Starts at 0 and Ends at end of the table.

	while (i < MAX_FIELDS_PRODUCT)
	{
		//prints the prompts for add customers
		printf("%s", adding_prompts_products[i]);
		//gets the input from user
		fgets(input, INPUT_LEN, stdin);
		//This is macro, it expands into a strncpy
		STRCPY_P(input, i);
		//Traverse the table
		i++;
	}
	/*
	* The strings for unit_cost and stock are first stored into a seperate buffer
	* then they are converted into integers using the functions "dollar_to_int" & "number_builder"
	* these values are then stored into their proper fields in newproduct.
	*/
	newproduct.unit_cost = dollar_to_int(cost);
	newproduct.stock = number_builder(stocks);
	newproduct.restock = number_builder(restock);
	newproduct.status = ACTIVE_P;
	newproduct.pid = pid;

	fseek(pfd, (newproduct.pid) * sizeof(PFREC), SEEK_SET);
	fwrite(&newproduct, sizeof(PFREC), 1, pfd);

	hrec.available_index++;
	fseek(pfd, 0, SEEK_SET);
	fwrite(&hrec, sizeof(HEADER_P), 1, pfd);
}



void print_product_fields(PRODUCT* product_field_address) {
	/*
	* This function prints the contents of a variable of type PRODUCT.
	*/
	printf("Name: %s\n", product_field_address->name);
	printf("Classification: %s\n", product_field_address->classification);
	printf("Manufacturer: %s\n", product_field_address->manufacturer);
	printf("Unit Cost: %d.%02d\n", product_field_address->unit_cost / 100, product_field_address->unit_cost % 100);
	printf("Manufacturer Code: %s\n", product_field_address->manufacturer_code);
	printf("Stock: %d\n", product_field_address->stock);
	printf("Reorder level: %d\n", product_field_address->restock);
	putchar('\n');
}

void print_p(FILE* file, int pid)
{
	/*
		This function prints a specified customer record (parameter cid) from a sprcified file (parameter file)
	*/

	fseek(pfile, pid * sizeof(PFREC), SEEK_SET);
	fread(&newproduct, sizeof(PRODUCT), 1, pfile);
	printf("ID: >%d< status: >%d< Product_Name: >%s< Classification: >%s< Manufacturer_Name: >%s<"
		"Unit_Cost: >$%d.%02d< Manufacturer_Code: >%s< In_Stock: >%d< \nRestock: >%d<\n\n",
		newproduct.pid, newproduct.status, newproduct.name, newproduct.classification, newproduct.manufacturer, newproduct.unit_cost / 100,
		newproduct.unit_cost % 100, newproduct.manufacturer_code, newproduct.stock, newproduct.restock);
}

void delete_product(FILE* pfd)
{
	HEADER_C hrec;
	int pid = 0;

	//Gets the Header record from the products file
	fseek(pfd, 0, SEEK_SET);
	fread(&hrec, sizeof(HEADER_C), 1, pfd);

	//Here we use the available_index stores in the header of the file to identify where the filled/valid record ends
	//If a record number is greater than or equal to available_index, then it should be empty as available_index is used
	//to denote the next available empty record for writing data into.
	do {
		printf("Enter the product id: ");
		scanf("%d", &pid);
	} while (pid > hrec.available_index || pid < 0);

	//Retrives the data of the product to be deleted
	fseek(pfd, (pid * sizeof(PFREC)), SEEK_SET);
	fread(&del_product, sizeof(PFREC), 1, pfd);

	//Checks if the product has already been deleted already
	if (del_product.status == DELETED_P)
	{
		printf("This product is already deleted!!\n");
		return;
	}
	else {
		//checks if the product still has stock left as products can only be deleted 
		if (del_product.stock > 0)
		{
			printf("This product still has remaining stock!!\n");
		}
		else
		{
			//change the status to deleted 
			del_product.status = DELETED_P;

			//writes the updated produtced record back to the file
			fseek(pfd, (pid * sizeof(PFREC)), SEEK_SET);
			fwrite(&del_product, sizeof(PFREC), 1, pfd);

			//prints the record to the screen
			print_p(pfd, pid);
		}
		
	}

}

//This is an array of strings of prompts to get input related to update product.
//This array runs in parallel with the array of pointers and sizes for products.
char* updating_prompts_products[MAX_FIELDS_PRODUCT] =
{ "Update Product Name: ",
  "Update Product Classification: ",
  "Update Product Manufacturer: ",
  "Update Product Cost: ",
  "Update Product Manufacturer Code: ",
  "Update Product Stocks: ",
  "Update Product Restock level: ",
};

void update_product(FILE* pfd) 
{
	/*
	This function updates a product from the customer database.
	-The fucntion is based on the following structure for customers:
		Product: Name, Classification, Manufacturer, Cost, Manufacturer Code, Stocks
*/
	HEADER_C hrec; //header
	int pid; //record number
	int option = -1; // a holder to store the option entered by the user
	int ext = -1;
	int i = 0;
	fseek(pfd, 0, SEEK_SET); //set pointer to first header
	fread(&hrec, sizeof(HEADER_P), 1, pfd); //read first 


	do {
		do {
			printf("Enter product record number: ");
			scanf("%d", &pid); //saves the enterd id into the record id
			getchar();
		} while (pid < 0 || pid >= hrec.available_index); // the do while continues to operate until a valid id is entered

		fseek(pfd, (pid) * sizeof(PFREC), SEEK_SET); // points to wanted record
		fread(&newproduct, sizeof(PFREC), 1, pfd); // reads the content of the record into a buffer called newproduct

		if (newproduct.status == ACTIVE_P) { // Checks whether the product still exists in the database
			printf("Product found:\n");
			print_product_fields(&newproduct); // prints the content of selected product
			printf("Chose option\n");
			while (i < MAX_FIELDS_PRODUCT) {
				// shows the updated options for product
				printf("%d.%s\n", i, updating_prompts_products[i]);
				i++;
			}
			i = 0; // reset i for next itteration
			printf("7.Exit\n");
			printf("Option: ");
			scanf("%d", &option); // Reads selected option from user
			getchar();

			switch (option)
			{
			case 3:
				//special case: cost
				printf("%s", updating_prompts_products[option]); //prompt user to update selected option
				fgets(input, INPUT_LEN, stdin); //read change from user
				STRCPY_P(input, option); // update product field in buffer record
				newproduct.unit_cost = dollar_to_int(cost);
				break;

			case 5:
				//special case: Stock
				printf("%s", updating_prompts_products[option]); //prompt user to update selected option
				fgets(input, INPUT_LEN, stdin); //read change from user
				STRCPY_P(input, option); // update product field in buffer record
				newproduct.stock = number_builder(stocks);
				break;

			case  6:
				//special case: restock level
				printf("%s", updating_prompts_products[option]); //prompt user to update selected option
				fgets(input, INPUT_LEN, stdin); //read change from user
				STRCPY_P(input, option); // update product field in buffer record
				newproduct.restock = number_builder(restock);
				break;

			case 7:
				//case to exit the update on a record 
				break;

			default:
				printf("%s", updating_prompts_products[option]); //prompt user to update selected option
				fgets(input, INPUT_LEN, stdin); //read change from user
				STRCPY_P(input, option); // update product field in buffer record
			}
			
			fseek(pfd, (pid) * sizeof(PFREC), SEEK_SET); //point to wanted record
			fwrite(&newproduct, sizeof(PFREC), 1, pfd); // write new updated fields to desired record
		}
		else {
			printf("Product >%d< is deleted\n", pid); // if the record is deleted this is statement is printed
		}
		printf("Enter 1. to change more products\n");
		printf("      0. to exit\n");
		printf("Option: ");
		scanf("%d", &ext);
	} while (option != 7 && ext != 0); //exit while loop if 7 is entered
}

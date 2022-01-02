/*
*													Main.c
* The main program comprises of:
*	1. The relative file names and calls (both automatic and from the user), as well as closing them
*	2. The main user interface to get the user selections from operations such as make sale, add a customes, delete a customer
*		and many more. 
*	3. To control the flow of the program by making function calls for each operation and having the functions return
*	   the control back to main. 
*/

#define _CRT_SECURE_NO_WARNINGS
#include "Task7.h"

CUSTOMER  newcustomer;    //Global variable for customers
PRODUCT   newproduct;     //Global variable for products
SUPPLIERS newsupplier;	  //Global variable for supplier
SALE      newsale;		  //Global variable for sales
BACKORDER newbackorder;   //Global variable for backorders

int main(int argc, char* argv[])
{
	Enter_Date();			//Function to get the current date from the user	
	Reminder_Date(newdate); //Function to calculate the reminder date based on the current date entered by the user
	getchar();
	
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

	int options = -1;		//Local variable used for storing user selection of operation

	int file_option = -1;	//Local variable used for storing user selection for database files

	printf("Enter file names manually by pressing 1. or automatic by 0.\noption: ");
	scanf("%d", &file_option);
	getchar();					//Catches the LF from the scanf and prevents inputs into the next scanf.

	//file_option 1: Will prompt the user for inputs for the database file names.
	//file_option 2: will automaticaly open the files with the defaulted names and assign file pointers to them.
	if (file_option == 1)
	{
		if(open_rel_c(&cfile) == -1) return 0;
		if(open_rel_p(&pfile) == -1) return 0;
		if(open_rel_s(&sfile) == -1) return 0;
		if(open_rel_sale(&sales_file) == -1) return 0;
	}
	else if (file_option == 0)
	{
		//_set_fmode(_O_BINARY);
		cfile = fopen("customers.dat", "r+b");
		//_set_fmode(_O_BINARY);
		pfile = fopen("products.dat", "r+b");
		//_set_fmode(_O_BINARY);
		sfile = fopen("suppliers.dat", "r+b");
		//_set_fmode(_O_BINARY);
		sales_file = fopen("sales.dat", "r+b");
	}

	//Opening the backorders.dat file
	//_set_fmode(_O_BINARY);
	bfile = fopen("backorders.dat", "r+b");

	//function to open ordersfilled file.
	open_file_ordersfilled();

	while (options != Done){
		printf("\nWhat would you like to do: \n"
			"1. Add customer    2. Add product    3.  Add supplier\n"
			"4. Make sale \n"
			"5. Update Customer 6. Update Product 7.  Update Supplier\n"
			"8. Delete Customer 9. Delete Product 10. Delete Supplier\n"
			"11.Backorder fill\n"
			"0. Exit\n"
			"Selection: ");
		scanf("%d", &options);
		
		getchar();  //catches the LF from keyboard (in the input buffer) and prevents unwanted inputs into later scanf

		switch (options) {
		case Customer: // Adding customer
			printf("Customer operations\n");
			initalize_customer_fields_ptr(&newcustomer);
			add_customers(cfile);
			print_customer(&newcustomer);
			break;

		case Product: //Adding product
			printf("Product operations\n");
			init_product_fields(&newproduct);
			add_products(pfile);
			print_product_fields(&newproduct);
			break;

		case Supplier: //Adding Supplier
			printf("Supplier operations\n");
			init_supplier_fields(&newsupplier);
			add_Supplier(sfile);
			print_supplier_fields(&newsupplier);
			break;

		case Sale: // making sale
			printf("Sales operations\n");
			make_sale(sales_file, pfile, cfile);
			break;

		case upd_cust: // updating customer
			printf("Updating customer\n");
			initalize_customer_fields_ptr(&newcustomer);
			update_customer(cfile);
			break;

		case upd_prod: // updating product
			printf("Updating product\n");
			init_product_fields(&newproduct);
			update_product(pfile);
			break;

		case upd_supp:
			printf("Updating supplier\n");
			init_supplier_fields(&newsupplier);
			update_supplier(sfile);

		case del_cust: // deleting customer
			printf("Deleting a customer\n");
			delete_customer(cfile);
			break;

		case del_prod: // deleting product
			printf("Deleting a product\n");
			delete_product(pfile);
			break;

		case del_supp: // deleting supplier
			printf("Deleting a supplier\n");
			delete_supplier(sfile);
			break;

		case fill_backorder: //Filling a backorder
			printf("Filling a backorder\n");
			fill_back_order(cfile, pfile, bfile);
			break;
		}
	} 
	
	open_file_reorder();
	Reminder_due(sales_file, cfile);
	find_reorders(pfile, sfile);

	printf("End of operations\n");
	getchar();

	/*Closing of used files*/
	fclose(cfile);
	fclose(pfile);
	fclose(sfile);
	fclose(sales_file);
	fclose(reorder);
	fclose(ordersfilled);
	return 0;
}
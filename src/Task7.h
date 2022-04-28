/*
Task 7:
*This program is improvement over the Task 6 whihc handeles database for Stellarton Solar and can perform the following tasks:
*
 1.) This program can add a new customer into the customer database.
		--Adding a new customer is based on the following structure:
			customer: customer id, Name(FN LN), Business Name, Street Address, Town, Province, Postal Code, Telephone

 2.) It can add a new product into the product database.
		--Adding  new product is based on the following structure:
			product: product id, Name, Classification, Manufacturer, Unit Cost,	Manufacturer code, Stock

 3.) It can add a new supplier into the supplier database.
		--Adding  new supplier is based on the following structure:
			Supplier: supplier id, Manufacturer, Contact, Company, Address, Telephone

 4.) Finally, this program can also make a sale. 
		--Making a sale is based on the following structure:
			Sales: customer id, 1{products}20, 1{Quantity}20, total_cost, saleid, payment date, reminder date

***********************************************************************************************************************
*This program has new functions:
* 1.) Update a field in customer, product or supplier records.
* 2.) Delete a customer, product or supplier record.
* 3.) Decrement the stock of product when a purchase of that product is made. This program also generates a daily
*	  orders file which contains reorder details of the products which are below restock level or 0 and the details of
*	  their respective suppliers.
* 4.) Generate a 30 Day payment due reminder file to indicate the customers who have payments due at that date.
* 5.) Create a Backorder if a customers wants to purchase a product which is not in stock or the asking amount in not 
*     available.
* 6.) Be able to fill a back order and send the remaining stock if any back into the inventory.

***********************************************************************************************************************
Group Number: 6
Members:
	-Abdullah Al Alawi (B00832821)
	-Waleed Seleman    (B00827774)
	-Manav Sohi        (B00844935)

Date: December 07th, 2021
***********************************************************************************************************************
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "customers.h"
#include "products.h"
#include "suppliers.h"
#include "sales.h"
#include "date.h"
#include "backorder.h"

#define NUL '\0'

#define fname_len 50
#define START     1000

#define MAX_LEN   256

//States for opertion prompts in main

#define Done     0			//exit the code
#define Customer 1			//add a customer
#define Product  2			//add a product
#define Supplier 3			//add a supplier
#define Sale     4			//make a sale
#define upd_cust 5			//update a customer field
#define upd_prod 6			//update a product field
#define upd_supp 7			//update a supplier field
#define del_cust 8			//delete a customer
#define del_prod 9			//delete a product
#define del_supp 10			//delete a supplier
#define fill_backorder 11	//fill a backorder

FILE* infile;    
FILE* reorder;
FILE* bfile;
FILE* ordersfilled;
char fname[fname_len];

//Macro to remove LF before the null in a record.
#define TRUNCATE(name)  name[strlen(name) - 1] = NUL

//Customers Functions (from customers.c):
extern CUSTOMER newcustomer;

extern int open_rel_c(FILE** file);
extern void initalize_customer_fields_ptr(CUSTOMER* cust_field_adr);
extern void add_customers(FILE* cfd);
extern void print_customer(CUSTOMER* cust_field_adr);
extern void delete_customer(FILE* cfd);
extern void update_customer(FILE* cfd);

//Products Fucntions (from products.c):
extern PRODUCT newproduct;

extern int open_rel_p(FILE** file);
extern void init_product_fields(PRODUCT* product_field_address);
extern void add_products(FILE* pfd);
extern void print_product_fields(PRODUCT* product_field_address);
extern void delete_product(FILE* pfd);
extern void update_product(FILE* pfd);

//Suppliers Functions (from suppliers.c):
extern SUPPLIERS newsupplier;

extern int open_rel_s(FILE** file);
extern void init_supplier_fields(SUPPLIERS* supplier_field_address);
extern void add_Supplier(FILE* spfd);
extern void print_supplier_fields(SUPPLIERS* supplier_field_address);
extern void delete_supplier();
extern void update_supplier(FILE* sfd);

//Sales Functions (from sales.c):
extern SALE newsale;

extern int open_rel_sale(FILE** file);
extern int make_sale(FILE* salefd, FILE* pfd, FILE* cfd);
extern void print_sale(FILE* salefd, FILE* cfd, FILE* pfd, unsigned int id);
extern int Reminder_due(FILE* salefd, FILE* cfd);

//Date functions (from date.c):

extern void Enter_Date();
extern int Check_Date(unsigned int year, unsigned int month, unsigned int day);
extern void Reminder_Date(DATE y);
extern long encode(DATE y);

//For reorders:
extern void open_file_reorder();
extern int find_reorders(FILE* pfd, FILE* sfd);

//backorders: 
extern BACKORDER newbackorder;
extern void backorders(FILE* salefd, FILE* cfd, FILE* pfd, unsigned int pid, unsigned int cid, unsigned int saleid, int quantity);

//fill backorders:
extern void open_file_ordersfilled();
extern void fill_back_order(FILE* cfd, FILE* pfd, FILE* bfd);
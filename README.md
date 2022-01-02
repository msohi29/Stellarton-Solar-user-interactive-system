# Stellarton-Solar-user-interactive-system

This software can:

  o	add a new customer, product, supplier,
  
  o	make a sale,
  
  o	place orders automatically of products below restock level and maintain a daily record of ordered products. The name of the daily orders files has the current encoded date
    at the end of it, 
    
  o	update any field in existing customer, product, supplier record as well as delete an existing record. The deletion is logical and not physical.
  
  o	make a backorder of purchase of products which are out of stock,
  
  o	fill a backorder and maintain a backorders filled file,
  
  o	generate a payment due reminder for sales made 30 days ago.
  

The format of the database files is predefined in order to use relative file access. Which allows all searches to be constant time.
The contents of the database were provided by Dr. Larry Hughes for the purpose of this project.

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
**How to Run**
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

To compile the program, use gcc and run the following command in the terminal of your working directory:

gcc ./main.c ./backorder.c ./customers.c ./date.c ./products.c ./reorder.c ./sales.c ./suppliers.c -o myapp

or

make

then to run the program use:

./myapp

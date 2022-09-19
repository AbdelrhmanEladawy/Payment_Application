# Payment_Application
Its simulation of Payment Application using **C language** concepts.
this project is was developed for Udacity course project, Advanced embedded system.

There are different types of transactions you can make, _SALE_, _REFUND_, _Pre-Authorization_, and _VOID_.
* SALE: means to buy something and its price will be deducted from your bank account.
* REFUND: this means that you will return something and wants your money back to your bank account.
* Pre-Authorization: means holding an amount of money from your account, e.g Hotel reservation.
* VOID: this means canceling the transaction, e.g if the seller entered the wrong amount.

## Project flow 

#### 1) Development environment preparation
    Make the folders, source, and header files. Also include libraries and link the files together.
#### 2) Implement the card module
    get card information and check the format and if it's meeting the requirement. Then send it to terminal.
#### 3) Implement the terminal module
    Get the data from card and use Luhn Algorithm to check card PAN number. Then get the Transaction amount and doing some processes before send it to bank server.
#### 4) Implement the server module
    Checks if the PAN number is in the database or if the card is stolen. Also, check if the balance is available or not. Then save the transaction into the database.
#### 5) Implement the application
    Implement the main function, write the logic of the code, and modify the output printed on the screen.
#### 6) Testing the application
    try as many test cases and fix the problems.

## how to run the project
it's recommended to use C99 version or above of the C programming language standard. You can use any c/c++ editor with MinGW compiler. Visual Studio for example.


/*
*   Author        : Abdelrhman Ahmed Ealdawy
*   Created       : 08/08/2022
*   last modfiled : 
*
*/

#ifndef _SERVER
#define _SERVER 

#include "../Terminal/terminal.h"
// ******************************** typedef ******************************** //

typedef enum EN_transState_t
{
    APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD, INTERNAL_SERVER_ERROR
}EN_transState_t;

typedef enum EN_serverError_t
{
    Ok, SAVING_FAILED, TRANSACTION_NOT_FOUND, ACCOUNT_NOT_FOUND, LOW_BALANCE
}EN_serverError_t;

typedef struct ST_accountsDB_t
{
    float balance;
    uint8_t primaryAccountNumber[20];
}ST_accountsDB_t;

typedef struct ST_transaction_t
{
    ST_cardData_t cardHolderData;
    ST_terminalData_t terminalData;
    EN_transState_t transState;
    uint32_t transactionSequenceNumber;
}ST_transaction_t;


// ******************************** global variable ******************************** //

ST_accountsDB_t accounts_database[255];
static uint8_t accountsDB_num_of_element = 0; //index to know the last element in the array

ST_transaction_t transactions_database[255];
static uint8_t transactionDB_num_of_element = 0; //index to know the last element in the array

// ******************************** function prototype ******************************** //


/*
* This function will take all transaction data and validate its data.
* It checks the account details and amount availability.
* If the account does not exist return DECLINED_STOLEN_CARD, if the amount is not available will return
  DECLINED_INSUFFECIENT_FUND, if a transaction can't be saved will return INTERNAL_SERVER_ERROR and will 
  not save the transaction, else returns APPROVED.
* It will update the database with the new balance.
*/
EN_transState_t recieveTransactionData(ST_transaction_t* transData);
/*
* This function will take card data and validate these data.
* It checks if the PAN exists or not in the server's database.
* If the PAN doesn't exist will return DECLINED_STOLEN_CARD, else will return OK
*/
EN_serverError_t isValidAccount(ST_cardData_t* cardData);
/*
* This function will take terminal data and validate these data.
* It checks if the transaction's amount is available or not.
* If the transaction amount is greater than the balance in the database will return LOW_BALANCE, else will return OK
  You should deliver a maximum 2min video to discuss your implementation and run different test cases on this function
*/
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData);
/*
* This function will take all transaction data into the transactions database.
* It gives a sequence number to a transaction, this number is incremented once a transaction is processed into the server.
* If saves any type of transaction, APPROVED or DECLINED, with the specific reason for declining/transaction state.
* If transaction can't be saved will return SAVING_FAILED, else will return OK
*/
EN_serverError_t saveTransaction(ST_transaction_t* transData);
/*
* This function will take a transaction sequence number and search for this transaction in the database.
* If the transaction can't be found will return TRANSACTION_NOT_FOUND, else will return OK and store 
  the transaction in a structure
*/
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData);

// generate random PAN and balance and save it into database. for test the code
uint8_t find_account_index_number(ST_cardData_t* cardData);
//using concpet of Luhn Algorithm, to get PAN number and fill the data base with accounts
void get_random_server_account(void);
//to add specific account to database for test the system
void ForTEST_add_server_account(ST_cardData_t card , float balance);
//find index to return balance for specific account
uint8_t check_balance(ST_cardData_t card);

#endif/* _SERVER.H */
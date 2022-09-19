/*
*   Author        : Abdelrhman Ahmed Ealdawy
*   Created       : 08/08/2022
*
*/

#include "server.h"

//to search for account in database
static int8_t index_number = -1;

EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{
	find_account_index_number(cardData);
	if (index_number == -1)
		return ACCOUNT_NOT_FOUND;
	else
		return Ok;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{
	if (termData->transAmount > accounts_database[index_number].balance)
		return LOW_BALANCE;
	else
		return Ok;
}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	static uint32_t Sequence = 1000000000;
	
	if (transactionDB_num_of_element >= 255)
		//if the number of transaction in array reach 255 , it will failed
		return SAVING_FAILED;

		//let Sequence Number start with 1000000000
		transData->transactionSequenceNumber = ++Sequence;		
	
	transactions_database[transactionDB_num_of_element] = *transData;
	transactionDB_num_of_element++;
	
	return oK;
}

EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	if (isValidAccount(&transData->cardHolderData) == ACCOUNT_NOT_FOUND)
		return DECLINED_STOLEN_CARD;
	else if (isAmountAvailable(&transData->terminalData) == LOW_BALANCE)
		return DECLINED_INSUFFECIENT_FUND;
	else if (saveTransaction(transData) == SAVING_FAILED)
		return INTERNAL_SERVER_ERROR;
	else
	{
		//update the database with the new balance
		accounts_database[index_number].balance -= transData->terminalData.transAmount;
		return APPROVED;
	}
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData)
{
	uint8_t i;
	for (i = 0; i < transactionDB_num_of_element; i++)
	{
		if (transactions_database[i].transactionSequenceNumber == transactionSequenceNumber)
		{
			*transData = transactions_database[i];
			return oK;
		}
	}
	return TRANSACTION_NOT_FOUND;
}

//search for account in database using PAN number using concept of Binary search
uint8_t find_account_index_number(ST_cardData_t* cardData)
{
	int8_t i = 0, j = accountsDB_num_of_element, k = (i + j) / 2; //for while loop
	uint8_t comp_coff=0, l; //l is for for loop (to compare two arrays), comp_coff is for check the state
	while (i <= j)
	{
		comp_coff = 0;
		for (l = 0; l < 20 && cardData->primaryAccountNumber[l] != '\0' ; l++)
		{	//check if the 20 PAN number are the same or not
			if (cardData->primaryAccountNumber[l] > accounts_database[k].primaryAccountNumber[l])
			{
				comp_coff = 1;
				break;
			}

			if (cardData->primaryAccountNumber[l] < accounts_database[k].primaryAccountNumber[l])
			{
				comp_coff = 2;
				break;
			}
		}

		if (comp_coff == 0) //if the two number are equal
		{
			index_number = k; //update the index global variable
			return k;
		}
		else if (comp_coff == 1) //if the number is greater than
			i = k + 1;
		else if (comp_coff == 2) //if the namber is smaller than
			j = k - 1;

		k = (i + j) / 2;
	}

	index_number = -1;

	//return the index for insert new element
	if (comp_coff == 1)
		return (i >= 0) ? (i) : (0); 
	else if (comp_coff == 2)
		return (j >= 0) ? (j+1) : (0);
	else
		return k;
}

//using concpet of Luhn Algorithm, to get PAN number and fill the data base with accounts
void get_random_server_account(void)
{
	ST_cardData_t temp = {0};
	//generate 15 random number, and the last number modified to meet the rule of PAN number 
	uint8_t i,j, number, sum = 0; //number and sum is for PAN rule.
	for (i = 0; i < 15; i++)
	{
		number = (uint8_t)(rand() % 10);
		temp.primaryAccountNumber[i] = number+'0';

		if ((i + 1) % 2 == 0)
		{
			sum += (2 * number) % 10;
			sum += (2 * number) / 10;
		}
		else
			sum += number;
	}

	//is the sum is odd number,
	if ((sum % 10) % 2 != 0) 
		number = ( 19 - (sum % 10))/2 ; //this will lead to make last digit in the sum = 0;
	else //if even number
		number = ( (10 - (sum % 10))%10 )/2; //this will lead to make last digit in the sum = 0;

	temp.primaryAccountNumber[15] = number+'0';
	sum += 2 * number;

	for( i = 17 ; i<20 ; i++)
		temp.primaryAccountNumber[i] = '0';

	//binray search
	number = find_account_index_number(&temp);
	j = 0;

	if (index_number != -1) //it shouldn't be repeated in database
	{
		get_random_server_account(); //re-call the function
		return;
	}

	//save the account in database in correct place
	if (accountsDB_num_of_element < 255)
	{
		for (j = accountsDB_num_of_element; j > number; j--)
		{
			accounts_database[j] = accounts_database[j - 1];
		}
		
		memcpy(accounts_database[j].primaryAccountNumber, temp.primaryAccountNumber, sizeof(accounts_database[j].primaryAccountNumber));
		accounts_database[j].balance = (rand() % 100000) / (float)1; //make random balance
		
		accountsDB_num_of_element++; //data base index
	}

}

//to add specific account to database for test the system
void ForTEST_add_server_account(ST_cardData_t card, float balance)
{
	memcpy(accounts_database[accountsDB_num_of_element].primaryAccountNumber, card.primaryAccountNumber, sizeof(card.primaryAccountNumber));
	accounts_database[accountsDB_num_of_element].balance = balance;
	accountsDB_num_of_element++;
}

//find index to return balance for specific account
uint8_t check_balance(ST_cardData_t card)
{
	find_account_index_number(&card);
	return index_number;
}
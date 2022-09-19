/*
*   Author        : Abdelrhman Ahmed Ealdawy
*   Created       : 08/08/2022
*
*/

#include "terminal.h"

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	//_strdate gives in form DD/MM/YY
	_strdate(termData->transactionDate); //note that is works for windows only ....

	//the following lines of code are for just meet the required (witch is format of year should be 20$$ )
	//................................
	termData->transactionDate[8] = termData->transactionDate[6];
	termData->transactionDate[9] = termData->transactionDate[7];
	termData->transactionDate[6] = '2';
	termData->transactionDate[7] = '0';
	termData->transactionDate[10] = '\0';
	//................................

	printf("the cuurent date : %s\n", termData->transactionDate );

	if ((strlen(termData->transactionDate) != 10) )
		return WRONG_DATE;
	else
		return oK;
}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
	//to convert str to int and save to compare the date
	DDMMYYDateType_t CurrDateMMYY = { 0 }, CardExpDateMMYY = { 0 };

	char MMYY_str[6] = ""; //just for get_number in conversion

	//the current date is in form DD/MM/YY
	//to get day from current date 
	strncpy_s(MMYY_str, sizeof(MMYY_str), termData.transactionDate , 2);
	CurrDateMMYY.day = atoi(MMYY_str);

	//to get month from current date 
	strncpy_s(MMYY_str, sizeof(MMYY_str), termData.transactionDate + 3, 2);
	CurrDateMMYY.month = atoi(MMYY_str);
	
	//to get year from current date 
	strncpy_s(MMYY_str, sizeof(MMYY_str), termData.transactionDate + 8, 2);
	CurrDateMMYY.year = atoi(MMYY_str);

	//the card exp date is in form MM/YY
	//to get month from exp date
	strncpy_s(MMYY_str, sizeof(MMYY_str), cardData.cardExpirationDate , 2);
	CardExpDateMMYY.month = atoi(MMYY_str);

	//to get year from exp date
	strncpy_s(MMYY_str, sizeof(MMYY_str), cardData.cardExpirationDate+3, 2);
	CardExpDateMMYY.year = atoi(MMYY_str);

	if (CardExpDateMMYY.year < CurrDateMMYY.year)
		return EXPIRED_CARD;
	else if ( (CardExpDateMMYY.year == CurrDateMMYY.year) && (CardExpDateMMYY.month < CurrDateMMYY.month))
		return EXPIRED_CARD;
	else
		return oK;
}

// ******************* Luhn Check Algorithm ******************* //
// it is an algorithm to check if the PAN naumber is correct or not, the idea is 
// very simple, just multiply the add the odd digit and double of even digits.
EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData)
{
	uint8_t i = 0, sum = 0, get_number = 0;

	for (i = 0; i < 20 ;  i++) //20 is size of PAN array
	{
		get_number = (uint8_t)(cardData->primaryAccountNumber[i]); //convert char to number
		if (get_number >= (uint8_t)'0')
			get_number -= (uint8_t)'0';
		if ((i+1) % 2 == 1) //if it is an odd number , just add it to the variable.
			sum += get_number;
		else			//if it is an even number
		{				//mutiply it by 2, if the resulting two digit, add each digit individual
			sum += (2 * get_number )% 10;
			sum += (2 * get_number )/ 10;
		}
	}

	if (sum % 10 == 0)
		return oK;
	else
		return INVALID_CARD;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	printf("please enter the Transaction Amount : ");
	scanf_s("%f", &termData->transAmount);

	if (termData->transAmount < 0.01)
		return INVALID_AMOUNT;
	else
		return oK;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	if (termData->transAmount > termData->maxTransAmount)
		return EXCEED_MAX_AMOUNT;
	else
		return oK;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
	fflush(stdin); //clear input buffer
	scanf_s("%f", &termData->maxTransAmount);

	if (termData->maxTransAmount < 1)
		return INVALID_MAX_AMOUNT;
	else
		return oK;
}
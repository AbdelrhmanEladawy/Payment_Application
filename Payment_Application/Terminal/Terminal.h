/*
*   Author        : Abdelrhman Ahmed Ealdawy
*   Created       : 08/08/2022
*   last modfiled :
*
*/

#ifndef _TERMINAL 
#define _TERMINAL 

#include "../Card/card.h"
#include <dos.h>

// ******************************** typedef ******************************** //

typedef struct ST_terminalData_t
{
    float transAmount;
    float maxTransAmount;
    uint8_t transactionDate[11];
}ST_terminalData_t;

typedef enum EN_terminalError_t
{
    oK, WRONG_DATE, EXPIRED_CARD, INVALID_CARD, INVALID_AMOUNT, EXCEED_MAX_AMOUNT, INVALID_MAX_AMOUNT
}EN_terminalError_t;

//this struct is for date compare, convert the input date from str to int and save it into struct
typedef struct DDMMYYDateType_t
{
    uint8_t day;
    uint8_t month;
    uint8_t year;
}DDMMYYDateType_t ;

// ******************************** function prototype ******************************** //

/*
* The function will read the current date from your computer and store it into terminal data with the mentioned size and format.
* Transaction date is 10 characters string in the format DD/MM/YYYY, e.g 25/06/2022.
* If the transaction date is NULL, less than 10 characters or wrong format will return WRONG_DATE error, else return OK.
*/
EN_terminalError_t getTransactionDate(ST_terminalData_t* termData);
/*
* This function compares the card expiry date with the transaction date.
* if the card expiration date is before the transaction date will return EXPIRED_CARD, else return OK.
*/
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData);
/*
* This function checks if the PAN is a Luhn number or not.
* If PAN is not a Luhn number will return INVALID_CARD, else return OK.
*/
EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData);
/*
* The function will read the current date from your computer and store it into terminal data with the mentioned size and format.
* Transaction date is 10 characters string in the format DD/MM/YYYY, e.g 25/06/2022.
* If the transaction date is NULL, less than 10 characters or wrong format will return WRONG_DATE error, else return OK.
*/
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData);
/*
* This function compares the transaction amount with the terminal max amount.
* If the transaction amount is larger than the terminal max amount will return EXCEED_MAX_AMOUNT, else return OK.
*/
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData);
/*
* This function sets the maximum allowed amount into terminal data.
* Transaction max amount is a float number.
* If transaction max amount less than or equal to 0 will return INVALID_MAX_AMOUNT error, else return OK.
*/
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData);

#endif/* _TERMINAL.H */